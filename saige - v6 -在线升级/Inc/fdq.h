/*
名称：fdq.h
功能：防盗器头文件
日期：20170916
*/

#ifndef _GP_FDQ_H_
#define _GP_FDQ_H_

#include "type.h"

#define ALARM_WAIT 300

/****************bit*******************/

extern u16 Cnt_unlock,Cnt_lock;

extern u8 Start_type,Alm_type,Alm_state,Speed_enable;

extern u8 SG_type;

void elock (void);

void eunlock (void);

void estart (u8 type);

void eclose (void);

void fdq_task_10ms (void);

void speedsend(void);

void speedcmd(void);

#endif

//结束
