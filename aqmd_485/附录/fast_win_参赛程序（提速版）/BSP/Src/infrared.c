/**
  ******************************************************************************
  * File Name          : infrared.c
  * Description        : ���⴫������������
	* @author            : SuperEgo-��ҫ��
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#include "infrared.h"
#include <stdlib.h>
#include <stdio.h>

/**************************************************************************
�������ܣ���ȡ���⴫������ʾ�����Զ�������ʽ���أ�����"01100"
��ڲ�������
����  ֵ����ȡ������·��������
**************************************************************************/
u8 infrared_get_data(void)
{
	return ~((HAL_GPIO_ReadPin(L2_GPIO_Port, L2_Pin) << 4| HAL_GPIO_ReadPin(L1_GPIO_Port, L1_Pin) << 3\
				|HAL_GPIO_ReadPin(M_GPIO_Port, M_Pin) << 2|HAL_GPIO_ReadPin(R1_GPIO_Port, R1_Pin) << 1\
				|HAL_GPIO_ReadPin(R2_GPIO_Port, R2_Pin)) & 0x1F);
}

/**************************************************************************
�������ܣ������⴫��������·������ʾ��oled��Ļ��
��ڲ�������
����  ֵ����
**************************************************************************/
void infrared_oled_show(void)
{
	u8 infrared_data = infrared_get_data();
	char s[6] = "00000";
	s[5] = '\0';
	for (int i = 0; i<5; i++)
	{
		if((1<<(4-i)) & infrared_data){
			s[i] = '1';
		}
	}
	
//	dma_printf("%s\r\n",(char*)s);
	
	OLED_ShowString(40, 15, (u8*)s);
	OLED_Refresh_Gram();
}













