#ifndef __PID_H
#define	__PID_H

#include "stm32f10x.h"
#include "stdio.h"
#include "ads1118.h"
#include "bsp_GeneralTim.h"

typedef struct
{
    //PID����ģʽ
    uint8_t mode;
    //PID ������������
    float Kp;
    float Ki;
    float Kd;

    float max_out;  //PID������
    float max_iout; //PID���������

    float set;	  //PIDĿ��ֵ
    float fdb;	  //PID��ǰֵ

    float out;		//����������
    float Pout;		//���������
    float Iout;		//���������
    float Dout;		//΢�������
	
    //΢�����������ֵ 0���� 1��һ�� 2���ϴ�
    float Dbuf[3];  
	
    //������������ֵ 0���� 1��һ�� 2���ϴ�
    float error[3];  

} PID_TypeDef;

enum PID_MODE  //ģʽѡ��0.λ��ʽ 1.����ʽ
{
    PID_POSITION = 0,
    PID_DELTA
};


extern  float PID[3];  //������������P��I��D�ĳ�ʼֵ
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
