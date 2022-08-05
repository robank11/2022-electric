#include "stm32f10x.h"
#include "ads1118.h"
#include "delay.h"

void ADS1118_Init(void)	   //ADS1118��ʼ��
{
	uint16_t config = ADS1118_CONFIG_SS_START_OFF |  // 
                    ADS1118_CONFIG_MODE_CONTIN  |  //����ת��ģʽ 
                    ADS1118_CONFIG_DR_128SPS   |  // ת�� ���� SPS
                    ADS1118_CONFIG_TS_MODE_ADC |  // ��ȡADC���������¶�
                    ADS1118_CONFIG_PULL_UP_ON  |  //������������
                    ADS1118_CONFIG_NOP_VALID   |  // this is valid data (default)
										ADS1118_CONFIG_PGA_6_144V  | // FSR is ��6.144 V
										ADS1118_CONFIG_MUX_SINGLE_0|  //AINP is AIN0 and AINN is GND//����
                    ADS1118_CONFIG_RESV;          // reserved bits must be set to 1
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(ADS1118_SCLK_GPIO_CLK|
												 ADS1118_DIN_GPIO_CLK|
												 ADS1118_CS_GPIO_CLK|
												 ADS1118_DOUT_GPIO_CLK, ENABLE);	 //ʹ�ܶ˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = ADS1118_SCLK_PIN|ADS1118_DIN_PIN|ADS1118_CS_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADS1118_DOUT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	CS_H;
	MISO_H;
	SCLK_L;
	MOSI_L;
	
	Write_ADS1118(config,1);//����ads1118
	 
	
}

/*******************************************************************************
//�������ƣ�ADS_SEL_Read����
//�������ܣ���ȡ��·��ѹ��ͨ������switchѡ���ȡ��ͬ��ͨ��
//��    �룺road:����Ŵ������˵ĵ�ѹѡ�񣬲�ѡ��⼸·��ѹ
//          Ref: ѡ��ο���ѹ����6��ѡ��
//          mode: �Ƿ���¶������� 0���� 1������
//��    ����dat��16λadת������
//��    ע����һ�ζ�����ת����������һ�ε�ת�����ݣ���Ҫ����.����ѡ����ǵ���
            ת����ѹֵ����Ȼ��Ҳ����ѡ����ת��,ͨ���Ĵ����ĵ�8λ��������
*******************************************************************************/
int16_t ADS_SEL_Read(uint8_t road,uint8_t Ref,uint8_t mode)         //�⼸·��ѹ
{
    int dat = 0;
		uint16_t config = ADS1118_CONFIG_SS_START_OFF |  // 
                    ADS1118_CONFIG_MODE_CONTIN  |  //����ת��ģʽ 
                    ADS1118_CONFIG_DR_128SPS   |  // ת�� ���� SPS
                    ADS1118_CONFIG_TS_MODE_ADC |  // ��ȡADC���������¶�
                    ADS1118_CONFIG_PULL_UP_ON  |  //������������
                    ADS1118_CONFIG_NOP_VALID   |  // this is valid data (default)
                    ADS1118_CONFIG_RESV;          // reserved bits must be set to 1
    switch(road)	   //ѡ����Чת����·��
    {
			case 0:  config |= ADS1118_CONFIG_MUX_DIFF_0_1;break;    //AINP = AIN0 and AINN = AIN1 (default)
			case 1:  config |= ADS1118_CONFIG_MUX_DIFF_0_3;break;    //AINP = AIN0 and AINN = AIN3
			case 2:  config |= ADS1118_CONFIG_MUX_DIFF_1_3;break;    //AINP = AIN1 and AINN = AIN3
			case 3:  config |= ADS1118_CONFIG_MUX_DIFF_2_3;break;    //AINP = AIN2 and AINN = AIN3
			case 4:  config |= ADS1118_CONFIG_MUX_SINGLE_0;break;    //AINP = AIN0 and AINN = GND
			case 5:  config |= ADS1118_CONFIG_MUX_SINGLE_1;break;    //AINP = AIN1 and AINN = GND
			case 6:  config |= ADS1118_CONFIG_MUX_SINGLE_2;break;    //AINP = AIN2 and AINN = GND
			case 7:  config |= ADS1118_CONFIG_MUX_SINGLE_3;break;    //AINP = AIN3 and AINN = GND
			default : break;
    }
    switch(Ref)
    {
			case 0:  config |= ADS1118_CONFIG_PGA_6_144V;break;    //000 : FS = ��6.144V(1)
			case 1:  config |= ADS1118_CONFIG_PGA_4_096V;break;    //001 : FS = ��4.096V(1)
			case 2:  config |= ADS1118_CONFIG_PGA_2_048V;break;    //002 : FS = ��2.048V(1)
			case 3:  config |= ADS1118_CONFIG_PGA_1_024V;break;    //003 : FS = ��1.024V(1)
			case 4:  config |= ADS1118_CONFIG_PGA_0_512V;break;    //004 : FS = ��0.512V(1)
			case 5: case 6: case 7: config |= ADS1118_CONFIG_PGA_0_256V;break;    //005 : FS = ��0.256V(1)
			default : break;
    }
    dat = Write_ADS1118(config,mode);
    return dat;
}
/*******************************************************************************
//�������ƣ�Write_ADS1118����
//�������ܣ�����1118�Ĵ���
//��    �룺config:�Ĵ�������
//          discardData: �Ƿ���¶������� 0���� 1������
//��    �أ� 16λadת������
//��    ע�� 
*******************************************************************************/
int16_t Write_ADS1118(uint16_t config,uint8_t discardData)
{
	uint8_t i=0;
  static int16_t read=0;
	CS_L;
	if(discardData==0) 
	{
		read=0;
	}
	delay_us(1);
	 for(i=0;i<16;i++)     // ѭ��16�� 
	{		
		if(config & 0x8000)MOSI_H;
		else MOSI_L;
		config <<= 1;

		delay_us(1);
		SCLK_H;
		delay_us(1);
		SCLK_L;
		delay_us(1);
		
		if(discardData==0)
		{
			read<<=1;
			if(READ_MISO)read ++;
				
		}else{
			delay_us(2);
		}
	}
	CS_H;
	delay_us(2);
	CS_L;
//	SCLK_L;
//	MOSI_L;
//	MISO_H;
	return read;
}


