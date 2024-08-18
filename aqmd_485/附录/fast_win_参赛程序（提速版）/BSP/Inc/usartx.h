#ifndef __SBUS_H
#define __SBUS_H

#include "main.h"
//#include "usart.h"

#define USART3_REC_LEN 100		//Define the maximum number of bytes that can be accepted
#define USART5_REC_LEN 25
#define DMA_PRINTF_MAX 100
#define RTOS_PRINTF_MAX 100

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


//my_uart_check_correct函数校验检查
//如果校验正确，则返回CORRECT(0)，如果报头校验错误，则返回START_ERROR(1)
//如果异或校验错误，则返回XOR_ERROR(2)
typedef enum MY_UART_ERROR_CHECK
{
      CORRECT=0, START_ERROR, XOR_ERROR
}MY_UART_ERROR_CHECK;

extern uint8_t UART3_RX_BUF[USART3_REC_LEN];
extern uint8_t UART5_RX_BUF[1];
extern SBUS_CH_Struct SBUS_CH;
void my_uart_IT_re_enable(void);
void sbus_show_ch(void);
void dma_printf (char *fmt, ...);
void rtos_printf(char *fmt, ...);


#endif
