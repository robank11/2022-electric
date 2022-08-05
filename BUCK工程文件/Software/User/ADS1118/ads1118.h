#ifndef __ADS1118_H
#define __ADS1118_H	 
#include "stm32f10x.h"

//GPIO����
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����



//�˿ڶ���
#define CONFIG_VALUE    0X408B       //AIN0-AIN1  4.096  128sps  pull on DOUT

#define ADS1118_SCLK_PIN                   GPIO_Pin_12
#define ADS1118_SCLK_GPIO_PORT             GPIOC
#define ADS1118_SCLK_GPIO_CLK              RCC_APB2Periph_GPIOC

#define ADS1118_DIN_PIN                   GPIO_Pin_11
#define ADS1118_DIN_GPIO_PORT             GPIOC
#define ADS1118_DIN_GPIO_CLK              RCC_APB2Periph_GPIOC

#define ADS1118_CS_PIN                   GPIO_Pin_8
#define ADS1118_CS_GPIO_PORT             GPIOC
#define ADS1118_CS_GPIO_CLK              RCC_APB2Periph_GPIOC

#define ADS1118_DOUT_PIN                   GPIO_Pin_10
#define ADS1118_DOUT_GPIO_PORT             GPIOC
#define ADS1118_DOUT_GPIO_CLK              RCC_APB2Periph_GPIOC


#define SCLK PCout(12)	//ads1118ʱ������
#define MOSI PCout(11)	//ads1118 DIN����
#define CS   PCout(8)	//ads1118 CS����

#define MISO PCout(10)	//ads1118 DOUT/DRDY����
#define READ_MISO  PCin(10)

#define SCLK_H SCLK=1
#define SCLK_L SCLK=0

/*��Ƭ��Ϊ����*/
#define MOSI_H MOSI=1  //�������ӻ���
#define MOSI_L MOSI=0

#define MISO_H MISO=1  //������ӻ���
#define MISO_L MISO=0

#define   CS_H   CS=1
#define   CS_L   CS=0

//����ת����ʼ
#define ADS1118_CONFIG_SS_START_MASK 0x8000
#define ADS1118_CONFIG_SS_START_ON   0x8000  //��ʼ����ת��
#define ADS1118_CONFIG_SS_START_OFF  0x0000  // ��Ч
 
//�����·����������
#define ADS1118_CONFIG_MUX_MASK      0x7000
#define ADS1118_CONFIG_MUX_DIFF_0_1  0x0000  //AINP is AIN0 and AINN is AIN1 //���
#define ADS1118_CONFIG_MUX_DIFF_0_3  0x1000  //AINP is AIN0 and AINN is AIN3
#define ADS1118_CONFIG_MUX_DIFF_1_3  0x2000  //AINP is AIN1 and AINN is AIN3
#define ADS1118_CONFIG_MUX_DIFF_2_3  0x3000  //AINP is AIN2 and AINN is AIN3
#define ADS1118_CONFIG_MUX_SINGLE_0  0x4000  //AINP is AIN0 and AINN is GND//����
#define ADS1118_CONFIG_MUX_SINGLE_1  0x5000  //AINP is AIN1 and AINN is GND
#define ADS1118_CONFIG_MUX_SINGLE_2  0x6000  //AINP is AIN2 and AINN is GND
#define ADS1118_CONFIG_MUX_SINGLE_3  0x7000  //AINP is AIN3 and AINN is GND
	
//�ɱ������Ŵ�������
#define ADS1118_CONFIG_PGA_MASK      0x0E00  //FSR is ��0.256 V
#define ADS1118_CONFIG_PGA_6_144V    0x0000  //FSR is ��6.144 V
#define ADS1118_CONFIG_PGA_4_096V    0x0200  //FSR is ��4.096 V
#define ADS1118_CONFIG_PGA_2_048V    0x0400  //FSR is ��2.048 V
#define ADS1118_CONFIG_PGA_1_024V    0x0600  //FSR is ��1.024 V
#define ADS1118_CONFIG_PGA_0_512V    0x0800  //FSR is ��0.512 V
#define ADS1118_CONFIG_PGA_0_256V    0x0A00  //FSR is ��0.256 V

//�豸����ģʽ
#define ADS1118_CONFIG_MODE_MASK     0x0100
#define ADS1118_CONFIG_MODE_CONTIN   0x0000 //����ת��ģʽ
#define ADS1118_CONFIG_MODE_SINGLE   0x0100 //���磬����ת��ģʽ

//��������
#define ADS1118_CONFIG_DR_MASK       0x00E0  //860SPS
#define ADS1118_CONFIG_DR_8SPS       0x0000
#define ADS1118_CONFIG_DR_16SPS      0x0020
#define ADS1118_CONFIG_DR_32SPS      0x0040
#define ADS1118_CONFIG_DR_64SPS      0x0060
#define ADS1118_CONFIG_DR_128SPS     0x0080  
#define ADS1118_CONFIG_DR_250SPS     0x00A0
#define ADS1118_CONFIG_DR_475SPS     0x00C0
#define ADS1118_CONFIG_DR_860SPS     0x00E0

//�¶ȴ�����ģʽ
#define ADS1118_CONFIG_TS_MODE_MASK  0x0010
#define ADS1118_CONFIG_TS_MODE_ADC   0x0000  //ADCģʽ
#define ADS1118_CONFIG_TS_MODE_TEMP  0x0010  //�¶ȴ�����ģʽ

//����ʹ��
//����CSΪ�ߵ�ƽʱ����λʹ��DOUT / DRDY�����ϵ��ڲ����������衣 ���ú��ڲ�400k����������������·���ӵ���Դ�� ����ʱ��DOUT / DRDY�������ա�
#define ADS1118_CONFIG_PULL_UP_MASK  0x0008
#define ADS1118_CONFIG_PULL_UP_ON    0x0008  //������������
#define ADS1118_CONFIG_PULL_UP_OFF   0x0000  //�����������

//�����Ƿ�����д�����üĴ���
#define ADS1118_CONFIG_NOP_MASK      0x0006
#define ADS1118_CONFIG_NOP_VALID     0x0002  // ��Ч���ݣ�����Config�Ĵ���
#define ADS1118_CONFIG_NOP_INVALID   0x0006  //��Ч���ݣ�������Config�Ĵ���������

//Ĭ����Ҫ����
#define ADS1118_CONFIG_RESV          0x0001  // default, required

//
#define ADS1118_CONST_C_PER_BIT      (0.03125)
#define ADS1118_CONST_BIT_PER_C      32

//
#define ADS1118_CONST_6_144V_LSB_mV  (0.1875)
#define ADS1118_CONST_4_096V_LSB_mV  (0.125)
#define ADS1118_CONST_2_048V_LSB_mV  (0.0625)
#define ADS1118_CONST_1_024V_LSB_mV  (0.03125)
#define ADS1118_CONST_0_512V_LSB_mV  (0.015625)
#define ADS1118_CONST_0_256V_LSB_mV  (0.0078125)


void ADS1118_Init(void);

int16_t ADS_SEL_Read(uint8_t road,uint8_t Ref,uint8_t mode);  

int16_t Write_ADS1118(uint16_t config,uint8_t discardData);
		 				    
#endif

