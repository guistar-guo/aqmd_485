#ifndef __MOTOR__H
#define __MOTOR__H

#include "main.h"

//�����ĸ��궨����������4������ļ��ԣ�ֻ�ܸ�-1��1(���ô��󣬻ᵼ��pid�ջ�����)
#define MOTORA_POLARITY 1
#define MOTORB_POLARITY 1	
#define MOTORC_POLARITY -1

//������D�ջ��ˣ����D��ʱ���ñ�����B
#define MOTORD_POLARITY -1

//���õ��ʾ���ļ���(�������ô���Ҳ���ᵼ��pid�ջ�����)


#define SMOOTH_STEP 5.0f

typedef void (*motor_set_pwm)(int16_t pwm);
typedef void (*Clear_encoder)(void);
typedef int (*Get_encoder)(void);

typedef struct{
	int16_t								speed;					//�ṩ���û��ıջ��ٶ����ýӿ�
	motor_set_pwm 				set_pwm;				//����ָ�룬���õ����pwm���Ƶ������ת(������Χ��-16799��16799)
	float									smooth_speed;		//ƽ����������ı���������target_speed�������һ��int16_t����ת��
	int16_t								target_speed;		//���PI�ջ�Ŀ���ٶ�
	Get_encoder 					get_encoder;		//����ָ�룬���ر�������ǰ����ֵ
	Clear_encoder 				clear_encoder;	//����ָ�룬�����������ǰ����ֵ
}Motor;

extern Motor motorA;
extern Motor motorB;
extern Motor motorC;
extern Motor motorD;

void motor_init(void);

//���ֲ����ٶȱջ��û��ӿ�
void motor_pi_control(short idD, short idC);	//�ջ����Ƶ���ٶ�
void motor_getCD_speed(short* current_speed);	//���ص���ĵ�ǰ�ٶ�(������5ms�ٶ�)
void motor_task(void);	//�ٶȱջ�ϵͳ��������

//���ֲ���PWM���������û��ӿ�
void set_motorDC_pwm(int16_t pwmD, int16_t pwmC);
void motot_show_DC_encoder(void);

#endif


















