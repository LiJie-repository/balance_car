#ifndef __MOTOR_H
#define __MOTOR_H 
#include "stm32f10x.h"                  // Device header

void Motor_Init(void);
void Motor_Limit(int *MotorA_PWM,int *MotorB_PWM);
void Motor_SetPWM(int PWM_MotorA,int PWM_MotorB);
void Motor_Stop(void);

int GetAbsValue(int Value);

#endif
