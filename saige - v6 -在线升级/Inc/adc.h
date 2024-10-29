/*
	名称：adc.h
	作者：LiGuo.Li
	功能描述：ADC定义头文件
	创建日期：2017.6.7
	修改日期：2017.6.7
	版本：V1.0
	功能修订日志：
	版本      日期      修订内容
	V1.0    2017.6.7	创建文件
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
