/*
名称：tim.h
功能：定时器头文件
日期：20151119
*/

#ifndef _GD32F_TIM_H
#define _GD32F_TIM_H

#include "type.h"
extern u16 logov;
void tim_init (void);
void GTIM_INIT(void);
void chage_pwm(u16 vlue);
#endif

//结束
