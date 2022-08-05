#include "stm32f10x.h"
#include "ads1118.h"
#include "delay.h"

void ADS1118_Init(void)	   //ADS1118初始化
{
	uint16_t config = ADS1118_CONFIG_SS_START_OFF |  // 
                    ADS1118_CONFIG_MODE_CONTIN  |  //连续转换模式 
                    ADS1118_CONFIG_DR_128SPS   |  // 转换 速率 SPS
                    ADS1118_CONFIG_TS_MODE_ADC |  // 读取ADC，而不是温度
                    ADS1118_CONFIG_PULL_UP_ON  |  //上拉电阻启用
                    ADS1118_CONFIG_NOP_VALID   |  // this is valid data (default)
										ADS1118_CONFIG_PGA_6_144V  | // FSR is ±6.144 V
										ADS1118_CONFIG_MUX_SINGLE_0|  //AINP is AIN0 and AINN is GND//单端
                    ADS1118_CONFIG_RESV;          // reserved bits must be set to 1
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(ADS1118_SCLK_GPIO_CLK|
												 ADS1118_DIN_GPIO_CLK|
												 ADS1118_CS_GPIO_CLK|
												 ADS1118_DOUT_GPIO_CLK, ENABLE);	 //使能端口时钟

	GPIO_InitStructure.GPIO_Pin = ADS1118_SCLK_PIN|ADS1118_DIN_PIN|ADS1118_CS_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADS1118_DOUT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	CS_H;
	MISO_H;
	SCLK_L;
	MOSI_L;
	
	Write_ADS1118(config,1);//设置ads1118
	 
	
}

/*******************************************************************************
//函数名称：ADS_SEL_Read（）
//函数功能：读取各路电压，通过两个switch选择读取不同的通道
//输    入：road:增益放大器两端的电压选择，并选择测几路电压
//          Ref: 选择参考电压，有6种选择
//          mode: 是否更新读回数据 0读回 1不读回
//输    出：dat：16位ad转换数据
//备    注：这一次读出的转换数据是上一次的转换数据，不要混淆.这里选择的是单次
            转换电压值，当然，也可以选择多次转换,通过寄存器的第8位可以设置
*******************************************************************************/
int16_t ADS_SEL_Read(uint8_t road,uint8_t Ref,uint8_t mode)         //测几路电压
{
    int dat = 0;
		uint16_t config = ADS1118_CONFIG_SS_START_OFF |  // 
                    ADS1118_CONFIG_MODE_CONTIN  |  //连续转换模式 
                    ADS1118_CONFIG_DR_128SPS   |  // 转换 速率 SPS
                    ADS1118_CONFIG_TS_MODE_ADC |  // 读取ADC，而不是温度
                    ADS1118_CONFIG_PULL_UP_ON  |  //上拉电阻启用
                    ADS1118_CONFIG_NOP_VALID   |  // this is valid data (default)
                    ADS1118_CONFIG_RESV;          // reserved bits must be set to 1
    switch(road)	   //选择有效转换的路数
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
			case 0:  config |= ADS1118_CONFIG_PGA_6_144V;break;    //000 : FS = ±6.144V(1)
			case 1:  config |= ADS1118_CONFIG_PGA_4_096V;break;    //001 : FS = ±4.096V(1)
			case 2:  config |= ADS1118_CONFIG_PGA_2_048V;break;    //002 : FS = ±2.048V(1)
			case 3:  config |= ADS1118_CONFIG_PGA_1_024V;break;    //003 : FS = ±1.024V(1)
			case 4:  config |= ADS1118_CONFIG_PGA_0_512V;break;    //004 : FS = ±0.512V(1)
			case 5: case 6: case 7: config |= ADS1118_CONFIG_PGA_0_256V;break;    //005 : FS = ±0.256V(1)
			default : break;
    }
    dat = Write_ADS1118(config,mode);
    return dat;
}
/*******************************************************************************
//函数名称：Write_ADS1118（）
//函数功能：设置1118寄存器
//输    入：config:寄存器配置
//          discardData: 是否更新读回数据 0读回 1不读回
//返    回： 16位ad转换数据
//备    注： 
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
	 for(i=0;i<16;i++)     // 循环16次 
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


