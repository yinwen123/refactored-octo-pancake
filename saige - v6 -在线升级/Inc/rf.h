/*
	名称：init.h
	作者：LiGuo.Li
	功能描述：RF函数头文件
	创建日期：2016.9.2
	修改日期：2016.9.2
	版本：V1.0
	功能修订日志：
	版本      日期      修订内容
	V1.0    2016.9.2	创建文件
*/

#ifndef _GP_RF_H
#define _GP_RF_H

#include "type.h"

#define RFDA_H 40      //2ms
#define RFDA_L 12       //600us

void rf_task_50us (void);
void rf_task_10ms (void);

extern u32 *RF_addr;
extern u8 RF_learn;

#endif
