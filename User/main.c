#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "oled.h"
#include "MPU6050Exti.h"
#include "PWM.h"
#include "Encoder.h"
#include "Motor.h"
#include "PID_Control.h"
#include "Esp8266.h"

float TargetSpeed = 0;
float TurnSpeed = 0;

float Med_Angle = -0.20 ;//-1

float Vertical_Kp = 800,Vertical_Kd = 0.00 ;   //kp = 410 kd = -0.9 x0.6 : kp = 246 kd = 0.85
float Velocity_Kp = 0,Velocity_Ki =0;//Velocity_Kp = 0.65,Velocity_Ki =0.65 /200;
float Turn_Kp = 0, Turn_Kd = 0;//60  0.6


float Pitch,Roll,Yaw;	
short gyrox,gyroy,gyroz;
short aacx,aacy,aacz; 

int Encoder_Left,Encoder_Right;
int Velocity_Out,Vertical_Out,Turn_Out;
int MotorA_PWM,MotorB_PWM;

char StopFlag = 0;
uint8_t Advance,Back,Right,Left;

float TargetSpeed_Max = 25;
float TargetSpeed_Min = -15;

float TurnSpeed_Max = 10;//20
float TurnSpeed_Min = -10;//20

//int PWM_Max = 7200 , PWM_Min = -7200;

int main(void)
{
	delay_init(); //系统时钟开启
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组
	OLED_Init();
	Motor_Init();
	MPU_Init();
	mpu_dmp_init();
	MPU6050_Exti_Init();
	Encoder_TIM3_Init();
	Encoder_TIM4_Init();
	Esp8266_Init();
 	PWM_TIM2_Init(0,(7200 - 1));	//100HZ 
	while (1)
	{
		OLED_ShowSignedNum(1,1,Encoder_GetSpeed(3),6,2);
		OLED_ShowSignedNum(2,1,Encoder_GetSpeed(4),6,2);
		
//		OLED_ShowSignedNum(1,1,Pitch,6,2);
//		OLED_ShowSignedNum(2,1,gyroy,6,2);
//		OLED_ShowSignedNum(3,1,Yaw,6,2);
		
//		OLED_ShowNum(4,1,Advance,1);
//		OLED_ShowNum(4,3,Back,1);
//		OLED_ShowNum(4,5,Right,1);
//		OLED_ShowNum(4,7,Left,1);
	}
}

void EXTI4_IRQHandler(void)
{
	int PWM_Out;
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{	
		EXTI_ClearITPendingBit(EXTI_Line4);		//清除中断标志位
		Encoder_Left = - Encoder_GetSpeed(3);	//电机相对安装,需要有一个取反
		Encoder_Right = Encoder_GetSpeed(4);    //获取编码器值,小车的速度
			
		mpu_dmp_get_data(&Pitch,&Roll,&Yaw);	//欧拉角
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);//角速度(陀螺仪)
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);//加速度
		
		/*前进后退控制*/
		if(Advance == 0&& Back == 0) TargetSpeed = 0;
		if(Advance == 1)TargetSpeed ++;
		if(Back == 1)TargetSpeed  --;
		
		if(TargetSpeed > TargetSpeed_Max)
		{
			TargetSpeed = TargetSpeed_Max;
		}else if(TargetSpeed < TargetSpeed_Min)
		{
			TargetSpeed = TargetSpeed_Min;
		}   									//速度限幅
		
		/*转向控制*/
		if(Right == 0 && Left == 0)TurnSpeed = 0;
		if(Right == 1) TurnSpeed ++;
		if(Left == 1) TurnSpeed --;
		
		if(TurnSpeed > TurnSpeed_Max)
		{
			TurnSpeed = TurnSpeed_Max;
		}else if(TurnSpeed < TurnSpeed_Min)
		{
			TurnSpeed = TurnSpeed_Min;
		}   									//转向限幅
		
		if(Right == 0 && Left == 0 ) Turn_Kd = 0.6;
		else if(Right == 1 || Left == 1)Turn_Kd = 0;
		
//		Velocity_Out = Velocity_Control(Encoder_Left,Encoder_Right,TargetSpeed);	//速度环控制
		Vertical_Out = Vertical_Control(Med_Angle + Velocity_Out,Pitch,gyroy);		//直立环控制
		//Turn_Out =  Turn_Control(gyroz,TurnSpeed);									//转向环控制
		
		PWM_Out = Vertical_Out;					//PID输出
		MotorA_PWM = PWM_Out + Turn_Out;
		MotorB_PWM = PWM_Out - Turn_Out;		//加上转向环控制
		
		Motor_Limit(&MotorA_PWM,&MotorB_PWM);   //电机PWM限幅
		if(GetAbsValue(Pitch - Med_Angle) > 30) //小车倒下紧急停止电机
		{
			Motor_Stop();
			StopFlag = 1;
		}
		else
		{
			StopFlag = 0;
			Motor_SetPWM(MotorA_PWM,MotorB_PWM);
		}
		//输出PWM驱动电机
	}
}

void USART1_IRQHandler(void)
{
	uint16_t Serial_RxData;
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
	{
		Serial_RxData = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);
		
		switch (Serial_RxData)
		{
			case 0x00 :Advance = 0,Back = 0,Right = 0,Left = 0;break;
			case 0x41 :Advance = 1,Back = 0,Right = 0,Left = 0;break;
			case 0x42 :Advance = 0,Back = 1,Right = 0,Left = 0;break;
			case 0x43 :Advance = 0,Back = 0,Right = 1,Left = 0;break;
			case 0x44 :Advance = 0,Back = 0,Right = 0,Left = 1;break;
			default :Advance = 0,Back = 0,Right = 0,Left = 0;break;
		}
	}
}

