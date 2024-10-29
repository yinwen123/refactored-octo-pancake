


#include "inc.h"

u32 Cnt_learn_tim = 0,Cnt_init_tim = 3000;
u8 Cnt_acc_num = 0,Key_status = 0,M_key = 0,ZZ_cuth,ZZ_cutl,card_learn,last_type;
/*
void learn_check (void)
{
    static u32 cnt_up = 0,cnt_long = 0;
	if(Start_type)
	{
	    return;
	}
	if(KEY_LEARN)
	{
		cnt_up ++;
		if(cnt_up > 9)
		{
			cnt_up = 0;
			cnt_long = 0;
			RF_learn = 0;
		}
	}
	else if(cnt_long < 10)
	{
		cnt_up = 0;
		cnt_long ++;
		if(cnt_long == 10)
		{
			RF_learn = 1;
			MUSIC_T(1);
		}
	}
}
*/
void acc_check (void)	//µçÃÅ¼ì²â
{
	static u32 cnt_up = 20,cnt_long = 0,cnt_wait = 0,cnt_upzz = 0,cnt_longzz = 0,zz_flag,zzl_cut;
	
	if(ACC_IN)
	{
		cnt_up = 0;
		if(cnt_long < 10)
		{
			cnt_long ++;
			if(cnt_long == 10)
			{
               if(Start_type==0)
               {
                 Start_type = START_TYPE_ACC;
                  last_type=START_TYPE_ACC;
               }
				
                 if(Cnt_acc_num>3)
                 {
                    Cnt_acc_num = 0;
                    Cnt_learn_tim = 0;
                    if(Cnt_init_tim)
                    {
                       set_mode(255);
                       card_learn=1;
                      //MUSIC_T(3);
                    } 
                 }
                 
			}
		}
	}
	else
	{
		cnt_up ++;
		if(cnt_up > 20)
		{
            card_learn=0;
            Tim_write=0;
			cnt_up = 0;
			cnt_long = 0;
//           if(Start_type!=START_TYPE_NFC)
			Start_type = 0;
            if(last_type>0)
            {
               Cnt_acc_num++;
               last_type=0; 
            }
		}
	}
    if(run_step==0)
        return;
        
     if(ZZ_IN == 0)
    {
        ZZ_cutl=0;
        zzl_cut=0;
        ZZ_cuth++;
        if(ZZ_cuth>4)
        {
          ZZ_cuth=9;
          zz_flag=1;
          turn_left_on;
        }
    }
    else
    {
        ZZ_cuth=0;
        ZZ_cutl++;
        if(zz_flag)
        {
            zz_flag=0;
            zzl_cut=4;
            
        }
        else
          zzl_cut=1; 
       if(ZZ_cutl>zzl_cut)
        {
            ZZ_cutl=zzl_cut+1;
           turn_left_off;
        }
    }
	
}

void xw_check (void)
{
	static u32 cnt_long = 0,tims = 0;
	static u32 xw_io;
	

}

void quake_check (void)
{

}
void led_check (void)   //µÆ¹â¼ì²â
{
    static u32
        cnt_upzz = 0,cnt_longzz = 0,
        cnt_upyz = 0,cnt_longyz = 0,
		cnt_upskd = 0,cnt_longskd = 0,
		cnt_upjgd = 0,cnt_longjgd = 0,
        cnt_updd = 0,cnt_longdd = 0;
	
	if(Nfc_af < 4)
	{
		return;
	}
	
    if(Start_type == 0||run_step==0)   //2024.9.21
    {
        cnt_longzz = 0;
        cnt_longyz = 0;
        cnt_longdd = 0;
		//cnt_longlb = 0;
		cnt_longskd = 0;
		//Led_flag &= 0xcf;
        return;
    }

   

    if(YZ_IN == 0)
    {
        cnt_upyz = 0;
        if(cnt_longyz < 2)
        {
            cnt_longyz ++;
            if(cnt_longyz == 2)
            {
               turn_right_on;
            }
        }
    }
    else
    {
        cnt_upyz ++;
        if(cnt_upyz > 1)
        {
            cnt_upyz = 0;
            cnt_longyz = 0;
            turn_right_off;
        }
    }

    if(DD_IN == 0)
    {
        cnt_longdd=0;
        cnt_updd ++;
        if(cnt_updd>2)
        {
           cnt_updd=3;
               
               DD_ON;
        }
    }
    else
    {
        cnt_longdd++;
        cnt_updd=0;
        if(cnt_longdd> 2)
        {
           cnt_longdd =3; 
           DD_OFF;
        }
    }

	if(SKD_IN == 0)
    {
        cnt_upskd++;
        cnt_longskd=0;
        if(cnt_upskd > 2)
        {
            cnt_upskd=3;
             SKD_ON;
            Led_level=5;
            if(Start_type)
            logov=300;
            else
            logov=0; 
        }
    }
    else 
    {
        cnt_longskd++;
        cnt_upskd=0;;
        if(cnt_longskd> 2)
        {
            cnt_longskd =3;
            Led_level=12;
            if(Start_type)
            logov=1000;
            
            
            SKD_OFF;
        }
    }
}

void key_task_10ms (void)
{
    if(Cnt_init_tim)
    {
        Cnt_init_tim --;
    }
    
	//learn_check ();
      led_check ();
	  acc_check ();
	//xw_check ();
}
