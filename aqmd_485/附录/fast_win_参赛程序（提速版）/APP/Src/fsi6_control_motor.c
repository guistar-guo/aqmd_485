/**
  ******************************************************************************
  * File Name          : fsi6_control_motor.c
  * Description        : ��˹i6ң��Ӧ��
	* @author            : SuperEgo-��ҫ��
  ******************************************************************************
  * @attention
  *
  * ���ļ�����usartx�����SBUS_CH�ṹ������ȡ�������sbus������ͨ��ֵ��Ȼ��ͨ��
	* ����ӳ�亯����ͨ��ֵӳ�䵽����ṹ���speed��Ա�ϣ����ң�ؿ��Ƶ��������
  *
  ******************************************************************************
  */
#include "fsi6_control_motor.h"

/**************************************************************************
�������ܣ�����ӳ�亯����������[cur_left, cur_right]�ڵ�ĳ����xӳ�䵽����
					[mapping_left, mapping_right]����һ�����������䷵��
��ڲ�������
����  ֵ��ӳ��������
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





















