#ifndef __ENCODER_H
#define __ENCODER_H 
#include "stm32f10x.h"                  // Device header

void Encoder_TIM3_Init(void);
void Encoder_TIM4_Init(void);
int Encoder_GetSpeed(uint8_t Encoder_TIMx);

#endif
