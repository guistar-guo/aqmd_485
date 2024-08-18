#ifndef __TRACE_H
#define __TRACE_H

#include "main.h"

#define trace_set_motor(left, right) 	set_motorDC_pwm((left), -(right))	//设置电机pwm，参数范围：-16799到16799

void app_kernel(void);
void trace_turn_right(void);
void trace_turn_left(void);
void trace_turn_180(void);
void trace_turn_left_45(void);
void trace_turn_right_45(void);
void trace_back_control(u8 cur);

void tace_pushrod(void);

#endif















