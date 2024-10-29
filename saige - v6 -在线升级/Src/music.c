


#include	"inc.h"

volatile u8 YIP = 0;

volatile u16 Music_cycle = 0,Cnt_dlb_tim = 0;
u8 Start_type = 0;     //当ACC上电则将该变量置1，使能music,从查看结果看并未使用
__align(4) volatile MU_REG0 Mu_reg0 = {0};
__align(4) volatile MU_REG1 Mu_reg1 = {0};
#define ALARM_WAIT 300
u16 Cnt_unlock,Cnt_lock = ALARM_WAIT,Cnt_alm_tim = 0,Cnt_alm_first = 0;
void music_end(void)
{
    M_TIE = 0;
    CW_BTIM1->BCR_f.EN = 0;
    CW_BTIM1->CNT = 0;
    MUSIC_PORT_STOP;
    Mu_reg0.Word = 0;
    Mu_reg1.Word = 0;
}

void music_start(void)
{
	CW_BTIM1->BCR_f.EN = 1; 
	M_TIE = 1;
	MUSIC_PORT_OUT;
}

void music_isr (void)
{
    Cnt_lock = ALARM_WAIT;
    switch(YIP)
    {
        case 1:
            if(Mu_reg1.Bytes.Music_wait == 0)
            {
                MUSIC_PORT_XOR;
            }
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Mu_reg1.Bytes.Music_wait)
                {
                    if(Mu_reg0.Bytes.Hcycle)
                    {
                        Mu_reg0.Bytes.Hcycle --;
                        Mu_reg0.Bytes.Cycle = 18;
                    }
                    else
                    {
                        Mu_reg1.Bytes.Music_wait = 0;
                        MUSIC_PORT_OUT;
                    }
                }
                else
                {
                    if(Mu_reg1.Bytes.Music_pos)
                    {
                        if(Music_cycle < 142)
                        {
                            Mu_reg0.Bytes.Cycle = 5;
                            Music_cycle ++;
                            M_CYCLE( Music_cycle );
                        }
                        else
                        {
                            Mu_reg1.Bytes.Music_pos = 0;
                            if(Mu_reg0.Bytes.Num == 0)
                            {
                                if(Mu_reg1.Bytes.Music_alm)
                                {
                                    YIP = 2;
                                    M_CYCLE( 72 );
                                    Mu_reg0.Bytes.Num = 9;
                                    Mu_reg0.Bytes.Cycle = 8;
                                }
                                else
                                {
                                    music_end();
                                }
                            }
                            else
                            {
                                MUSIC_PORT_STOP;
                                Mu_reg0.Bytes.Num --;
                                Mu_reg0.Bytes.Hcycle = Mu_reg0.Bytes.Wait_Cycle;
                                Mu_reg0.Bytes.Cycle = 18;
                                Mu_reg1.Bytes.Music_wait = 1;
                            }
                        }
                    }
                    else
                    {
                        if(Music_cycle > 83)
                        {
                            Mu_reg0.Bytes.Cycle = 5;
                            Music_cycle --;
                            M_CYCLE( Music_cycle );
                        }
                        else
                        {
                            Mu_reg1.Bytes.Music_pos = 1;
                        }
                    }
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        case 2:
            MUSIC_PORT_XOR;
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Music_cycle < 213)
                {
                    Mu_reg0.Bytes.Cycle = 8;
                    Music_cycle ++;
                    M_CYCLE( Music_cycle );
                }
                else
                {
                    if(Mu_reg0.Bytes.Num == 0)
                    {
                        YIP = 3;
                        M_CYCLE( 75 );
                        Mu_reg0.Bytes.Num = 3;
                        Mu_reg0.Bytes.Hcycle = 202;
                        Mu_reg0.Bytes.Cycle = 16;
                        Mu_reg1.Bytes.Music_pos = 0;
                    }
                    else
                    {
                        Mu_reg0.Bytes.Num --;
                        M_CYCLE( 72 );
                        Mu_reg0.Bytes.Cycle = 8;
                    }
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        case 3:
            MUSIC_PORT_XOR;
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Mu_reg1.Bytes.Music_pos == 0)
                {
                    if(Mu_reg0.Bytes.Hcycle)
                    {
                        Mu_reg0.Bytes.Hcycle --;
                        Mu_reg0.Bytes.Cycle = 16;
                    }
                    else
                    {
                        M_CYCLE( 171 );
                        Mu_reg0.Bytes.Hcycle = 84;
                        Mu_reg0.Bytes.Cycle = 16;
                        Mu_reg1.Bytes.Music_pos = 1;
                    }
                }
                else
                {
                    if(Mu_reg0.Bytes.Hcycle)
                    {
                        Mu_reg0.Bytes.Hcycle --;
                        Mu_reg0.Bytes.Cycle = 16;
                    }
                    else if(Mu_reg0.Bytes.Num)
                    {
                        Mu_reg0.Bytes.Num --;
                        M_CYCLE( 75 );
                        Mu_reg0.Bytes.Hcycle = 202;
                        Mu_reg0.Bytes.Cycle = 16;
                        Mu_reg1.Bytes.Music_pos = 0;
                    }
                    else
                    {
                        YIP = 4;
                        M_CYCLE( 227 );
                        Mu_reg0.Bytes.Num = 3;
                        Mu_reg0.Bytes.Cycle = 15;
                    }
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        case 4:
            MUSIC_PORT_XOR;
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Music_cycle > 77)
                {
                    Mu_reg0.Bytes.Cycle = 15;
                    Music_cycle --;
                    M_CYCLE( Music_cycle );
                }
                else
                {
                    if(Mu_reg0.Bytes.Num == 0)
                    {
                        YIP = 5;
                        M_CYCLE( 58 );
                        Mu_reg0.Bytes.Cycle = 14;
                        Mu_reg0.Bytes.Hcycle = 126;
                        Mu_reg0.Bytes.Num = 3;
                        Mu_reg1.Bytes.Music_wait = 0;
                    }
                    else
                    {
                        Mu_reg0.Bytes.Num --;
                        M_CYCLE( 227 );
                        Mu_reg0.Bytes.Cycle = 15;
                    }
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        case 5:
            if(Mu_reg1.Bytes.Music_wait == 0)
            {
                MUSIC_PORT_XOR;
            }
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Mu_reg1.Bytes.Music_wait)
                {
                    if(Mu_reg0.Bytes.Hcycle == 0)
                    {
                        M_CYCLE( 206 );
                        Mu_reg0.Bytes.Cycle = 15;
                    }
                    else
                    {
                        M_CYCLE( 63 );
                        Mu_reg0.Bytes.Cycle = 15;
                    }
                    MUSIC_PORT_OUT;
                    Mu_reg1.Bytes.Music_wait = 0;
                }
                else
                {
                    if(Mu_reg0.Bytes.Hcycle > 1)
                    {
                        Mu_reg0.Bytes.Hcycle --;
                        Mu_reg0.Bytes.Cycle = 0;
                        M_CYCLE( 206 );
                        Mu_reg1.Bytes.Music_wait = 1;
                        MUSIC_PORT_STOP;
                    }
                    else if(Mu_reg0.Bytes.Hcycle)
                    {
                        Mu_reg0.Bytes.Hcycle --;
                        Mu_reg0.Bytes.Cycle = 225;
                        M_CYCLE( 206 );
                        Mu_reg1.Bytes.Music_wait = 1;
                        MUSIC_PORT_STOP;
                    }
                    else
                    {
                        if(Mu_reg0.Bytes.Num)
                        {
                            M_CYCLE( 63 );
                            Mu_reg0.Bytes.Num --;
                            Mu_reg0.Bytes.Cycle = 15;
                            Mu_reg0.Bytes.Hcycle = 126;
                        }
                        else
                        {
                            YIP = 6;
                            M_CYCLE( 198 );
                            Mu_reg0.Bytes.Num = 9;
                            Mu_reg0.Bytes.Cycle = 4;
                        }
                    }
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        case 6:
            MUSIC_PORT_XOR;
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Music_cycle > 68)
                {
                    Mu_reg0.Bytes.Cycle = 4;
                    Music_cycle --;
                    M_CYCLE( Music_cycle );
                }
                else
                {
                    if(Mu_reg0.Bytes.Num == 0)
                    {
                        Mu_reg1.Bytes.Music_alm = 0;
                        music_end();
                    }
                    else
                    {
                        Mu_reg0.Bytes.Num --;
                        M_CYCLE( 198 );
                        Mu_reg0.Bytes.Cycle = 4;
                    }
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        case 7:
            MUSIC_PORT_XOR;
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Music_cycle == 552)
                {
                    Mu_reg0.Bytes.Cycle = 150;
                    M_CYCLE( 500 );
                }
                else if(Music_cycle == 500)
                {
                    Mu_reg0.Bytes.Cycle = 167;
                    M_CYCLE( 452 );
                }
                else
                {
                    music_end();
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        case 8:
            MUSIC_PORT_XOR;
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Music_cycle == 452)
                {
                    Mu_reg0.Bytes.Cycle = 150;
                    M_CYCLE( 500 );
                }
                else if(Music_cycle == 500)
                {
                    Mu_reg0.Bytes.Cycle = 136;
                    M_CYCLE( 552 );
                }
                else
                {
                    music_end();
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        case 9:
            MUSIC_PORT_XOR_ONE;
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Mu_reg0.Bytes.Hcycle == 0)
                {
                    music_end();
                }
                else
                {
                    Mu_reg0.Bytes.Hcycle --;
                    Mu_reg0.Bytes.Cycle = 255;
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        case 10:
            if(Mu_reg1.Bytes.Music_wait == 0)
            {
                MUSIC_PORT_XOR;
            }
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Mu_reg1.Bytes.Music_wait)
                {
                    Mu_reg1.Bytes.Music_wait = 0;
                    MUSIC_PORT_OUT;
                    Mu_reg0.Bytes.Cycle = 80;
                }
                else
                {
                    if(Mu_reg0.Bytes.Num == 0)
                    {
                        music_end();
                    }
                    else
                    {
                        MUSIC_PORT_STOP;
                        Mu_reg0.Bytes.Num --;
                        Mu_reg0.Bytes.Cycle = 100;
                        Mu_reg1.Bytes.Music_wait = 1;
                    }
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        case 11:
        {
            MUSIC_PORT_XOR;
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Start_type && Cnt_dlb_tim)
                {
                    M_CYCLE(75);
                    Mu_reg0.Bytes.Cycle = 20;
                }
                else
                {
                    music_end();
                }
            }
            else if(Mu_reg0.Bytes.Cycle == 10)
            {
                M_CYCLE(125);
                Mu_reg0.Bytes.Cycle --;
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        }
        case 12:
            if(Mu_reg1.Bytes.Music_wait == 0)
            {
                MUSIC_PORT_XOR;
            }
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Mu_reg1.Bytes.Music_wait)
                {
                    Mu_reg1.Bytes.Music_wait = 0;
                    Mu_reg0.Bytes.Cycle = 150;
                    MUSIC_PORT_OUT;
                }
                else if(Mu_reg0.Bytes.Num == 0)
                {
                    music_end();
                }
                else
                {
                    MUSIC_PORT_STOP;
                    Mu_reg0.Bytes.Num --;
                    Mu_reg0.Bytes.Cycle = 100;
                    Mu_reg1.Bytes.Music_wait = 1;
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        case 13:
        {
            MUSIC_PORT_XOR;
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Mu_reg0.Bytes.Num)
                {
                    Mu_reg0.Bytes.Num --;
                    Mu_reg0.Bytes.Cycle = 200;
                }
                else
                {
                    music_end();
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        }/*
		case 14: //寻车，报警
        {
            if(Mu_reg1.Bytes.Music_wait == 0)
            {
                MUSIC_PORT_XOR;
            }
            if(Mu_reg0.Bytes.Cycle == 0)
            {
                if(Mu_reg0.Bytes.Hcycle == 0)
                {
                    if(Mu_reg1.Bytes.Music_wait)
                    {
                        Mu_reg1.Bytes.Music_wait = 0;
                        MUSIC_PORT_OUT;
                        M_CYCLE(208);
                        Mu_reg0.Bytes.Cycle = 239;
                    }
                    else
                    {
                        if(Music_cycle == 208)
                        {
                            Mu_reg0.Bytes.Cycle = 200;
                            M_CYCLE(250);
                        }
                        else
                        {
                            if(Mu_reg0.Bytes.Num == 0)
                            {
                                if(Mu_reg1.Bytes.Music_pos == 0)
                                {
                                    music_end();
                                }
                                else
                                {
                                    Mu_reg1.Bytes.Music_pos --;
                                    MUSIC_PORT_STOP;
                                    Mu_reg0.Bytes.Cycle = 200;
                                    Mu_reg0.Bytes.Hcycle = 9;
                                    Mu_reg0.Bytes.Num = Mu_reg0.Bytes.Wait_Cycle;
                                    Mu_reg1.Bytes.Music_wait = 1;
                                }
                            }
                            else
                            {
                                Mu_reg0.Bytes.Num --;
                                MUSIC_PORT_STOP;
                                Mu_reg0.Bytes.Cycle = 20;
                                Mu_reg1.Bytes.Music_wait = 1;
                            }
                        }
                    }
                }
                else
                {
                    Mu_reg0.Bytes.Hcycle --;
                    Mu_reg0.Bytes.Cycle = 200;
                }
            }
            else
            {
                Mu_reg0.Bytes.Cycle --;
            }
            break;
        }*/
        default:
            music_end();
            break;
    }
}

void music_set (u8 mic)
{
    music_end();
    switch(mic)
    {
        case BJ_MUID:
			
            M_CYCLE( 142 );
            Mu_reg1.Bytes.Music_alm = 1;
            Mu_reg0.Bytes.Num = 5;
            YIP = 1;
            Mu_reg0.Bytes.Cycle = 5;
		/*
			M_CYCLE(208);
            Mu_reg0.Bytes.Num = 5;
            Mu_reg0.Bytes.Wait_Cycle = 5;
            Mu_reg1.Bytes.Music_pos = 6;
            YIP = 14;
            Mu_reg0.Bytes.Hcycle = 0;
            Mu_reg0.Bytes.Cycle = 239;
		*/
        break;
        case ST_MUID:
            M_CYCLE( 552 );
            YIP = 7;
            Mu_reg0.Bytes.Cycle = 136;
        break;
        case PD_MUID:
            M_CYCLE( 500 );
            YIP = 12;
            Mu_reg0.Bytes.Num = 0;
            Mu_reg0.Bytes.Cycle = 150;
        break;
        case PD1_MUID:
            M_CYCLE( 500 );
            YIP = 12;
            Mu_reg0.Bytes.Num = 1;
            Mu_reg0.Bytes.Cycle = 150;
        break;
        case CL_MUID:
            M_CYCLE( 452 );
            YIP = 8;
            Mu_reg0.Bytes.Cycle = 167;
        break;
        case YBJ_MUID:
			
            M_CYCLE( 142 );
            Mu_reg0.Bytes.Num = 4;
            YIP = 1;
            Mu_reg0.Bytes.Wait_Cycle = 120;
            Mu_reg0.Bytes.Hcycle = 120;
            Mu_reg0.Bytes.Cycle = 5;
		/*
			M_CYCLE(208);
            Mu_reg0.Bytes.Num = 5;
            Mu_reg0.Bytes.Wait_Cycle = 5;
            Mu_reg1.Bytes.Music_pos = 1;
            YIP = 14;
            Mu_reg0.Bytes.Hcycle = 0;
            Mu_reg0.Bytes.Cycle = 239;
		*/
        break;
        case GB_MUID:
            M_CYCLE( 250 );
            YIP = 9;
            Mu_reg0.Bytes.Hcycle = 5;
            Mu_reg0.Bytes.Cycle = 120;
        break;
        case SF_MUID:
            M_CYCLE( 142 );
            YIP = 1;
            Mu_reg0.Bytes.Cycle = 5;
        break;
        case JF_MUID:
            M_CYCLE( 142 );
            Mu_reg0.Bytes.Num = 1;
            YIP = 1;
            Mu_reg0.Bytes.Cycle = 5;
        break;
        case XC_MUID:
            M_CYCLE( 142 );
            Mu_reg0.Bytes.Num = 15;
            YIP = 1;
            Mu_reg0.Bytes.Cycle = 5;
        break;
        case DLB_MUID:
            M_CYCLE( 75 );
            YIP = 11;
            Mu_reg0.Bytes.Cycle = 20;
        break;
        case OF_MUID: return;
        default:
            M_CYCLE( 522 );
            Mu_reg0.Bytes.Num = mic - 1;
            YIP = 10;
            Mu_reg0.Bytes.Cycle = 80;
        break;
    }
    music_start();
}

void BTIM1_IRQHandler(void)
{
    CW_BTIM1->ICR_f.OV = 0;
	
	music_isr();

    return;
}
