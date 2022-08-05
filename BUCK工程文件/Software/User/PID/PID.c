#include "PID.h"


float PID[3];  //������������P��I��D�ĳ�ʼֵ
float ref;
PID_TypeDef pid;
int CCR_Value=0, CCR_Next_Value=0;

uint32_t ADC_Value_Sum=0;
uint32_t ADC_Value_Average=0;
uint8_t i=0;

uint16_t Set_Value=120;
uint8_t temp[3]={0};
uint8_t j=0;

void PID_Init(PID_TypeDef *pid, uint8_t mode, 
	            const float PID[3], float max_out, float max_iout)
{
    if (pid == NULL || PID == NULL)
    {
        return;
    }
    pid->mode = mode;  //PID����ģʽ��0.λ��ʽ 1.����ʽ
    pid->Kp = PID[0];  //PID[]���飬���ڴ�������������� P, I, D �ĳ�ʼֵ
    pid->Ki = PID[1];  
    pid->Kd = PID[2];
    pid->max_out = max_out;
    pid->max_iout = max_iout;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
}

float PID_Calc(PID_TypeDef *pid, float ref, float set)
{
    //�жϴ����PIDָ�벻Ϊ��
    if (pid == NULL)
        return 0.0f;
    
    //��Ź�ȥ���μ�������ֵ
    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
		
    //�趨Ŀ��ֵ�͵�ǰֵ���ṹ���Ա
    pid->set = set;
    pid->fdb = ref;
		
    //�������µ����ֵ
    pid->error[0] = set - ref;
		
    //�ж�PID���õ�ģʽ
    if (pid->mode == PID_POSITION)
    {
        //λ��ʽPID
        //������������
        pid->Pout = pid->Kp * pid->error[0];
        //������������
        pid->Iout += pid->Ki * pid->error[0];
        //��Ź�ȥ���μ����΢�����ֵ
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        //��ǰ����΢���ñ�������ȥ��һ�����������
        pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
        //΢�������
        pid->Dout = pid->Kd * pid->Dbuf[0];
        //�Ի���������޷�
        LimitMax(pid->Iout, pid->max_iout);
        //������������������
        pid->out = pid->Pout + pid->Iout + pid->Dout;
        //������������޷�
        LimitMax(pid->out, pid->max_out);
    }
		
    else if (pid->mode == PID_DELTA)
    {
        //����ʽPID
        //�Ա���������ϴ����Ĳ�ֵ��Ϊ�����������������
        pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
        //�Ա��������Ϊ������������
        pid->Iout = pid->Ki * pid->error[0];
        //����΢���������
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        //�Ա���������ϴ����Ĳ�ֵ��ȥ�ϴ���������ϴ����Ĳ�ֵ��Ϊ΢���������������
        pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
        pid->Dout = pid->Kd * pid->Dbuf[0];
        //����������������Ϊ�����
        pid->out += pid->Pout + pid->Iout + pid->Dout;
        //���������һ�����޷�
        LimitMax(pid->out, pid->max_out);
	}
	return pid->out;
}

void PID_DeInit(PID_TypeDef *pid)
{
    if (pid == NULL)
        return;
		
	//��ǰ�������
    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    //΢��������
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    //�������
    pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
    //Ŀ��ֵ�͵�ǰֵ����
    pid->fdb = pid->set = 0.0f;
}


void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//��ʱ�� TIM4 ��ʼ��
	TIM_TimeBaseStructure.TIM_Period = (72-1); 
	TIM_TimeBaseStructure.TIM_Prescaler =(1000-1); 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 
	//�ж����ȼ� NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	TIM_Cmd(TIM4, ENABLE);
}


void TIM4_IRQHandler() //TIM4�жϺ���
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	 	
		if(i==16)		//Last time:i = 16
		{
			
			CCR_Value=(int)PID_Calc(&pid,0.125*(float)(ADC_Value_Sum/16)/1000,((float)Set_Value)/50.22);
			
			//printf("%f\n",0.1875*(float)ADS_SEL_Read(4,0,0)/1000);
			
			CCR_Next_Value = CCR_Value + CCR1_Val;
			
			
			if(CCR_Next_Value>=450&&CCR_Next_Value<=1350)
				CCR1_Val=CCR_Next_Value;
			else if(CCR_Next_Value<450)
				CCR1_Val=450;
			else
				CCR1_Val=1350;
			
			TIM_SetCompare1(TIM3,CCR1_Val);
			
			i=0;
			ADC_Value_Sum=0;
		}
		ADC_Value_Sum+=ADS_SEL_Read(4,1,0);
		i++;
	}
}



















