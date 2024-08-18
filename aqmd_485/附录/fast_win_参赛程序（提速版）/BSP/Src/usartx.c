/**
  ******************************************************************************
  * File Name          : usartx.c
  * Description        : 用于串口通信协议相关的应用
	* @author            : SuperEgo-郭耀辉
  ******************************************************************************
  * @attention
  *
  * sbus协议对应的串口5默认会初始化好。用户只需在任何需要的时候，直接访问SBUS_CH
	*	对象，或者直接通过sbus_show_ch函数打印出所有通道的当前值即可
  *
  ******************************************************************************
  */

#include "usartx.h"
#include <stdarg.h>
//串口重定向
//注意此步骤要想生效，必须配置工程target选项支持Use MicroLIB
int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart3,(uint8_t *)&ch,1,HAL_MAX_DELAY);
//	 HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&ch, 1); 
	return ch;
}

char dma_printf_buf[DMA_PRINTF_MAX];
void dma_printf (char *fmt, ...)
{ 
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(dma_printf_buf, DMA_PRINTF_MAX+1, fmt, arg_ptr);
	HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&dma_printf_buf, strlen(dma_printf_buf)); 
}


uint8_t UART3_RX_BUF[USART3_REC_LEN];//串口3接收缓冲区
uint8_t UART5_RX_BUF[1];//串口5接收缓冲区
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_uart5_rx;

uint8_t USART5_RX_STA;
uint8_t USART5_SBUS_RX_BUF[USART5_REC_LEN];

//重新开启串口中断使能，此函数仍需要在main函数最开始初始化一次
void my_uart_IT_re_enable(void)
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, UART3_RX_BUF, sizeof(UART3_RX_BUF));
	__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);	
}

///////////////////////////CODBOT 4WD ROS 机器人通信协议(串口3)////////////////////////////
//具体协议见附录：CODBOT ROS机器人通讯协议(4WD 差速底盘)，CODBOT ROS机器人通讯协议举例.txt

//CODBOT 4WD ROS 机器人通信协议校验校验函数
MY_UART_ERROR_CHECK my_uart_check_correct(uint8_t* data)
{
	int i;
	uint8_t xor_check = 0;
	int len = data[1];//获取报文长度
	
	//报头校验
	if(data[0] != 0xCD)
	{
		return START_ERROR;
	}
	
	//异或校验
	for (i = 2; i<len+2; i++)
	{
		xor_check ^= data[i];
	}
	if(data[len+2] != xor_check)
	{
		return XOR_ERROR;
	}
	
	return CORRECT;
}

//CODBOT 4WD ROS 机器人通信协议校验回调函数(直接放在串口空闲中断中即可校验协议)
void CODBOT_4WD_ROS_Callback(void)
{
	MY_UART_ERROR_CHECK check = my_uart_check_correct(UART3_RX_BUF);
	if(check == CORRECT)
		{
			printf("CORRECT\r\n");
		}
		else if(check == START_ERROR)
		{
			printf("START_ERROR\r\n");
		}
		else if(check == XOR_ERROR)
		{
			printf("XOR_ERROR\r\n");
		}
}

/////////////////////////////sbus协议相关代码(串口5)/////////////////////////////
//代码参考文章见附录：sbus协议csdn代码参考

SBUS_CH_Struct SBUS_CH;

//将sbus信号转化为通道值
uint8_t update_sbus(uint8_t *buf)
{
    if (buf[23] == 0)
    {
        SBUS_CH.ConnectState = 1;
        SBUS_CH.CH1 = ((int16_t)buf[ 1] >> 0 | ((int16_t)buf[ 2] << 8 )) & 0x07FF;
        SBUS_CH.CH2 = ((int16_t)buf[ 2] >> 3 | ((int16_t)buf[ 3] << 5 )) & 0x07FF;
        SBUS_CH.CH3 = ((int16_t)buf[ 3] >> 6 | ((int16_t)buf[ 4] << 2 ) | (int16_t)buf[ 5] << 10 ) & 0x07FF;
        SBUS_CH.CH4 = ((int16_t)buf[ 5] >> 1 | ((int16_t)buf[ 6] << 7 )) & 0x07FF;
        SBUS_CH.CH5 = ((int16_t)buf[ 6] >> 4 | ((int16_t)buf[ 7] << 4 )) & 0x07FF;
        SBUS_CH.CH6 = ((int16_t)buf[ 7] >> 7 | ((int16_t)buf[ 8] << 1 ) | (int16_t)buf[9] << 9 ) & 0x07FF;
        SBUS_CH.CH7 = ((int16_t)buf[ 9] >> 2 | ((int16_t)buf[10] << 6 )) & 0x07FF;
        SBUS_CH.CH8 = ((int16_t)buf[10] >> 5 | ((int16_t)buf[11] << 3 )) & 0x07FF;
        SBUS_CH.CH9 = ((int16_t)buf[12] << 0 | ((int16_t)buf[13] << 8 )) & 0x07FF;
        SBUS_CH.CH10 = ((int16_t)buf[13] >> 3 | ((int16_t)buf[14] << 5 )) & 0x07FF;
        SBUS_CH.CH11 = ((int16_t)buf[14] >> 6 | ((int16_t)buf[15] << 2 ) | (int16_t)buf[16] << 10 ) & 0x07FF;
        SBUS_CH.CH12 = ((int16_t)buf[16] >> 1 | ((int16_t)buf[17] << 7 )) & 0x07FF;
        SBUS_CH.CH13 = ((int16_t)buf[17] >> 4 | ((int16_t)buf[18] << 4 )) & 0x07FF;
        SBUS_CH.CH14 = ((int16_t)buf[18] >> 7 | ((int16_t)buf[19] << 1 ) | (int16_t)buf[20] << 9 ) & 0x07FF;
        SBUS_CH.CH15 = ((int16_t)buf[20] >> 2 | ((int16_t)buf[21] << 6 )) & 0x07FF;
        SBUS_CH.CH16 = ((int16_t)buf[21] >> 5 | ((int16_t)buf[22] << 3 )) & 0x07FF;
        return 1;
    }
    else 
    {
        SBUS_CH.ConnectState = 0;
        return 0;
    }
}

//sbus解析之后，调用此函数打印通道数据
void sbus_show_ch(void)
{
	if(SBUS_CH.ConnectState == 0)
	{
		printf("no connectstate!\r\n");
	}
	else
	{
		dma_printf("ch1 value: %d\r\n",SBUS_CH.CH1);
		dma_printf("ch2 value: %d\r\n",SBUS_CH.CH2);
		dma_printf("ch3 value: %d\r\n",SBUS_CH.CH3);
		dma_printf("ch4 value: %d\r\n",SBUS_CH.CH4);

		dma_printf("ch5 value: %d\r\n",SBUS_CH.CH5);
		dma_printf("ch6 value: %d\r\n",SBUS_CH.CH6);
		dma_printf("ch7 value: %d\r\n",SBUS_CH.CH7);
		dma_printf("ch8 value: %d\r\n",SBUS_CH.CH8);

		dma_printf("ch9 value: %d\r\n",SBUS_CH.CH9);
		dma_printf("ch10 value: %d\r\n",SBUS_CH.CH10);
		dma_printf("ch11 value: %d\r\n",SBUS_CH.CH11);
		dma_printf("ch12 value: %d\r\n",SBUS_CH.CH12);

		dma_printf("ch13 value: %d\r\n",SBUS_CH.CH13);
		dma_printf("ch14 value: %d\r\n",SBUS_CH.CH14);
		dma_printf("ch15 value: %d\r\n",SBUS_CH.CH15);
		dma_printf("ch16 value: %d\r\n",SBUS_CH.CH16);
	}
//	for (int i = 0; i<25; i++)
//	{
//		printf("%d ", USART5_SBUS_RX_BUF[i]);
//	}
//	printf("\r\n");
}

/////////////////////////////串口中断//////////////////////////////

//串口3中断(CODBOT 4WD ROS 机器人通信协议校验)
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == USART3)
	{
		//HAL_UART_DMAStop(huart);
		CODBOT_4WD_ROS_Callback();
		my_uart_IT_re_enable();
	}
}

//串口5中断(SBUS协议解析)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//printf(" Hi, guo yao hui ! \r\n");
	while (huart->Instance == UART5) //如果是串口5
	{
		USART5_SBUS_RX_BUF[USART5_RX_STA] = UART5_RX_BUF[0];
		if (USART5_RX_STA == 0 && USART5_SBUS_RX_BUF[USART5_RX_STA] != 0x0F) break; //帧头不对，丢掉
		USART5_RX_STA++;
		if (USART5_RX_STA > USART5_REC_LEN) USART5_RX_STA = 0;  ///接收数据错误,重新开始接收
		if (USART5_SBUS_RX_BUF[0] == 0x0F && USART5_SBUS_RX_BUF[24] == 0x00 && USART5_RX_STA == 25)	//接受完一帧数据
		{
			update_sbus(USART5_SBUS_RX_BUF);
			for (int i = 0; i<25; i++)		//清空缓存区
				USART5_SBUS_RX_BUF[i] = 0;
			USART5_RX_STA = 0;
		}
		break;
	}
	HAL_UART_Receive_IT(&huart5, (u8 *)UART5_RX_BUF, 1);
}

char rtos_printf_buf[RTOS_PRINTF_MAX];
void rtos_printf(char *fmt, ...)
{
	taskENTER_CRITICAL();
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(rtos_printf_buf, RTOS_PRINTF_MAX+1, fmt, arg_ptr);
	printf("%s", rtos_printf_buf);	
	taskEXIT_CRITICAL();
}









