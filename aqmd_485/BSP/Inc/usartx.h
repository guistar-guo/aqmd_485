#ifndef __SBUS_H
#define __SBUS_H

#include "main.h"
//#include "usart.h"

#define USART3_REC_LEN 100
#define USART5_REC_LEN 25
#define DMA_PRINTF_MAX 100

typedef struct
{
		uint16_t CH1;//通道1数值
		uint16_t CH2;//通道2数值
		uint16_t CH3;//通道3数值
		uint16_t CH4;//通道4数值
		uint16_t CH5;//通道5数值
		uint16_t CH6;//通道6数值
    uint16_t CH7;//通道7数值
    uint16_t CH8;//通道8数值
    uint16_t CH9;//通道9数值
    uint16_t CH10;//通道10数值
    uint16_t CH11;//通道11数值
    uint16_t CH12;//通道12数值
    uint16_t CH13;//通道13数值
    uint16_t CH14;//通道14数值
    uint16_t CH15;//通道15数值
    uint16_t CH16;//通道16数值
		uint8_t ConnectState;//遥控器与接收器连接状态 0=未连接，1=正常连接
}SBUS_CH_Struct;

typedef struct{
	uint8_t data_size;						//最新数据帧的字节数
	uint8_t uart_receive[200];		//最新数据帧的内容(只包含有效数据)
}ROS_Upper_computer;

extern uint8_t UART3_RX_BUF[1];//供system.c函数访问，来初始化串口中断
extern uint8_t UART4_RX_BUF[1];//供system.c函数访问，来初始化串口中断
extern uint8_t UART5_RX_BUF[1];//供system.c函数访问，来初始化串口中断
extern SBUS_CH_Struct SBUS_CH;//sbus接口，用户可随时访问来查看当前的sbus数据
extern ROS_Upper_computer ros_upper_computer;
void sbus_show_ch(void);//打印sbus所有通道的当前数值
void dma_printf (char *fmt, ...);
void rtos_printf(char *fmt, ...);

#endif




















