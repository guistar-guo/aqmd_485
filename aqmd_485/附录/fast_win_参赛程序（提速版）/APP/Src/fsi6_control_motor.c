/**
  ******************************************************************************
  * File Name          : fsi6_control_motor.c
  * Description        : 富斯i6遥控应用
	* @author            : SuperEgo-郭耀辉
  ******************************************************************************
  * @attention
  *
  * 该文件调用usartx组件的SBUS_CH结构体来获取解析后的sbus的所有通道值，然后通过
	* 区间映射函数将通道值映射到电机结构体的speed成员上，完成遥控控制电机的任务。
  *
  ******************************************************************************
  */
#include "fsi6_control_motor.h"

/**************************************************************************
函数功能：区间映射函数，将区间[cur_left, cur_right]内的某个数x映射到区间
					[mapping_left, mapping_right]的另一个数，并将其返回
入口参数：无
返回  值：映射后的数据
**************************************************************************/
static float Interval_mapping(float x, float cur_left, float cur_right, float mapping_left, float mapping_right)
{
	return mapping_left + (x - cur_left)/(cur_right - cur_left) * (mapping_right - mapping_left);
}

void fsi6_control_motorCD(void)
{
	short forward = Interval_mapping(SBUS_CH.CH2, 224, 1824, -16799, 16799);
	short zhuan_wan = Interval_mapping(SBUS_CH.CH1, 224, 1824, -16799, 16799);
	short left_speed = forward + zhuan_wan;
	if(left_speed > 16799)
	{
		left_speed = 16799;
	}
	else if(left_speed < -16799)
	{
		left_speed = -16799;
	}
	
	short right_speed = forward - zhuan_wan;
	if(right_speed > 16799)
	{
		right_speed = 16799;
	}
	else if(right_speed < -16799)
	{
		right_speed = -16799;
	}
	
	trace_set_motor(-left_speed, -right_speed);
}





















