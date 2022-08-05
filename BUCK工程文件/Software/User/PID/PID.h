#ifndef __PID_H
#define	__PID_H

#include "stm32f10x.h"
#include "stdio.h"
#include "ads1118.h"
#include "bsp_GeneralTim.h"

typedef struct
{
    //PID运算模式
    uint8_t mode;
    //PID 三个基本参数
    float Kp;
    float Ki;
    float Kd;

    float max_out;  //PID最大输出
    float max_iout; //PID最大积分输出

    float set;	  //PID目标值
    float fdb;	  //PID当前值

    float out;		//三项叠加输出
    float Pout;		//比例项输出
    float Iout;		//积分项输出
    float Dout;		//微分项输出
	
    //微分项最近三个值 0最新 1上一次 2上上次
    float Dbuf[3];  
	
    //误差项最近三个值 0最新 1上一次 2上上次
    float error[3];  

} PID_TypeDef;

enum PID_MODE  //模式选择：0.位置式 1.增量式
{
    PID_POSITION = 0,
    PID_DELTA
};


extern  float PID[3];  //三个基本参数P、I、D的初始值
extern float ref;
extern PID_TypeDef pid;
extern int CCR_Value, CCR_Next_Value;

extern uint32_t ADC_Value_Sum;
extern uint32_t ADC_Value_Average;
extern uint8_t i;

extern uint16_t Set_Value;
extern uint8_t temp[3];
extern uint8_t j;


#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }

		
void PID_Init(PID_TypeDef *pid, uint8_t mode, 
							const float PID[3], float max_out, float max_iout);
float PID_Calc(PID_TypeDef *pid, float ref, float set);
void PID_DeInit(PID_TypeDef *pid);
void TIM4_Init(void);

		
#endif
