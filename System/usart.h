#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f10x.h"                  // Device header

void uart1_init(u32 bound);					//����1��ʼ������
void USART1_IRQHandler(void);     	//����1�жϷ������
#endif


