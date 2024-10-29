/*
	名称：adc.c
	作者：LiGuo.Li
	功能描述：ADC源文件
	创建日期：2017.6.7
	修改日期：2017.6.7
	版本：V1.0
	功能修订日志：
	版本      日期      修订内容
	V1.0    2017.6.7	创建文件
*/

#include"inc.h"

#define ADC_CH  4

//u16 ADT = 0;

u8 Soc_tmp = 99,Soc = 0;
uint16_t UD_ADCREF=0;
//const u16 V48_ADC0[9] = {5000,4900,4800,4700,4600,4500,4400,4300,4200};
const u16 V48_ADC0[9] = {5000,4758,4658,4558,4508,4448,4388,4328,4200};
const u16 V60_ADC0[9] = {6200,6081,5962,5843,5725,5606,5481,5368,5250};
const u16 V72_ADC0[9] = {7400,7262,7125,6987,6850,6712,6575,6537,6300};
uint16_t percent,voltage;
void adc_init(void)
{
	//SYSCTRL->APBEN2_f.bADC = 1;
	CW_ADC->CR0_f.EN = 1;
	while(CW_ADC->ISR_f.READY == 0);
	//ADC->SQR_f.SQR0 = ADC_CH;	//通道0
	//ADC->CR0_f.CLK = 1;
	//ADC->CR0_f.SAM = 3;
	//ADC->CR0_f.REF = 1;
	//ADC->CR0_f.MODE = 0;
	
	CW_ADC->CR0 |= 0x1940; //
	
	CW_ADC->CR1_f.CHMUX = ADC_CH;
	CW_ADC->START_f.START = 1;
}
u16 pre1,pre2;
void adc_check(void)
{
    static u16 *adc_lev = (u16*)V48_ADC0,cnt_timh = 0,cnt_timl = 0,cnt = 0,first = 0;
	static u32 adc_t = 0;
	
    if(CW_ADC->ISR_f.EOC)
    {
		u16 adt = CW_ADC->RESULT0;
        adc_t += adt;
		
		CW_ADC->ICR_f.EOC = 0;
		cnt ++;
		if(cnt > 16383) //820ms
		{
			cnt = 0;
			
			adc_t >>= 14;


			if(INIT_MODE > 2)
			{
				INIT_MODE = 1;
			}
			else if(INIT_MODE == 1)
			{
				INIT_MODE = 0;
				
				u32 t = (6000 << 10) / adc_t; //60V校准  
				
				UD_ADCREF = t;	
				
				flash_adc_updata();

				INIT_MODE = 2;
			}
			else
			{
                
				u32 t = (adc_t * UD_ADCREF) >> 10;
				voltage=(t/10)+2;
                if(V60_STA)
                {
                  pre1=V60_ADC0[0]-V60_ADC0[8];
                  pre2=t;
                  if(pre2>=V60_ADC0[8])
                   pre2-=V60_ADC0[8];
                  else
                    pre2=0;
                  percent=pre2*125/pre1;
                }
                else if(V72_STA)
                {
                  pre1=V72_ADC0[0]-V72_ADC0[8];
                   pre2=t;
                  if(pre2>=V72_ADC0[8])
                   pre2-=V72_ADC0[8];
                  else
                     pre2=0;
                  percent=pre2*125/pre1;
                }
                else
                {
                 pre1=V48_ADC0[0]-V48_ADC0[8];
                    pre2=t;
                 if(pre2>=V48_ADC0[8])
                    pre2-=V48_ADC0[8];
                  else
                    pre2=0;
                 percent=pre2*125/pre1;
               
                }
                 
                if(percent>100)
                    percent=100;
//				if(Start_type == 0)
//				{
//					Soc_tmp = 99;
//				}
                if(first == 0)
				{
					first = 1;
					Soc_tmp = 11;
				}
					if(V60_STA)
					{
						adc_lev = (u16*)V60_ADC0;
					}
					else if(V72_STA)
					{
						adc_lev = (u16*)V72_ADC0;
					}
					else
					{
						adc_lev = (u16*)V48_ADC0;
					}
					
					switch(Soc_tmp)
					{
						case 11:
						{
							cnt_timl = 0;
							cnt_timh = 0;
							if(t > adc_lev[1])
							{
								Soc_tmp = 8;
							}
							else if(t > adc_lev[2])
							{
								Soc_tmp = 7;
							}
							else if(t > adc_lev[3])
							{
								Soc_tmp = 6;
							}
							else if(t > adc_lev[4])
							{
								Soc_tmp = 5;
							}
							else if(t > adc_lev[5])
							{
								Soc_tmp = 4;
							}
							else if(t > adc_lev[6])
								Soc_tmp = 3;
							else if(t > adc_lev[7])
								Soc_tmp = 2;
							else if(t > adc_lev[8])
								Soc_tmp=1;
                            else
                               Soc_tmp=0; 
							break;
						}
						case 8:
						{
							cnt_timh = 0;
							if(t < adc_lev[1])
							{
								if(cnt_timl < 3)
								{
									cnt_timl ++;
								}
								else
								{
									cnt_timl = 0;
									Soc_tmp = 7;
								}
							}
							else
							{
								cnt_timl = 0;
							}
							break;
						}
						case 7:
						{
							if(t < adc_lev[2])
							{
								cnt_timh = 0;
								if(cnt_timl < 3)
								{
									cnt_timl ++;
								}
								else
								{
									cnt_timl = 0;
									Soc_tmp = 6;
								}
							}
							else if(t > adc_lev[1])
							{
								cnt_timl = 0;
								if(cnt_timh < 2)
								{
									cnt_timh ++;
								}
								else
								{
									cnt_timh = 0;
									Soc_tmp = 8;
								}
							}
							else
							{
								cnt_timl = 0;
								cnt_timh = 0;
							}
							break;
						}
						case 6:
						{
							if(t < adc_lev[3])
							{
								cnt_timh = 0;
								if(cnt_timl < 3)
								{
									cnt_timl ++;
								}
								else
								{
									cnt_timl = 0;
									Soc_tmp = 5;
								}
							}
							else if(t > adc_lev[2])
							{
								cnt_timl = 0;
								if(cnt_timh < 2)
								{
									cnt_timh ++;
								}
								else
								{
									cnt_timh = 0;
									Soc_tmp = 7;
								}
							}
							else
							{
								cnt_timl = 0;
								cnt_timh = 0;
							}
							break;
						}
						case 5:
						{
							if(t < adc_lev[4])
							{
								cnt_timh = 0;
								if(cnt_timl < 3)
								{
									cnt_timl ++;
								}
								else
								{
									cnt_timl = 0;
									Soc_tmp = 4;
								}
							}
							else if(t > adc_lev[3])
							{
								cnt_timl = 0;
								if(cnt_timh < 2)
								{
									cnt_timh ++;
								}
								else
								{
									cnt_timh = 0;
									Soc_tmp = 6;
								}
							}
							else
							{
								cnt_timl = 0;
								cnt_timh = 0;
							}
							break;
						}
						case 4:
						{
							if(t < adc_lev[5])
							{
								cnt_timh = 0;
								if(cnt_timl < 3)
								{
									cnt_timl ++;
								}
								else
								{
									cnt_timl = 0;
									Soc_tmp = 3;
								}
							}
							else if(t > adc_lev[4])
							{
								cnt_timl = 0;
								if(cnt_timh < 2)
								{
									cnt_timh ++;
								}
								else
								{
									cnt_timh = 0;
									Soc_tmp = 5;
								}
							}
							else
							{
								cnt_timl = 0;
								cnt_timh = 0;
							}
							break;
						}
						case 3:
						{
							if(t<adc_lev[6])
							{
								cnt_timh = 0;
								if(cnt_timl < 3)
								{
									cnt_timl ++;
								}
								else
								{
									cnt_timl = 0;
									Soc_tmp = 2;
								}
							}
							else if(t > adc_lev[5])
							{
								if(cnt_timh < 2)
								{
									cnt_timh ++;
								}
								else
								{
									cnt_timh = 0;
									Soc_tmp = 4;
								}
							}
							else
							{
								cnt_timl = 0;
								cnt_timh = 0;
							}
							break;
						}
						case 2:
						{
							if(t<adc_lev[7])
							{
								cnt_timh = 0;
								if(cnt_timl < 3)
								{
									cnt_timl ++;
								}
								else
								{
									cnt_timl = 0;
									Soc_tmp = 1;
								}
							}
							else if(t > adc_lev[6])
							{
								if(cnt_timh < 2)
								{
									cnt_timh ++;
								}
								else
								{
									cnt_timh = 0;
									Soc_tmp = 3;
								}
							}
							else
							{
								cnt_timl = 0;
								cnt_timh = 0;
							}
							break;
						}
						case 1:
						{
							if(t<adc_lev[8])
							{
								cnt_timh = 0;
								if(cnt_timl < 3)
								{
									cnt_timl ++;
								}
								else
								{
									cnt_timl = 0;
									Soc_tmp = 0;
								}
							}
							else if(t > adc_lev[7])
							{
								if(cnt_timh < 2)
								{
									cnt_timh ++;
								}
								else
								{
									cnt_timh = 0;
									Soc_tmp = 2;
								}
							}
							else
							{
								cnt_timl = 0;
								cnt_timh = 0;
							}
							break;
						}
						case 0:
						{
							if(t > adc_lev[8])
							{
								if(cnt_timh < 2)
								{
									cnt_timh ++;
								}
								else
								{
									cnt_timh = 0;
									Soc_tmp = 1;
								}
							}
							else
							{
								cnt_timl = 0;
								cnt_timh = 0;
							}
							break;
						}
						default:
						{
							Soc_tmp = 11;
							break;
						}
					}
			}
			
			adc_t = 0;
		}
        
        CW_ADC->START_f.START = 1;
    }
}
