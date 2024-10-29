#include "inc.h"

USER_REG User_data;

u8 Status = 0,Init_mode = 0xff;
u16 Sta_addr = 0,Odo_addr = 0;
u32 ODO_lc = 0;
extern uint16_t ADC_ref;
void flash_earse(u32 addr)	//page < 128 addr 512整数倍
{
	u32 eaddr = addr;
	CW_FLASH->CR1 = 0x5A5A0002;

	*(u8*)eaddr = 0x55;
	while(CW_FLASH->CR1 & 0x20);

	CW_FLASH->CR1 = 0x5A5A0000;
}

void flash_write_word(u32 addr,u32 *data,u16 len)
{
	u32 eaddr = addr;
	CW_FLASH->CR1 = 0x5A5A0001;
	for(u16 i = 0;i < len;i ++)
	{
		*(u32*)eaddr = data[i];
		while(CW_FLASH->CR1 & 0x20);
		eaddr += 4;
	}
	CW_FLASH->CR1 = 0x5A5A0000;
}

void flash_write_byte(u32 addr,u8 data)
{
	CW_FLASH->CR1 = 0x5A5A0001;
	*(u8*)addr = data;
	while(CW_FLASH->CR1 & 0x20);
	CW_FLASH->CR1 = 0x5A5A0000;
}

void flash_init(void)
{
    CW_FLASH->PAGELOCK = 0x5A5A03FF;	//40页可写 一个bit代表4页每页512字节
	
	u32 i = 0;
	
	//for(i = 0;i < 10;i ++)
	{
		//Set_card[i] = *(volatile u8*)(0x12660 + i);
	}
	/*
	*(u32*)&Set_card[0] = *(u32*)0x10084C;
	*(u16*)&Set_card[4] = *(u16*)0x100858;
	*(u16*)&Set_card[6] = *(u16*)0x100854;
	Set_card[8] = *(u8*)0x100850;
	Set_card[9] = *(u8*)0x10085c;
	*/
	for(i = 0;i < 16;i ++)
	{
		User_data.Word[i] = *(u32*)(FLASH_UID_BASE + (i << 2));
	}
	
	if(UD_LM > 1)
	{
		UD_LM = 0;
	}
	if(UD_GB > 1)
	{
		UD_GB = 0;
	}
	
	if(CARD_LEVEL > 8)
	{
		CARD_LEVEL = 2;
	}
	
	if((UD_LEVEL < 9) || (UD_LEVEL > 99))
	{
		UD_LEVEL = 19;
	}
	
	RF_addr = (u32*)&User_data.Word[11];
	
	Init_mode = *(u8*)0x07f0;
	ADC_ref = *(u16*)0x07f2;

	//***********************************//状态获取
    /*
	u8 *p = (u8*)FLASH_STA_BASE;
	for(i = 0;i < 512;i ++)
	{
		if(p[i] == 0xff)
		{
			break;
		}
	}
	if(i == 0)
	{
		Sta_addr = 512;
		Status = STATE_UNLOCK;
	}
	else
	{
		Sta_addr = i - 1;
		Status = p[Sta_addr];
	}	
	
	if(Status == STATE_START)
	{
		Status = STATE_UNLOCK;
	}
	*/
	//***********************************//里程获取
	u32 *pl = (u32*)FLASH_ODO_BASE;
	for(i = 0;i < 128;i ++)
	{
		if(pl[i] == 0xffffffff)
		{
			break;
		}
	}
	if(i == 0)
	{
		Odo_addr = 512;
		ODO_lc = 0;
	}
	else
	{
		u8 t = i - 1;
		Odo_addr = t << 2;
		ODO_lc = pl[t];
	}
    
}

u8 temp1;
void flash_adc_updata(void)
{
    temp1= *(u8*)0x07f0;
	u32 t = ADC_ref;
	t <<= 16;
	t |= Init_mode;
	flash_write_word(0x07f0,&t,1);
    temp1= *(u8*)0x07f0;
}
//  app 0800
//     07f0
void flash_odo_updata(void)
{
	if(ODO_lc > 0x999998)
	{
		return;
	}
	
	if(ODO_lc == 0)
	{
		Odo_addr = 512;
	}
	
	Odo_addr += 4;
	if(Odo_addr > 511)
	{
		Odo_addr = 0;
		flash_earse(FLASH_ODO_BASE);
	}
	flash_write_word(FLASH_ODO_BASE + Odo_addr,(u32*)&ODO_lc,1);
}

void flash_sta_updata(u8 sta)
{
	if(Status == sta)
	{
		return;
	}
	Status = sta;
	Sta_addr ++;
	if(Sta_addr > 511)
	{
		Sta_addr = 0;
		flash_earse(FLASH_STA_BASE);
	}
	flash_write_byte(FLASH_STA_BASE + Sta_addr,Status);
}

void flash_user_updata(void)
{
	flash_earse(FLASH_UID_BASE);
	flash_write_word(FLASH_UID_BASE,(u32*)&User_data,16);
}


