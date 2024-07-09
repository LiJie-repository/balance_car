#include "stm32f10x.h"                  // Device header
/*
	初始化电机
	PB12,PB13,PB14,PB15
*/
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_13 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}
/*
 	电机转速限幅
	参数:MotorA_PWM(当前电机A的PWM),MotorB_PWM(当前电机B的PWM)
*/
int PWM_Max = 7200;
int PWM_Min = -7200;
void Motor_Limit(int *MotorA_PWM,int *MotorB_PWM)
{
	if(*MotorA_PWM > PWM_Max)*MotorA_PWM = PWM_Max;
	if(*MotorB_PWM > PWM_Max)*MotorB_PWM = PWM_Max;
	
	if(*MotorA_PWM < PWM_Min)*MotorA_PWM = PWM_Min;
	if(*MotorB_PWM < PWM_Min)*MotorB_PWM = PWM_Min;
}
/*
	绝对值函数
	参数：Value(需要转换的参数)
*/
int GetAbsValue(int Value)
{
	int AbsValue;
	AbsValue = Value > 0 ? Value : (- Value);
	return AbsValue;
}
/*
	设置电机PWM正反转
	参数:PWM_MotorA,WM_MotorB
	MototA接入PB12,PB13
	MototB接入PB14,PB15
*/
void Motor_SetPWM(int PWM_MotorA,int PWM_MotorB)
{
	if(PWM_MotorA < 0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	}else
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
	TIM_SetCompare3(TIM2,GetAbsValue(PWM_MotorA));
	
		if(PWM_MotorB < 0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_13);
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	}else
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	}
	TIM_SetCompare2(TIM2,GetAbsValue(PWM_MotorB));
}

void Motor_Stop(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		
	TIM_SetCompare3(TIM2,0);
	TIM_SetCompare2(TIM2,0);
}
