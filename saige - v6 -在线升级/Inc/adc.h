/*
	���ƣ�adc.h
	���ߣ�LiGuo.Li
	����������ADC����ͷ�ļ�
	�������ڣ�2017.6.7
	�޸����ڣ�2017.6.7
	�汾��V1.0
	�����޶���־��
	�汾      ����      �޶�����
	V1.0    2017.6.7	�����ļ�
*/

#ifndef _GP_ADC_H
#define _GP_ADC_H

#include "type.h"

//extern u16 Adc_data[2];

extern u8 Soc_tmp,Soc;

void adc_init(void);

void adc_check(void);

extern u16 ADT;

#endif
