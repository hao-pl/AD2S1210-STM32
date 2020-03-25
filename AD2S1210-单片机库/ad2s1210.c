#include "ad2s1210.h"
#include "stm32f4xx.h"


void AD2S1210SelectMode(unsigned char mode)
{
	switch (mode)
	{
		case POSITION:
								CLR_A0();
								CLR_A1();
								delay_us(20);		//Normal Mode position output
								break;
		case VELOCITY:
								
								CLR_A0();
								SET_A1();
								delay_us(20);		//Normal Mode velocity output
								break;
		case CONFIG:
								SET_A0();
								SET_A1();
								delay_us(20);		//Configuration Mode
								break;
		default:
								break;
	}
}
void AD2S1210Initiate(void)
{
	PAout(15) = 0;  
	SET_SAMPLE();
	delay_us(20);
	PAout(15) = 1; 
	delay_ms(60);
	CLR_SAMPLE();
	delay_ms(1);
	SET_SAMPLE();
}

void WriteToAD2S1210(u8 addr, u8 data)
{
	unsigned	char	buf;

	//write control register address
	buf = addr;

	SET_SCLK();		//SET_SCLK();
	delay_ms(1);
	SET_CS();		//SET_CS();
	delay_ms(1);
	CLR_CS();		//CLR_CS();
	delay_ms(1);
	
	SET_WR();		//SET_WR();
	delay_ms(1);
	CLR_WR();		//CLR_WR();
	delay_ms(1);

	AD2S1210_Write(1,&buf);	  	 
	
	SET_WR();			//SET_WR();
	delay_ms(1);
	SET_CS();			//SET_CS();	
	//write control register address

	//write control register data
	buf = data;

	SET_SCLK();			//SET_SCLK();
	delay_ms(1);
	SET_CS();			//SET_CS();
	delay_ms(1);
	CLR_WR();			//CLR_CS();
	delay_ms(1);
	
	SET_WR();			//SET_WR();
	delay_ms(1);
	CLR_WR();			//CLR_WR();
	delay_ms(1);

	AD2S1210_Write(1,&buf);	  	 

	SET_WR();			//SET_WR();
	delay_ms(1);
	SET_CS();			//SET_CS();	
	//write control register data
}

void ReadFromAD2S1210(u8 mode, u8 addr, u8* buf)
{
	if(mode == CONFIG)
	{
		//write control register address
		buf[0] = addr;

		SET_SCLK();			//SET_SCLK();
		delay_ms(1);
		PAout(5) = 1;		//SET_CS();
		delay_ms(1);
		PAout(5) = 0;		//CLR_CS();
		delay_ms(1);

		SET_WR();			//SET_WR();
		delay_ms(1);
		CLR_WR();			//CLR_WR();
		delay_ms(1);

		AD2S1210_Write(1,buf);

		SET_WR();			//SET_WR();
		delay_ms(1);
		PAout(5) = 1;		//SET_CS();
		//write control register address
		delay_ms(10);

		//read 1-byte register
		
		SET_SCLK();			//SET_SCLK();
			
		PAout(5) = 1;		//SET_CS();
		SET_WR();			//SET_WR();
		delay_ms(1);

		PAout(5) = 0;//CLR_CS();
	//	PAout(4) = 0;
		delay_ms(1);

		SET_SCLK();			//CLR_SCLK();	
		delay_ms(1);

		CLR_WR();			//CLR_WR();
		delay_ms(1);

		AD2S1210_Read(1,buf);	

		SET_WR();			//SET_WR();
		delay_ms(1);

		PAout(5) = 1;		//SET_CS();
	//read 1-byte register
	}
	else if(mode==POSITION||mode==VELOCITY)
	{
		SET_SAMPLE();
		delay_ms(1);
		CLR_SAMPLE();
		delay_ms(5);

		//read 3-byte register 
		SET_SCLK();
				
		SET_CS();
		SET_WR();
		delay_ms(1);
		
		CLR_CS();
		delay_ms(1);
	
		CLR_SCLK();	
		delay_ms(1);
			
		CLR_WR();
		delay_ms(1);
	
		AD2S1210_Read(3,buf);		//read data register
	
		SET_WR();
		delay_ms(1);
	
		SET_CS();
		//read 3-byte register
	}	
}

void AD2S1210_Write(unsigned char count, unsigned char *buf)
{
	unsigned	char	ValueToWrite = 0;
  	unsigned	char	i = 0;
	unsigned	char	j = 0;
		
	for(i=count;i>0;i--)
 	{
	 	ValueToWrite = *(buf + i - 1);
		for(j=0; j<8; j++)
		{
			SET_SCLK();				//输入时钟//SET_SCLK();
			if(0x80 == (ValueToWrite & 0x80))
			{
				SET_SDI();			//Send one to SDI pin
			}
			else
			{
				CLR_SDI();			//Send zero to SDI pin
			}
			delay_ms(1);
			CLR_SCLK();				//输入时钟
			delay_ms(1);
			ValueToWrite <<= 1;		//Rotate data
		}
	}
}

//---------------------------------
//void ReadFromAD2S(unsigned char count,unsigned char *buf)
//---------------------------------
//Function that reads from the AD2S via the SPI port. 
//--------------------------------------------------------------------------------
void AD2S1210_Read(unsigned char count, unsigned char *buf)
{
	unsigned	char	i = 0;
	unsigned	char	j = 0;
	unsigned	int  	iTemp = 0;
	unsigned	char  	RotateData = 0;
	
	for(j=count; j>0; j--)
	{
		for(i=0; i<8; i++)
		{
		    SET_SCLK();					//输入时钟//SET_SCLK();
			RotateData <<= 1;			//Rotate data
			delay_ms(1);
			//iTemp = PBin(15);			//Read SDO of AD2S
			iTemp = GPIOB->IDR;
			CLR_SCLK();	
			if(0x00008000 == (iTemp & 0x00008000))
			{
				RotateData |= 1;	
			}
			delay_ms(1);
		
		}
		*(buf + j - 1)= RotateData;	  	
	}
}	

u16 SPI2_RW(u16 mosi)
{					 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位		  
	SPI_I2S_SendData(SPI2, mosi);            						//通过外设SPIx发送一个数据
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位  						    
	return SPI_I2S_ReceiveData(SPI2);          						//返回通过SPIx最近接收的数据					    
}


