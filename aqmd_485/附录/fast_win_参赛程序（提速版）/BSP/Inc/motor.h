#ifndef __MOTOR__H
#define __MOTOR__H

#include "main.h"

//以下四个宏定义用来设置4个电机的极性，只能给-1或1(设置错误，会导致pid闭环错误)
#define MOTORA_POLARITY 1
#define MOTORB_POLARITY 1	
#define MOTORC_POLARITY -1

//编码器D烧坏了，电机D暂时采用编码器B
#define MOTORD_POLARITY -1

//设置电机示数的极性(就算设置错误，也不会导致pid闭环错误)


#define SMOOTH_STEP 5.0f

typedef void (*motor_set_pwm)(int16_t pwm);
typedef void (*Clear_encoder)(void);
typedef int (*Get_encoder)(void);

typedef struct{
	int16_t								speed;					//提供给用户的闭环速度设置接口
	motor_set_pwm 				set_pwm;				//函数指针，设置电机的pwm控制电机正反转(参数范围：-16799到16799)
	float									smooth_speed;		//平滑函数处理的变量，它与target_speed仅仅相差一个int16_t类型转换
	int16_t								target_speed;		//电机PI闭环目标速度
	Get_encoder 					get_encoder;		//函数指针，返回编码器当前计数值
	Clear_encoder 				clear_encoder;	//函数指针，清零编码器当前计数值
}Motor;

extern Motor motorA;
extern Motor motorB;
extern Motor motorC;
extern Motor motorD;

void motor_init(void);

//二轮差速速度闭环用户接口
void motor_pi_control(short idD, short idC);	//闭环控制电机速度
void motor_getCD_speed(short* current_speed);	//返回电机的当前速度(编码器5ms速度)
void motor_task(void);	//速度闭环系统核心任务

//二轮差速PWM开环控制用户接口
void set_motorDC_pwm(int16_t pwmD, int16_t pwmC);
void motot_show_DC_encoder(void);

#endif


















