/*
名称：main.c
功能：主函数源文件
日期：20151119
*/

#include "inc.h"
#define RCC_HSI_TRIMCODEADDR (0x001007B8U)
#define RCC_LSI_TRIMCODEADDR (0x001007BAU)
volatile u16 Cnt_10ms = 0;
u32 Init_tims = 0,start_time;
extern uint16_t UD_ADCREF;
uint16_t led_cout,rel_speed=0;
uint8_t run_step=0,StartUp=0,start_step=0,cartoontip,sp_flg,led_change;
u8 half_time=0,data_chage=0;;
 u16 ledCut,numb,cut485;
void wdt_init(void)
{
	CW_SYSCTRL->APBEN1_f.IWDT =0;
//    CW_IWDT->KR = 0xCCCC;
//    CW_IWDT->KR = 0xAAAA;
}

void SystemInit (void)
{

	CW_SYSCTRL->HSI_f.TRIM=*(uint16_t*)(RCC_HSI_TRIMCODEADDR);
	CW_SYSCTRL->LSI_f.TRIM=*(uint16_t*)(RCC_LSI_TRIMCODEADDR);
	CW_SYSCTRL->HSI_f.DIV=0x08;
    CW_SYSCTRL->CR0=0x5A5A0000;
	CW_FLASH->CR2_f.WAIT=0x00;
    CW_SYSCTRL->CR1=0x5A5A0001;
	while(!CW_SYSCTRL->HSI_f.STABLE);

	
}
void BTIM2_IRQHandler(void)
{
    static u8 yxtcut=0;
    u8* step;
    CW_BTIM2->ICR_f.OV = 0;
	
	adc_check();
	
//	rf_task_50us ();
		
	com_task_50us ();
		
	quake_check ();
    over_time++;
    Tim_out ++;
    if(over_time>40)
    {
      over_time=41;
      rx_nub=0;
    }
	Cnt_10ms ++;
    if(ALM_s)
	{
       step=alm_send(4,ALM_c);
       if(*step>2)
       {
         yxtcut++;
         *step=0;                  
       }
       if(yxtcut>2)
       {
         yxtcut=0;
         *step=0;
         ALM_s=0; 
       }
	}
    else
         ACC_OFF;
    return;
}
void time_cout()
{
	Init_tims++;
	if(Init_tims%36==0)
    {
		half_time=1;
    }
	if(Start_type)
	{
      if(Init_tims%14==0)
	   start_time++;
	}
	else
    {
	  start_time=0;
      ledCut=0;numb=0;
    }
    if(Init_tims%480==0)
      data_chage=!data_chage;
    if(Init_tims%12==0)
    {
       sp_flg=1;
    }
    if(Init_tims%60==0)
      led_change=!led_change;  
    
}
void startDisplay(void)
{
	LOGO_ON;
    if(numb==0)
    {
      if(ledCut<4)
      {    
        Led_data[15]|=(0x08>>ledCut);
	    Led_data[15]|=(0x10<<ledCut);
      }
      if(ledCut>=4)
      {
        Led_data[5]|=0x80>>(ledCut-4);
        Led_data[12]|=0x01<<(ledCut-4);
        if(ledCut<9)
        {
           Led_data[4]|=0x01<<(ledCut-4);
           Led_data[13]|=0x01<<(ledCut-4);
        }
      }
    }
    if(numb==1)
    {
      if(ledCut<8)
      {
        Led2_data[2]|=0x80>>ledCut;
        Led2_data[9]|=0x80>>ledCut;
      }
      if((ledCut>=8)&&(ledCut<10))
      {
        Led2_data[11]|=0x02>>(ledCut-8);
        Led2_data[11]|=0x08>>(ledCut-8);
      }
      if((ledCut>=10)&&(ledCut<16))
      {
         Led2_data[3]|=0x20>>(ledCut-10);
      }
      if(ledCut==16)
      {
         Led2_data[8]|=0x01<<7;
      }
      if(ledCut>=10)
        Led2_data[8]|=0x01<<(ledCut-10);
      
    }
    if(numb==2)
    {
      if(ledCut<7)
      {
        Led2_data[0]|=0x40>>ledCut;
        if(ledCut<6)
        Led2_data[10]|=0x20>>ledCut;
      }
      if(ledCut<1)
      {
        Led_data[6]|=0x03<<6;
        Led2_data[14]|=0x03<<6;
      }
      if((ledCut>=1)&&(ledCut<7))
      {
         Led_data[6]|=0x20>>(ledCut-1);
         Led2_data[14]|=0x20>>(ledCut-1);
      }
      
    }
    if(numb==3)
	{
		Led2_data[13]|=0xff;
		numb=0;
		ledCut=0;
        start_time=0;
		start_step=1;
	}
	if(Init_tims%5==0)
		ledCut++;
    if(numb==0)
    {
       if(ledCut>11)
       {
          ledCut=0;
     	  numb++;
       }
    }
    else if(numb==1)
    {
      if(ledCut>16)
      {
         ledCut=0;
     	 numb++;
      }
    }
    else if(numb==2)
    {
      if(ledCut>6)
      {
         ledCut=0;
     	 numb++;
      }
    }        
}
void start_cout(void)
{
  if(start_time<10)
  {
	 XA_ON;
     MA_ON;
     MaoHao_on;
     ABS_ON;
     BaiFenHao_on;
     V_ON;
     QianYa_on;
     KM_ON ;
     TRIP_ON;
     ODO_ON;
     KM_H_ON;
     turn_left_on;
     DD_ON;
     XunHang_on;
     SKD_ON;
     turn_right_on;
	 R_ON;
	 temp.data=0xff;
     show_speed(88);
     show_gear(8);
     show_mileage(88888,0);
     power_level_display(8);
     show_zuoxia2_data(188,1);
     show_zuoxia1_data(88);
     Led_data[13]|=(0x01<<7);
	}
	else
	{
//		temp.data=0;
		XA_OFF;
		MA_OFF;
		ABS_OFF;
		V_OFF;
		QianYa_off;
		turn_left_off;
		turn_right_off;
        XunHang_off;
		R_OFF;
        MaoHao_off;
        KM_OFF ;
        ODO_OFF;
        data_chage=1;
        Led_data[4]&=~0x1f;
		Led_data[13]&=~0x1f;
        run_step=1;
        show_voltage((v485*10),0);
        Led_data[13]&=~(0x01<<7);        
	}
		
}
void run_cartoon()
{
   if(Speed>0)
   {
     if(Init_tims%25==0)
        cartoontip++;
     if(cartoontip>5)
     {
       cartoontip=0;
       Led_data[4]&=~0x1f;
	   Led_data[13]&=~0x1f;
     }
     if(cartoontip>0)
     {
       Led_data[4]|=0x01<<(cartoontip-1);
       Led_data[13]|=0x01<<(cartoontip-1);
     }         
   }
   else
   {
     if(cartoontip!=0)
     {
       Led_data[4]|=0x01<<(cartoontip-1);
       Led_data[13]|=0x01<<(cartoontip-1);
       cartoontip++;
       if(cartoontip>5)
       {
         cartoontip=0;
         Led_data[4]&=~0x1f;
	     Led_data[13]&=~0x1f; 
       }           
     }
     else
     {
         Led_data[4]&=~0x1f;
         Led_data[13]&=~0x1f;
     }
   }
  
}
void car_stat_updata()
{
    static uint8_t tcs_cut,tcs_hcut;
	if(car_stat.data.hull_error||car_stat.data.QueXiang)
		temp.STA.motor=1;
	else
		temp.STA.motor=0;
	if(car_stat.data.XunHang)
		XunHang_on;
	else
		XunHang_off;
	if(car_stat.data.ShaChe)
		temp.STA.ZhiDong=1;
	else
		temp.STA.ZhiDong=0;
	if(car_stat.data.control_error||car_stat.data.control_pr)
		temp.STA.ECU=1;
	else
		temp.STA.ECU=0;
	if(car_stat.data.FangFeiChe)
		ABS_ON;
	else
		ABS_OFF;
	if(car_stat.data.DuZhuan||car_stat.data.GuoLiu)
		FangHua_ON;
	else
		FangHua_OFF;
	if(car_stat.data.DaoChe)
		R_mod=1;
	else
		R_mod=0;
	if(car_stat.data.zhuanBa_error)
		temp.STA.ZhuanBa=1;
	else
		temp.STA.ZhuanBa=0;
    if(car_stat.data.JieNen)
       temp.STA.JieNen=1;
    else
       temp.STA.JieNen=0;
    if(car_stat.data.YunDon)
      temp.STA.DongLi=1;
    else
       temp.STA.DongLi=0;
    if(car_stat.data.ChangGui)
      temp.STA.BiaoZhun=1;
    else
      temp.STA.BiaoZhun=0;
    if(car_stat.data.TCS)
    {
      if(tcs_sta)
      {
         if( led_change)
           temp.STA.XueHua=1;
         else
           temp.STA.XueHua=0;
      }
      else
         temp.STA.XueHua=1;          
    }
    else
    {
        tcs_cut=0;
        temp.STA.XueHua=0;
    }
           
}
void show_Rmod()
{
   Led2_data[4]&=~(0x7f);
   Led2_data[5]&=~(0x7f);
   Led2_data[6]&=~(0x7f);
   Led2_data[7]&=~(0x7f);
   Led2_data[6]|=0x77;
   Led2_data[7]|=0x77;  
}
uint16_t temp3,temp4;
uint16_t odocut=360;
uint8_t last_485;

int main (void)
{
	NVIC_VOTR = 0x800;
	
	gpio_init ();
	
    flash_init();
	adc_init();

	wdt_init();
	
	tim_init ();
    GTIM_INIT();
    UART2_INIT(9600);
    while (1)
    {
	 if(Cnt_10ms > 199)
	  {
          cut485++;
          if(cut485>400)
             cut485=401; 
         if(yxt_rcv)
         {
           yxt_rcv=0;
           yxt_rcv1=1;
           timeover=300;
         }
         if(timeover)
           timeover--;
         else
         {
            yxt_rcv1=0; 
         }
		if(Start_type)
		{
			switch(run_step)
			{
				case 0:
				{
                    logov=1000;
					switch(start_step)
					{
						case 0:
					        startDisplay();
						break;
						case 1:
							start_cout( );
						break;		
					}
						
				}
				break;
				case 1:
				{
				  car_stat_updata( );
                  if(car_dt)         //电自
                  {
                      if(GuoBiao_mod)
                      {
                        temp.STA.BiaoZhun=0;
                        temp.STA.DongLi=0;
                        temp.STA.JieNen=0;
                      }
                  }
                   if(cut485>400)
                   {
//                     temp3=Speed;
//                  
//                    if(rel_speed<temp3)
//                    {
//                       if(sp_flg)
//                       {
//                         sp_flg=0;
//                         rel_speed+=2;
//                       }
//                       if(rel_speed>temp3)
//                         rel_speed=temp3;  
//                    }
//                    else
//                    {
//                       rel_speed=temp3;
//                    }
                      rel_speed=Speed;
                   }
                   else
                      rel_speed= speed_485;
                    if(car_dt)
                    {
                      if(rel_speed>25)
                          rel_speed=25;
                    }
                    else
                    {
                        if(car_stat.data.SanSu==1)
                        {
                           if(rel_speed>50)
                             rel_speed=50;
                        }
                        else if(car_stat.data.SanSu==2)
                        {
                            if(rel_speed>60)
                              rel_speed=60; 
                        }
                        else if(car_stat.data.SanSu==3)
                        {
                           if(rel_speed>70)
                              rel_speed=70;
                        }
                        if(GuoBiao_mod)
                        {
                          if(Speed>25)
                             Speed=25;
                        } 
                    }
                     show_speed(rel_speed);
                  if(P_STA)
                  {
                    KM_H_OFF;
                    Led2_data[4]&=~(0x7f);
	                Led2_data[5]&=~(0x7f);
	                Led2_data[6]&=~(0x7f);
	                Led2_data[7]&=~(0x7f);
	                Led2_data[6]|=0x73;
	                Led2_data[7]|=0x73;
                  }
                  else
                  {
                    run_cartoon();
                    KM_H_ON;
                  }
                  // ready 逻辑
               
                  if((rel_speed==0)&&(temp.STA.ECU==0)&&(temp.STA.motor==0)&&(temp.STA.ZhuanBa==0)&&(temp.STA.ZhiDong==0))
                  {
                    
                       Led2_data[3]|=(0x3f);
                       Led2_data[8]=0xff;
//                      if(car_dt)
//                      {
//                          if(P_STA)
//                          {
//                             Led2_data[3]&=~(0x3f);
//                             Led2_data[8]=0;   
//                          }
//                      }
                  }
                  else
                  {
                    Led2_data[3]&=~(0x3f);
                    Led2_data[8]=0;                      
                  }

                  if(R_mod)
                     show_Rmod();
                  if(card_learn)
                     show_card(CARD_INDEX+1);
                  if(car_dt)   //电自模式下
                  {
                      if(!GuoBiao_mod)
                      {
                       Led2_data[13]=0xff;                      
                       show_gear(car_stat.data.SanSu);
                      }
                      else   //国标模式不显示挡位
                      {
                        Led2_data[12]&=~(0x7f);
                        Led2_data[13]=0x00;                      
                      }
                  }
                  else
                  {
                     Led2_data[13]=0xff;
                     show_gear(car_stat.data.SanSu);
                  }
                 if(cut485<401)
                 { 
                    temp4=soc_485;
                    temp4=temp4*100;
                    temp4=(temp4/125);
                    if((temp4%10)>0)
                    {
                      temp4=temp4/10;
                      temp4+=1;
                    }
                    else
                      temp4=temp4/10;
                    power_level_display(temp4); 
                 }
                  else
                  {
                    power_level_display(Soc_tmp);
//                    car_dt=0;
                  }                      
                  if(half_time==1)
                  {
                    half_time=0;
                   if(yxt_rcv1)
                   {
                    if(cut485>400)
                     lc_add( );
                   }
                  }
                  if(data_chage)
                  {
                    V_OFF;
                    if(start_time<20)
                    {
                       if(cut485<401)
                        {
                          show_voltage((v485*10),0);
                          last_485=v485;
                        }
                        else
                        {
                            if(V60_STA)
                            {
                              show_voltage(600,0);
                            }
                            else if(V72_STA)
                            {
                              show_voltage(720,0);
                            }
                            else
                            {
                              show_voltage(480,0);
                            }
                        }       
                    }
                    else
                    {
                        KM_ON;
                        if(v485!=last_485||yxt_vch)
                        {
                            yxt_vch=0;
                           start_time=0; 
                        }
                        else
                        start_time=21;
                         if(odocut)
                         {
                            ODO_ON;
                           odocut--;
                           TRIP_OFF;
                           show_mileage((ODO_lc/10),0);
                         }
                         else
                         {
                            TRIP_ON;
                            if(cut485<401)
                             show_mileage(trip_485,1);
                            else
                             show_mileage((Trip_lc/10),1);
                            ODO_OFF;
                         }
                     }
                    if(cut485<401)
                    {
                      BaiFenHao_on;
                      show_zuoxia2_data(soc_485,1);
                    }
                    else
                    {
                      MaoHao_off;
                      BaiFenHao_off;
                      Led_data[1]&=~(0x7f);
                      Led_data[0]&=~(0x7f);
                      Led_data[3]&=~(0x7f);
                      Led_data[2]&=~(0x7f);
                    }                        
                  }
                  else if(!data_chage)
                  {
                     BaiFenHao_off;
                     V_OFF;
                     TRIP_OFF;
                     if(cut485<401)
                     {
                       time_dis( );
                       if(!odocut)
                       {
                         show_voltage(voltage_485,1); 
                         TRIP_OFF;
                        
                       }
                     }
                     else
                     {
                     
                        show_voltage(voltage,1);
                     
                      Led_data[1]&=~(0x7f);
                      Led_data[0]&=~(0x7f);
                      Led_data[3]&=~(0x7f);
                      Led_data[2]&=~(0x7f);
                      
                     }
                      if(!odocut)
                         KM_OFF;
                      else
                         KM_ON;                     
                  }
                  if((Soc_tmp<1)&&(cut485>400))
                  {
                     if(led_change)
                     { 
                        Led_data[13]|=(0x01<<7);
                        QianYa_on;
                     }
                     else
                     {
                         QianYa_off;
                         Led_data[13]&=~(0x01<<7);
                     }
                  }
                  else if((soc_485<5)&&(cut485<401))
                  {
                    if(led_change)
                     { 
                        Led_data[13]|=(0x01<<7);
                        QianYa_on;
                     }
                     else
                     {
                         QianYa_off;
                         Led_data[13]&=~(0x01<<7);
                     }
                  }
                  else
                  {                      
                    QianYa_off;
                    Led_data[13]&=~(0x01<<7);
                  }
				}
				break;
			}
           
		}
		else
		{
            logov=0;
            Trip_lc=0;
            logov=0;
            run_step=0;
            start_step=0;
            temp.data=0;
            odocut=360;
            for(u8 i=0;i<4;i++)
            car_stat.buf[i]=0;
			for(u8 i=0;i<16;i++)
			{
				Led_data[i]=0;
				Led2_data[i]=0;
			}
		 }
		   time_cout( );
           nfc_loop( );
           Led2_data[1]=temp.data;
           led_step( );
           key_task_10ms ( );
           Cnt_10ms = 0;
		 }
         chage_pwm(logov);
         recv_deal();
          
    }
}
