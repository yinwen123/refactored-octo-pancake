#ifndef _GP_FLASH_H_
#define _GP_FLASH_H_

#include "type.h"

#define FLASH_STA_BASE   	0x4800

#define FLASH_ODO_BASE  	0x4A00

#define FLASH_UID_BASE   	0x4C00

typedef union
{
    u32 Word[16];
	u16 Short[32];
    u8  Byte[64];
}USER_REG;

extern USER_REG  User_data;

extern u8 Status,Init_mode;
extern u16 Sta_addr,Odo_addr,ADC_ref;
 
extern u32 ODO_lc;

#define CARD_ID       User_data.Word	//1 - 10 Word  4-43 Byte

#define CARD_LEVEL    User_data.Byte[0]
#define CARD_INDEX    User_data.Byte[1]
#define CARD_LMODE    User_data.Byte[2] 

#define UD_RFADDR0	  User_data.Word[11]
#define UD_RFADDR1	  User_data.Word[12]	//44 - 51  Byte

#define UD_GB     	  User_data.Byte[52]
#define UD_LM     	  User_data.Byte[53]
#define UD_LEVEL      User_data.Byte[54]

#define UD_ADCREF	  ADC_ref
#define INIT_MODE     Init_mode


void flash_init(void);
void flash_earse(u32 addr);
void flash_write_page(u32 addr,u32 *data);

void flash_odo_updata(void);

void flash_sta_updata(u8 sta);

void flash_user_updata(void);

void flash_adc_updata(void);

#endif
