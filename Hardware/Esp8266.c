#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

/*
	初始化串口
	TX:PA2
	RX:PA3
*/
void Esp8266_Init(void)
{
		
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//开启UART2时钟
	
	GPIO_InitTypeDef GPIO_InitStruct;                     
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;        
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;     //TX: PA2
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;       
	GPIO_Init(GPIOA,&GPIO_InitStruct);     
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;        
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;		//RX:PA3
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;       
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //TX,RX模式
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;          //停止位一位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);				//开启串口中断
	
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);								//初始化NVIC
	
	USART_Cmd(USART1,ENABLE);									//开启UART2
}

