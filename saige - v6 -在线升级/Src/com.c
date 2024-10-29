/*
	名称：key.c
	作者：LiGuo.Li
	功能描述：开关函数
	创建日期：2016.9.2
	修改日期：2016.9.2
	版本：V1.0
	功能修订日志：
	版本      日期      修订内容
	V1.0    2016.9.2	创建文件
*/

#include"inc.h"

#define SP_REF 191  //224
#define SPT_REF 3000    //SP_REF * 18.43

u8 Speed = 0,Speedt = 0,Wf = 0,BLDC_Sta[11];

u16 Cnt_hall = 0,Trip_lc = 0,Com_tout = 0;
uint16_t m_cut;

stat_t car_stat;
uint16_t timeover;
uint8_t yxt_rcv,yxt_rcv1,car_dt;
uint8_t P_STA,V60_STA,V72_STA;
void speedope(void)
{
	/*
    //C=SP_REF / 256 * 2359 / 32768 * 65536     (SP_REF / 256) * 4718    SP_REF * 18.43
    u32 trip_t = (Cnt_hall * SPT_REF); //0.5s 行驶里程  1cm
    Speed = trip_t >> 16;     // km/H

    if(Speed > 25)
    {
        Speed = 25;
    }
	*/
	u32 speed_t = Cnt_hall;    //0.5s 霍尔个数

    //(0.5s里程常量)C = (3.14圆周率*256整数运算放大倍数) * D(轮胎直径cm) / 级对 / 6单级对换相个数;   D=37.4cm,  级对=26,  C=193,
    u32 trip_t = 0;// = (speed_t * Speed_opt) >> 8; //0.5s 行驶里程  1cm

    u32 s_t;
	/*
    if(Led_f.SCM)
    {
        s_t = (speed_t * S_ct) >> 8;
    }
    else
    {
        s_t = (speed_t * SP_T) >> 8;
        trip_t = s_t;
    }
	*/
	s_t = (speed_t * SP_REF) >> 8;
    trip_t = s_t;

    //(0.5s时速常量)K = 72=(1h=3600s / 0.5s / 100cm) * 32.768(方便运算Km，放大32.768K倍);   K=2359
    s_t *= 2359; // *32768  m/H
    Speed = s_t >> 15;     // km/H

//	if(Speed > 25)
//	{
//		Speed = 25;
//	}
   

	Speedt = Speed;
}

void lc_add(void)
{
    static u16 lc_t = 0;
	
//	if(Led_f.SCM) return;
   //调速状态，p档状态返回

    //(0.5s里程常量)C = (3.14圆周率* ) * D(轮胎直径cm) / 级对 / 6单级对换相个数;   D=37.4cm,  级对=26,  C=193,
    u32 trip_t = (Cnt_hall * SP_REF) >> 8; //0.5s 行驶里程  1cm

    if(P_STA)
    {
        return;
    }
	
	trip_t *= 85;
    trip_t /= 100;

    lc_t += trip_t;     //0.5秒 行驶里程累计 1cm
    if(lc_t > 9999) //0.1Km 计一次里程
    {
		ODO_lc ++;
		
		u32 t = 0xf,t1 = 0x9,t2 = 0x10;
		for(u32 i = 0;i < 5;i ++)
		{
			if((ODO_lc & t) > t1)
			{
				ODO_lc &= ~t;
				ODO_lc += t2;
				t <<= 4;
				t1 <<= 4;
				t2 <<= 4;
			}
			else break;
		}
		if(ODO_lc > 0x999999)
		{
			ODO_lc = 0x999999;
		}
		
		Trip_lc ++;
		
		t = 0xf;t1 = 0x9;t2 = 0x10;
		for(u32 i = 0;i < 3;i ++)
		{
			if((Trip_lc & t) > t1)
			{
				Trip_lc &= ~t;
				Trip_lc += t2;
				t <<= 4;
				t1 <<= 4;
				t2 <<= 4;
			}
			else break;
		}
		if(Trip_lc > 0x9999)
		{
			Trip_lc = 0x9999;
		}
        
        Wf = 1;
        lc_t -= 10000;  //不足100米留待下次累计
    }
}


typedef union
{
    u32 Word;
    struct
    {
        u8 Rf_tim;
        u8 Rf_high;
        u8 Rf_bit;
        u8 Rf_byte;
    }Bytes;
}RF_REG;
uint8_t pass;
u8 yxt_vch,yxt_v,motor_STA;
void Decoder_data(uint8_t*p)
{

   for(u8 i=0;i<11;i++)
   {
     BLDC_Sta[i]=p[i]; 
   }
   Cnt_hall=(BLDC_Sta[7]<<8)|BLDC_Sta[8];
   if(BLDC_Sta[4]&(0x01<<6))
      motor_STA=1;
   else
      motor_STA=0;
  if(cut485>400)    
  {
      if(((BLDC_Sta[2]&0x0C)>>2)==0x03||((BLDC_Sta[2]&0x0C)>>2)==0x00)
        car_dt=0;
     else
        car_dt=1; 
     V60_STA=((BLDC_Sta[10]&0x4));
     V72_STA =((BLDC_Sta[10]&0x10));
     P_STA=((BLDC_Sta[2] & 0x02));
     if(yxt_v!=BLDC_Sta[10])
     {
        yxt_v=BLDC_Sta[10];
        yxt_vch=1;
     }
    for(u8 i=0;i<9;i++)
    {
	 car_stat.buf[i]=BLDC_Sta[i+2]; 
    }
    if(car_stat.data.SanSu==0x00)
     GuoBiao_mod=1;
    else
     GuoBiao_mod=0; 
  }
}

__align(4) RF_REG RF_reg = {0};
 
void com_task_50us (void)  //捕获码值
{
    static __align(4)u8 data[14],check = 0;
    static u32 rf_io;
	
    if(COM_IN != rf_io)
    {
        rf_io = COM_IN;
        if(COM_IN == 0)
        {
            if(RF_reg.Bytes.Rf_tim == 65)
            {
                RF_reg.Word = 0;
                check = 0;
                return;
            }
            if(RF_reg.Bytes.Rf_tim < 25)
            {
                RF_reg.Word = 0;
                check = 0;
                return;
            }
            if(RF_reg.Bytes.Rf_high < 5)
            {
                RF_reg.Word = 0;
                check = 0;
                return;
            }
            RF_reg.Bytes.Rf_tim -= RF_reg.Bytes.Rf_high;
            if(RF_reg.Bytes.Rf_tim < 5)
            {
                RF_reg.Word = 0;
                check = 0;
                return;
            }
            data[RF_reg.Bytes.Rf_byte] <<= 1;
            if(RF_reg.Bytes.Rf_tim > RF_reg.Bytes.Rf_high)
            {
                data[RF_reg.Bytes.Rf_byte] |= 1;
            }
            RF_reg.Bytes.Rf_tim = 0;
            RF_reg.Bytes.Rf_bit ++;
            if(RF_reg.Bytes.Rf_bit > 95)
            {
                if(check == data[RF_reg.Bytes.Rf_byte])
                {
                    if(data[0] == 0x08)
                    {
//                      com_decode(data,0);
                        Decoder_data(data);
                        speedope();
                        yxt_rcv=1;
                                              
                    }
                }
                if(Wf)
                {
                    Wf = 0;
                    flash_odo_updata();    //写FLASH
                }

                RF_reg.Word = 0;
            }
						 else if((RF_reg.Bytes.Rf_bit & 7) == 0)
            {
                check ^= data[RF_reg.Bytes.Rf_byte];
                RF_reg.Bytes.Rf_byte ++;
            }
           
        }
        else
        {
            RF_reg.Bytes.Rf_high = RF_reg.Bytes.Rf_tim;
        }
    }
    else if(RF_reg.Bytes.Rf_tim < 65)
    {
        RF_reg.Bytes.Rf_tim ++;
    }
}

