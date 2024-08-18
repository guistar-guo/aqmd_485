/**
  ******************************************************************************
  * File Name          : pushrod.c
  * Description        : 推杆驱动代码
	* @author            : SuperEgo-郭耀辉
  ******************************************************************************
  * @attention
  *
  * 基于motor.c模块中的电机A来驱动推杆的编码电机（pwmA，encoderA）
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

//耗时操作！
void pushrod_reset(void)
{
	pushrod_set_pwm(PUSHROD_FALL_SPEED);
	osDelay(5000);
	pushrod_clear_encoder();
}

//耗时操作！
//参数输入范围：0 到 600
void pushrod_set_postion(short target)
{
	while(pushrod_get_encoder() < target)
	{
		pushrod_set_pwm(PUSHROD_RISE_SPEED);
		osDelay(1);
	}
	pushrod_set_pwm(0);
}



















