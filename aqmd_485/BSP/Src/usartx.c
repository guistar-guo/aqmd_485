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

uint8_t UART3_RX_BUF[1];//串口3接收缓冲区
uint8_t UART4_RX_BUF[1];//串口3接收缓冲区
uint8_t UART5_RX_BUF[1];//串口5接收缓冲区

uint8_t USART3_ROBOT_RX_BUF[USART3_REC_LEN];//接收ros数据缓冲
uint8_t USART3_RX_STA;//指示当前USART3_ROBOT_RX_BUF接收了多少个字节

uint8_t USART5_RX_STA;//指示当前USART5_SBUS_RX_BUF接收了多少个字节
uint8_t USART5_SBUS_RX_BUF[USART5_REC_LEN];//接收sbus数据缓冲

////////////////////////////////////发送函数相关///////////////////////////////////
//printf支持（串口重定向）
//注意此步骤要想生效，必须配置工程target选项支持Use MicroLIB
int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart3,(uint8_t *)&ch,1,HAL_MAX_DELAY);
	return ch;
}

//dma_printf支持
char dma_printf_buf[DMA_PRINTF_MAX];
void dma_printf (char *fmt, ...)
{ 
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(dma_printf_buf, DMA_PRINTF_MAX+1, fmt, arg_ptr);
	HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&dma_printf_buf, strlen(dma_printf_buf)); 
}

//rtos_printf支持（不可被其他任务打断，强制打印完整内容）
#define RTOS_PRINTF_MAX 100
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

//////////////////接收处理相关（当接收到完整数据帧了，在以下几个函数中处理数据帧）////////////////
//代码参考文章见附录：sbus协议csdn代码参考

SBUS_CH_Struct SBUS_CH;

//sbus数据帧处理
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
}

ROS_Upper_computer ros_upper_computer;//定义ros上位机

//ros通信：接收处理
//协议格式参考：附录 -> ros上位机协议.txt
//处理串口3发过来的数据（ros上位机），当成功接收到一帧数据，则在串口3中断的相应位置将调用此函数
//参数1是刚刚接收到的一帧数据头指针，参数2是这一帧数据的总长度(包括帧头字节，长度指示字节，校验字节)
static void ros_dispose_data(uint8_t *data, uint8_t size){
	memcpy(ros_upper_computer.uart_receive, data+2, size-3);
	ros_upper_computer.data_size = size-3;
}

////////////////////////////////////////////串口中断/////////////////////////////////////////////////

uint16_t ROBOT_UART_LEN;
uint8_t ROBOT_XOR_CHECK;

//串口中断(SBUS协议解析)
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
	
	////////////////////////////////////////////////////////////////
	while (huart->Instance == USART3){ //如果是串口3
		dma_printf("I love IU! \r\n");
		USART3_ROBOT_RX_BUF[USART3_RX_STA] = UART3_RX_BUF[0];
		if (USART3_RX_STA == 0 && USART3_ROBOT_RX_BUF[USART3_RX_STA] != 0xFD) break; //帧头不对，丢掉
		USART3_RX_STA++;
		if(USART3_RX_STA == 2){
			ROBOT_UART_LEN = USART3_ROBOT_RX_BUF[USART3_RX_STA-1];//第2个字节是数据长度
			break;
		}
		if (USART3_RX_STA == ROBOT_UART_LEN+3){	//接受完一帧数据
			ROBOT_XOR_CHECK = 0;
			for (int i = 2; i <= ROBOT_UART_LEN+1; i++){//异或校验
				ROBOT_XOR_CHECK ^= USART3_ROBOT_RX_BUF[i];
			}
			if(ROBOT_XOR_CHECK == USART3_ROBOT_RX_BUF[ROBOT_UART_LEN+2]){//成功接收到ros上位机发过来的一帧数据
				ros_dispose_data(USART3_ROBOT_RX_BUF, ROBOT_UART_LEN+3);//处理数据
			}
			for (int i = 0; i<USART3_RX_STA; i++)		//清空缓存区
				USART3_ROBOT_RX_BUF[i] = 0;
			USART3_RX_STA = 0;
			break;
		}
		break;
	}
	HAL_UART_Receive_IT(&huart3, (u8 *)UART3_RX_BUF, 1);
	
	if(huart->Instance == UART4){ //如果是串口4
		aqmd_receive_one_byte_callback(UART4_RX_BUF[0]);
	}
	HAL_UART_Receive_IT(&huart4, (u8 *)UART4_RX_BUF, 1);
}








