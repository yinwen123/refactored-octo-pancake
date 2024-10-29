/*
名称：tim.c
功能：定时器源文件
日期：20151119
*/

#include "inc.h"

void tim_init (void)
{
	//SYSCTRL->APBEN2_f.bBTIM = 1;
	
	CW_BTIM1->BCR = 0;
	//TIM1->ARR = 2400;
	//TIM1->IER_f.OV = 1;
	//TIM1->BCR_f.EN = 1;
	
	CW_BTIM2->BCR = 0;	//分频
	//TIM2->BCR = (3 << 7);	//8分频
	CW_BTIM2->ARR = 1200;
	CW_BTIM2->IER_f.OV = 1;
	CW_BTIM2->BCR_f.EN = 1;
	
	//NVIC_SetPriority(BTIM1_IRQn, 0);
	//NVIC_SetPriority(BTIM2_IRQn, 1);
	//NVIC_EnableIRQ(BTIM1_IRQn);
	//NVIC_EnableIRQ(BTIM2_IRQn);
	
	NVIC->IP[5] = (3 << 14);	//降低T2中断优先级
	
	NVIC->ISER[0] |= 0x00300000;	//T1 2  20 21
	
	//__enable_irq();
}
void GTIM_INIT()
{
    CW_GPIOB->DIR_f.PIN0=0x00;
    CW_GPIOB->OPENDRAIN_f.PIN0=0x00;
    CW_GPIOB->ANALOG_f.PIN0=0;
    CW_GPIOB->AFRL_f.AFR0=5;
    CW_SYSCTRL->APBEN1_f.GTIM=1;
    CW_GTIM->CR0_f.MODE=0x00;
    CW_GTIM->CR0_f.ONESHOT=0;
    CW_GTIM->CR0_f.PRS=0x07;
    CW_GTIM->ARR=1000;
    CW_GTIM->CCR1=0;
    CW_GTIM->CMMR_f.CC1M=0x0f;
//    CW_GTIM->IER_f.OV=1;
//    NVIC->ISER[0]|=0x01<<16;
//    NVIC->IP[4]|=0x11;
    CW_GTIM->CR0|=0x01;  
}
void chage_pwm(uint16_t vlue)
{
   CW_GTIM->CCR1= vlue;
}
u16 logov=0;
void GTIM_IRQHandler()
{
    if(CW_GTIM->ISR&(0x01))
    {
       CW_GTIM->ICR&=~(0x01);
        chage_pwm(logov);
    }
}


