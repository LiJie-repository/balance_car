#include "stm32f10x.h"                  // Device header

/*
	初始化TIM2编码器
	PA1&PA2
*/
void Encoder_TIM3_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启时钟
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period =65535 - 1 ;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct); //定时器初始化
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_ICFilter = 10;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);  //初始化输入捕获
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); //编码器模式设置
	TIM_SetCounter(TIM3,0); //清空计数值
	TIM_Cmd(TIM3,ENABLE);	//开启定时器
}
/*
	初始化TIM4编码器
	PB6&PB7
*/
void Encoder_TIM4_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period =65535 - 1 ;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_ICFilter = 10;
	TIM_ICInit(TIM4,&TIM_ICInitStruct);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4,ENABLE);
}
/*
	读取编码器速度函数
	参数：Encoder_TIMx 这里设置为TIM2和TIM4，所以参数可选2，4
	返回值：定时器Counter的数值，即编码器的速度
*/
int Encoder_GetSpeed(uint8_t Encoder_TIMx)
{
	int Encoder_Speed;  
	
	switch (Encoder_TIMx)
	{
		case 3 : Encoder_Speed = (short)TIM_GetCounter(TIM3);TIM_SetCounter(TIM3,0); break;
		case 4 : Encoder_Speed = (short)TIM_GetCounter(TIM4);TIM_SetCounter(TIM4,0); break;
	   default : Encoder_Speed = 0;
	}
	return Encoder_Speed;
}
