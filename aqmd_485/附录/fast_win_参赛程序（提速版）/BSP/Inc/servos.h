#ifndef __SERVOS_H
#define __SERVOS_H

#include "main.h"
#define INIT_ANGLE 70

void servos_init(void);
void servos1_set_angle(float angle);
void servos2_set_angle(float angle);
void servos3_set_angle(float angle);
void servos4_set_angle(float angle);
void servos5_set_angle(float angle);

void servos_set_angle(float angle);

#endif
