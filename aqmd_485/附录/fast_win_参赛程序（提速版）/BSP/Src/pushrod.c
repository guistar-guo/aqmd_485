/**
  ******************************************************************************
  * File Name          : pushrod.c
  * Description        : �Ƹ���������
	* @author            : SuperEgo-��ҫ��
  ******************************************************************************
  * @attention
  *
  * ����motor.cģ���еĵ��A�������Ƹ˵ı�������pwmA��encoderA��
  *
  ******************************************************************************
  */

#include "pushrod.h"

char str[100];
void pushrod_encoder_test(void)
{
		sprintf(str, "%4d", pushrod_get_encoder());
		OLED_ShowString(47, 30, (u8*)str);
		OLED_Refresh_Gram();
		osDelay(20);
}

//��ʱ������
void pushrod_reset(void)
{
	pushrod_set_pwm(PUSHROD_FALL_SPEED);
	osDelay(5000);
	pushrod_clear_encoder();
}

//��ʱ������
//�������뷶Χ��0 �� 600
void pushrod_set_postion(short target)
{
	while(pushrod_get_encoder() < target)
	{
		pushrod_set_pwm(PUSHROD_RISE_SPEED);
		osDelay(1);
	}
	pushrod_set_pwm(0);
}



















