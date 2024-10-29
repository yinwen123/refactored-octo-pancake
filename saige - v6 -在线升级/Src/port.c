/*
名称：port.c
功能：IO口源文件
日期：20151119
*/

#include "inc.h"

#define DEMO

#define PP_2M_L 	0x80
#define PP_2M_H 	0x84
#define DP_2M_L 	0xC0
#define DP_2M_H 	0xC4

//#define AP_50M	0x0B
//#define AP_2M	 	0x0A
//#define AD_2M	 	0x0E
#define IN_UP	 	0x08
#define IN_DOWN	 	0x10
#define IN_NOP	 	0x00
#define IN_AIN	 	0x01
void gpio_clock_cl(void)
{
	
}
void gpio_mode(GPIO_TypeDef *gpio,u32 port,u8 mode)
{
	if(mode & 0x80)	//输入输出
	{
		gpio->DIR &= ~(1 << port);
		if(mode & 0x40)	//开漏
		{
			gpio->OPENDRAIN |= (1 << port);
		}
		else
		{
			gpio->OPENDRAIN &= ~(1 << port);
		}
		if(mode & 0x04)	//置位
		{
			gpio->BSRR |= (1 << port);
		}
		else
		{
			gpio->BRR |= (1 << port);
		}
	}
	else
	{
		gpio->DIR |= (1 << port);
	}
	
	if(mode & 0x10)	//下拉
	{
		gpio->PDR |= (1 << port);
	}
	else
	{
		gpio->PDR &= ~(1 << port);
	}
	if(mode & 0x08)	//上拉
	{
		gpio->PUR |= (1 << port);
	}
	else
	{
		gpio->PUR &= ~(1 << port);
	}
	if(mode & 0x01)	//模拟
	{
		gpio->ANALOG |= (1 << port);
	}
	else
	{
		gpio->ANALOG &= ~(1 << port);
	}
}

void gpio_init (void)
{
	CW_SYSCTRL->AHBEN=0x00000000;
	CW_SYSCTRL->AHBEN|=0x72;
	CW_SYSCTRL->APBEN2_f.ADC=1;
	CW_SYSCTRL->APBEN2_f.BTIM=1;
    CW_SYSCTRL->CR2=0x5A5A0001;    //NRSET配置为普通io
//	SYSCTRL->CR2_f.KEY=0x5A5A;
//	SYSCTRL->CR2_f.RSTIO=1;
	gpio_mode(CW_GPIOC,5,PP_2M_L);   //PDOWN_M

//	gpio_mode(CW_GPIOB,0,PP_2M_L);   //LOGO
	gpio_mode(CW_GPIOB,6,PP_2M_L);   //OUTNFC
	gpio_mode(CW_GPIOA,7,IN_AIN);    //IN-VCC
    gpio_mode(CW_GPIOA,6,IN_NOP);   //IN-ACC
	gpio_mode(CW_GPIOA,4,PP_2M_L);   //485-RE 0接收 1发送
	gpio_mode(CW_GPIOA,0,PP_2M_H);   //LED-SCL
	gpio_mode(CW_GPIOB,2,IN_NOP);   //IN_YXT
	gpio_mode(CW_GPIOB,4,PP_2M_H);  //2LED-SDA
	gpio_mode(CW_GPIOB,5,PP_2M_H);  //LED-SDA
    gpio_mode(CW_GPIOC,1,IN_NOP);
	
}
void nfc_afinit(u8 t)
{
	if(t)
	{
		gpio_mode(CW_GPIOC, 2, PP_2M_L);   //MOSI
		//gpio_mode(GPIOC, 1, PP_2M_L);   //SCK
		gpio_mode(CW_GPIOC, 0, PP_2M_L);   //SCK
		gpio_mode(CW_GPIOB, 1, PP_2M_H);   //NSS
	}
	else
	{
		gpio_mode(CW_GPIOC, 2, IN_NOP);   //MOSI
		gpio_mode(CW_GPIOC, 1, IN_NOP);   //MISO
		gpio_mode(CW_GPIOC, 0, IN_NOP);   //SCK
		gpio_mode(CW_GPIOB, 1, IN_NOP);   //NSS
	}
}



