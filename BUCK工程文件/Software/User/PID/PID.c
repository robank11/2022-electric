#include "PID.h"


float PID[3];  //三个基本参数P、I、D的初始值
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
    pid->mode = mode;  //PID运行模式：0.位置式 1.增量式
    pid->Kp = PID[0];  //PID[]数组，用于存放三个基本参数 P, I, D 的初始值
    pid->Ki = PID[1];  
    pid->Kd = PID[2];
    pid->max_out = max_out;
    pid->max_iout = max_iout;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
}

float PID_Calc(PID_TypeDef *pid, float ref, float set)
{
    //判断传入的PID指针不为空
    if (pid == NULL)
        return 0.0f;
    
    //存放过去两次计算的误差值
    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
		
    //设定目标值和当前值到结构体成员
    pid->set = set;
    pid->fdb = ref;
		
    //计算最新的误差值
    pid->error[0] = set - ref;
		
    //判断PID设置的模式
    if (pid->mode == PID_POSITION)
    {
        //位置式PID
        //比例项计算输出
        pid->Pout = pid->Kp * pid->error[0];
        //积分项计算输出
        pid->Iout += pid->Ki * pid->error[0];
        //存放过去两次计算的微分误差值
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        //当前误差的微分用本次误差减去上一次误差来计算
        pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
        //微分项输出
        pid->Dout = pid->Kd * pid->Dbuf[0];
        //对积分项进行限幅
        LimitMax(pid->Iout, pid->max_iout);
        //叠加三个输出到总输出
        pid->out = pid->Pout + pid->Iout + pid->Dout;
        //对总输出进行限幅
        LimitMax(pid->out, pid->max_out);
    }
		
    else if (pid->mode == PID_DELTA)
    {
        //增量式PID
        //以本次误差与上次误差的差值作为比例项的输入带入计算
        pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
        //以本次误差作为积分项带入计算
        pid->Iout = pid->Ki * pid->error[0];
        //迭代微分项的数组
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        //以本次误差与上次误差的差值减去上次误差与上上次误差的差值作为微分项的输入带入计算
        pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
        pid->Dout = pid->Kd * pid->Dbuf[0];
        //叠加三个项的输出作为总输出
        pid->out += pid->Pout + pid->Iout + pid->Dout;
        //对总输出做一个先限幅
        LimitMax(pid->out, pid->max_out);
	}
	return pid->out;
}

void PID_DeInit(PID_TypeDef *pid)
{
    if (pid == NULL)
        return;
		
	//当前误差清零
    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    //微分项清零
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    //输出清零
    pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
    //目标值和当前值清零
    pid->fdb = pid->set = 0.0f;
}


void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//定时器 TIM4 初始化
	TIM_TimeBaseStructure.TIM_Period = (72-1); 
	TIM_TimeBaseStructure.TIM_Prescaler =(1000-1); 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 
	//中断优先级 NVIC 设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	TIM_Cmd(TIM4, ENABLE);
}


void TIM4_IRQHandler() //TIM4中断函数
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



















