/*
	名称：init.c
	作者：LiGuo.Li
	功能描述：程序初始化函数
	创建日期：2016.9.2
	修改日期：2016.9.2
	版本：V1.0
	功能修订日志：
	版本      日期      修订内容
	V1.0    2016.9.2	创建文件
*/

#include"inc.h"

u32 *RF_addr,RF_le_addr;
u16 RF_stop,RF_le_stop;
u8 RF_code,Learn_addr = 0,RF_learn = 0;

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

__align(4) RF_REG RFr_reg = {0};

void rf_task_50us (void)  //捕获码值
{
    //static u8 cnt_rf_bit,rf_high,cnt_rf;
    static u32 rf_data,rf_io;
	
    if(RF_IN != rf_io)
    {
        rf_io = RF_IN;
		if(RF_IN)
		{
		    if(RFr_reg.Bytes.Rf_tim == 40)
            {
                RFr_reg.Word = 0;
                return;
            }
            if(RFr_reg.Bytes.Rf_tim < 15)
            {
                RFr_reg.Word = 0;
                return;
            }
            if(RFr_reg.Bytes.Rf_high < 2)
            {
                RFr_reg.Word = 0;
                return;
            }
            RFr_reg.Bytes.Rf_tim -= RFr_reg.Bytes.Rf_high;
            if(RFr_reg.Bytes.Rf_tim < 2)
            {
                RFr_reg.Word = 0;
                return;
            }
            rf_data <<= 1;
            if(RFr_reg.Bytes.Rf_tim < RFr_reg.Bytes.Rf_high)
            {
                rf_data |= 1;
            }
            RFr_reg.Bytes.Rf_tim = 0;
            RFr_reg.Bytes.Rf_bit ++;
            if(RFr_reg.Bytes.Rf_bit > 23)
            {
                if(RF_learn == 0)
                {
                    if((rf_data & 0xFFFFF0) == RF_addr [0])
                    {
                        RF_stop = 15;
                        RF_code = rf_data & 0xF;
                    }
                    else if((rf_data & 0xFFFFF0) == RF_addr [1])
                    {
                        RF_stop = 15;
                        RF_code = rf_data & 0xF;
                    }
                }
                else
                {
                    if(RF_le_addr == (rf_data & 0xFFFFF0))
                    {
                        RF_le_stop = 15;
                    }
                    else if(RF_le_stop == 0)
                    {
                        RF_le_addr = (rf_data & 0xFFFFF0);
                    }
                }
                RFr_reg.Word = 0;
            }
        }
        else
        {
            RFr_reg.Bytes.Rf_high = RFr_reg.Bytes.Rf_tim;
        }
    }
    else if(RFr_reg.Bytes.Rf_tim < 40)
    {
        RFr_reg.Bytes.Rf_tim ++;
    }
}

void rf_task_10ms (void)
{
	 static u16 cnt_rf_long = 0,cnt_first = 0;
	static u8 rf_long = 0;
	if(cnt_first)
    {
        cnt_first --;
    }

	if(RF_stop)
	{
		RF_stop --;
		if(RF_stop == 0)       //短按
		{
			cnt_rf_long = 0;
			if(rf_long == 0)
			{
			    switch(RF_code)
			    {
                    case 1:
                    {
                        if(Start_type == 0)
                        {
                            if(cnt_first)
                            {
                                cnt_first = 0;
                                estart (START_TYPE_RF);
                                MUSIC_ST;
                            }
                            else
                            {
                                cnt_first = 100;
                            }
                        }
                        break;
                    }
                    case 2:
                    {
                        MUSIC_XC;
                        Alm_state = 2;
                        break;
                    }
                    case 4:
                    {
                        if(Start_type == 0)
                        {
                            elock ();
                            MUSIC_SF;
                        }
                        break;
                    }
                    case 8:
                    {
                        eunlock ();
                        MUSIC_JF;
                        Cnt_unlock = 1000;
                        break;
                    }
                }
			}
			else
			{
				rf_long = 0;
			}
		}
		else if(cnt_rf_long < 250)        //长按
		{
			cnt_rf_long ++;
			if(cnt_rf_long == 250)
			{
				cnt_rf_long = 0;
				switch(RF_code)
				{
					case 2:
					{
						switch(UD_LEVEL)
						{
							case 9: UD_LEVEL = 19; flash_user_updata();break;
							case 19: UD_LEVEL = 29; flash_user_updata();break;
							case 29: UD_LEVEL = 39; flash_user_updata();break;
							case 39: UD_LEVEL = 99; flash_user_updata();break;
							case 99: UD_LEVEL = 9; flash_user_updata();break;
							default:UD_LEVEL = 19; flash_user_updata();break;
						}
						break;
					}
                    case 4:
                    {
                        elock ();
                        MUSIC_SF;
                        break;
                    }
                    case 8:
                    {
                        eunlock ();
                        MUSIC_JF;
                        Cnt_unlock = 1000;
                        break;
                    }
				}
				rf_long = 1;
			}
		}
	}
 if(RF_learn)
	{
		if(RF_le_stop)
		{
			RF_le_stop --;
			if(RF_le_stop == 0)
			{
				if(Learn_addr < 2)
				{
					RF_addr[Learn_addr] = RF_le_addr;
					Learn_addr ++;
					if(Learn_addr == 2)
					{
						Learn_addr = 0;
					}
				}
				else
				{
					Learn_addr = 0;
					RF_addr[0] = RF_le_addr;
				}
				flash_user_updata();
				MUSIC_T(1);
			}
		}
	}
	else
	{
		RF_le_stop = 0;
	}
}
