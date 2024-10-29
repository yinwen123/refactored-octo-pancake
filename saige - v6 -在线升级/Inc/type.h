/*********************************************************
*Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
*�ļ���:  type.h
*��  ��:  ESM Application Team 
*��  ��:  V1.01
*��  ��:  2016/11/01
*��  ��:  type define
*��  ע:  ������HRSDK-GDB-8P506
          ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
**********************************************************/
#ifndef __TYPE_H__
#define __TYPE_H__

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define  __align(x) __attribute__((aligned(x)))
#define  __intstack __attribute__((interrupt("WCH-Interrupt-fast")))

/***************BIG DATA*******************/

typedef union
{ 
    
    u8 Byte[2];
	
	u16 SHORT;
    
} big16;

typedef union
{ 
    
    u8 Byte[4];

	u32 Word;
    
} big32;
/***************BIG DATA END*******************/

#endif
