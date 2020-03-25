
#include "delay.h"

#define SET_CS()		GPIO_SetBits(GPIOA,GPIO_Pin_5)		//GPIOF9,F10…Ë÷√∏ﬂ£¨µ∆√
#define CLR_CS()		GPIO_ResetBits(GPIOA,GPIO_Pin_5)

#define	SET_SCLK()		GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define	CLR_SCLK()		GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define SET_SDI()		GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define CLR_SDI()		GPIO_ResetBits(GPIOB,GPIO_Pin_14)

#define	SET_A0()		GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define	CLR_A0()		GPIO_ResetBits(GPIOA,GPIO_Pin_6)

#define SET_A1()		GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define CLR_A1()		GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define SET_WR()		GPIO_SetBits(GPIOC,GPIO_Pin_15)
#define CLR_WR()		GPIO_ResetBits(GPIOC,GPIO_Pin_15)

#define SET_SAMPLE()	GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define CLR_SAMPLE()	GPIO_ResetBits(GPIOC,GPIO_Pin_13)

#define 	POSITION 	0
#define 	VELOCITY 	1
#define 	CONFIG 		2



void AD2S1210SelectMode(unsigned char mode);
void AD2S1210Initiate(void);
void WriteToAD2S1210(u8 addr, u8 data);
void ReadFromAD2S1210(u8 mode, u8 addr, u8* buf);
void AD2S1210_Write(unsigned char count, unsigned char *buf);
void AD2S1210_Read(unsigned char count, unsigned char *buf);



