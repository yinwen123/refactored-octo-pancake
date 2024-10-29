/*
名称：fdq.c
功能：防盗器源文件
日期：20170916
*/

#include "inc.h"
extern  u16 Cnt_unlock,Cnt_lock,Cnt_alm_tim,Cnt_alm_first;
extern u8 Start_type;
u8  Alm_type = 0,SG_type,Alm_state = 0;
void elock (void)
{
	flash_sta_updata(STATE_LOCK);
	Cnt_lock = ALARM_WAIT;
	Cnt_unlock = 0;
	Cnt_alm_first = 0;
}

void eunlock (void)
{
    flash_sta_updata(STATE_UNLOCK);
	Cnt_unlock = 0;
}

void estart (u8 type)
{
    flash_sta_updata(STATE_START);
	Start_type = type;
	Cnt_unlock = 0;
}