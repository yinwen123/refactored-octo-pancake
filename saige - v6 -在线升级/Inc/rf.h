/*
	���ƣ�init.h
	���ߣ�LiGuo.Li
	����������RF����ͷ�ļ�
	�������ڣ�2016.9.2
	�޸����ڣ�2016.9.2
	�汾��V1.0
	�����޶���־��
	�汾      ����      �޶�����
	V1.0    2016.9.2	�����ļ�
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
