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
		uint16_t CH1;//ͨ��1��ֵ
		uint16_t CH2;//ͨ��2��ֵ
		uint16_t CH3;//ͨ��3��ֵ
		uint16_t CH4;//ͨ��4��ֵ
		uint16_t CH5;//ͨ��5��ֵ
		uint16_t CH6;//ͨ��6��ֵ
    uint16_t CH7;//ͨ��7��ֵ
    uint16_t CH8;//ͨ��8��ֵ
    uint16_t CH9;//ͨ��9��ֵ
    uint16_t CH10;//ͨ��10��ֵ
    uint16_t CH11;//ͨ��11��ֵ
    uint16_t CH12;//ͨ��12��ֵ
    uint16_t CH13;//ͨ��13��ֵ
    uint16_t CH14;//ͨ��14��ֵ
    uint16_t CH15;//ͨ��15��ֵ
    uint16_t CH16;//ͨ��16��ֵ
		uint8_t ConnectState;//ң���������������״̬ 0=δ���ӣ�1=��������
}SBUS_CH_Struct;


//my_uart_check_correct����У����
//���У����ȷ���򷵻�CORRECT(0)�������ͷУ������򷵻�START_ERROR(1)
//������У������򷵻�XOR_ERROR(2)
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
