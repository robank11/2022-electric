#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"
#include "delay.h"
#include "codetab.h"


#define OLED_I2C_WR	0		/* д����bit */
#define OLED_I2C_RD	1		/* ������bit */

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define OLED_GPIO_PORT_I2C	GPIOD			/* GPIO�˿� */
#define OLED_RCC_I2C_PORT 	RCC_APB2Periph_GPIOD		/* GPIO�˿�ʱ�� */
#define OLED_I2C_SCL_PIN		GPIO_Pin_3			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define OLED_I2C_SDA_PIN		GPIO_Pin_2			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define OLED_I2C_SCL_1()  GPIO_SetBits(OLED_GPIO_PORT_I2C, OLED_I2C_SCL_PIN)		/* SCL = 1 */
	#define OLED_I2C_SCL_0()  GPIO_ResetBits(OLED_GPIO_PORT_I2C, OLED_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define OLED_I2C_SDA_1()  GPIO_SetBits(OLED_GPIO_PORT_I2C, OLED_I2C_SDA_PIN)		/* SDA = 1 */
	#define OLED_I2C_SDA_0()  GPIO_ResetBits(OLED_GPIO_PORT_I2C, OLED_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define OLED_I2C_SDA_READ()  GPIO_ReadInputDataBit(OLED_GPIO_PORT_I2C, OLED_I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define OLED_I2C_SCL_1()  OLED_GPIO_PORT_I2C->BSRR = OLED_I2C_SCL_PIN				/* SCL = 1 */
	#define OLED_I2C_SCL_0()  OLED_GPIO_PORT_I2C->BRR = OLED_I2C_SCL_PIN				/* SCL = 0 */
	
	#define OLED_I2C_SDA_1()  OLED_GPIO_PORT_I2C->BSRR = OLED_I2C_SDA_PIN				/* SDA = 1 */
	#define OLED_I2C_SDA_0()  OLED_GPIO_PORT_I2C->BRR = OLED_I2C_SDA_PIN				/* SDA = 0 */
	
	#define OLED_I2C_SDA_READ()  ((OLED_GPIO_PORT_I2C->IDR & OLED_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif


void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
void i2c_CfgGpio(void);


#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78

uint8_t OLED_CheckDevice(uint8_t _Address);//���I2C�����豸OLED
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
uint8_t OLED_Test(void) ;//OLED������


#endif
