#ifndef __SBUS_H
#define __SBUS_H

#include "main.h"
//#include "usart.h"

#define USART3_REC_LEN 100
#define USART5_REC_LEN 25
#define DMA_PRINTF_MAX 100

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

typedef struct{
	uint8_t data_size;						//��������֡���ֽ���
	uint8_t uart_receive[200];		//��������֡������(ֻ������Ч����)
}ROS_Upper_computer;

extern uint8_t UART3_RX_BUF[1];//��system.c�������ʣ�����ʼ�������ж�
extern uint8_t UART4_RX_BUF[1];//��system.c�������ʣ�����ʼ�������ж�
extern uint8_t UART5_RX_BUF[1];//��system.c�������ʣ�����ʼ�������ж�
extern SBUS_CH_Struct SBUS_CH;//sbus�ӿڣ��û�����ʱ�������鿴��ǰ��sbus����
extern ROS_Upper_computer ros_upper_computer;
void sbus_show_ch(void);//��ӡsbus����ͨ���ĵ�ǰ��ֵ
void dma_printf (char *fmt, ...);
void rtos_printf(char *fmt, ...);

#endif




















