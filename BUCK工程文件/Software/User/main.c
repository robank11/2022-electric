#include "stm32f10x.h"
#include "bsp_usart.h"
#include "ads1118.h"
#include "delay.h"
#include "OLED.h"
#include "codetab.h"
#include "PID.h"
#include "bsp_GeneralTim.h"

int main(void)
{	
	delay_init(72);
	USART_Config();
	GENERAL_TIM_Init();
	//OLED_Init();
	ADS1118_Init();
	
	PID[0]=80;	//…œ“ª¥Œ£∫80°¢20°¢10
	PID[1]=20;
	PID[2]=10;
	TIM4_Init();
	PID_Init(&pid,PID_POSITION,PID,100,10);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	while (1)
	{
		
	}
}

