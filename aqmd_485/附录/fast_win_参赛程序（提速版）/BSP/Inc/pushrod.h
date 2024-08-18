#ifndef __PUSHROD__H
#define __PUSHROD__H

#include "main.h"
#define pushrod_set_pwm(pwm) 				motorA.set_pwm(pwm)
#define pushrod_get_encoder() 			-motorA.get_encoder()
#define pushrod_clear_encoder()			motorA.clear_encoder()

#define PUSHROD_RISE_SPEED 8000
#define PUSHROD_FALL_SPEED -8000

void pushrod_encoder_test(void);

void pushrod_reset(void);//ºÄÊ±²Ù×÷£¡
void pushrod_set_postion(short target);

#endif







