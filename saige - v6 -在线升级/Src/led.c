

#include "inc.h"

#define ET6930_MAX_SCAN_LINE  16


STA_T temp;
uint8_t Led_level=15;
uint8_t LED_HS=15;
uint8_t Led_data[16]={0};
uint8_t Led2_data[16]={0};
static uint8_t Nmb_buf[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6F};
void udelay(void)
{
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
}

void i2c_start(void)
{
	LED_SDA_H;
	udelay();
	LED_SCL_H;
	udelay();
	LED_SDA_L;
	udelay();
	LED_SCL_L;
}

void i2c_stop(void)
{
	LED_SDA_L;
	udelay();
	LED_SCL_H;
	udelay();
	LED_SDA_H;
}

void i2c_send(u8 da)
{
	u16 data = (da << 1);
	for(u32 i = 0;i < 9;i ++)
	{
		if(i == 8)
		{
			LED_SDA_I;
		}
		else if(data & 0x100)
		{
			LED_SDA_H;
		}
		else
		{
			LED_SDA_L;
		}
		data <<= 1;
		LED_SCL_H;
		udelay();
		LED_SCL_L;
	}
	LED_SDA_O;
}

//iic2
void i2c2_start(void)
{
	LED2_SDA_H;
	udelay();
	LED_SCL_H;
	udelay();
	LED2_SDA_L;
	udelay();
	LED_SCL_L;
}

void i2c2_stop(void)
{
	LED2_SDA_L;
	udelay();
	LED_SCL_H;
	udelay();
	LED2_SDA_H;
}

void i2c2_send(u8 da)
{
	u16 data = (da << 1);
	for(u32 i = 0;i < 9;i ++)
	{
		if(i == 8)
		{
			LED2_SDA_I;
		}
		else if(data & 0x100)
		{
			LED2_SDA_H;
		}
		else
		{
			LED2_SDA_L;
		}
		data <<= 1;
		LED_SCL_H;
		udelay();
		LED_SCL_L;
	}
	LED2_SDA_O;
}
//end
void show_speed(uint16_t data)   //速度
{
	uint8_t GeWei=0,ShiWei=0;
	ShiWei=data/10%10;
	GeWei=data%10;
	Led2_data[4]&=~(0x7f);
	Led2_data[5]&=~(0x7f);
	Led2_data[6]&=~(0x7f);
	Led2_data[7]&=~(0x7f);
	Led2_data[6]|=Nmb_buf[GeWei];
	Led2_data[7]|=Nmb_buf[GeWei];
	Led2_data[5]|=Nmb_buf[ShiWei];
	Led2_data[4]|=Nmb_buf[ShiWei];
}
void show_card(uint8_t data)
{
  Led2_data[4]&=~(0x7f);
  Led2_data[5]&=~(0x7f);
  Led2_data[6]&=~(0x7f);
  Led2_data[7]&=~(0x7f);
  Led2_data[6]|=Nmb_buf[data];
  Led2_data[7]|=Nmb_buf[data];
  Led2_data[5]|=0x38;
  Led2_data[4]|=0x38;
}
void show_gear(uint8_t data)     //挡位
{
	Led2_data[12]&=~(0x7f);
	Led2_data[12]|=Nmb_buf[data];
}
void show_mileage(uint32_t data,uint8_t mod)
{
  uint32_t ge,shi,bai,qian,wan;
  ge=data/1%10;
  shi=data/10%10;
  bai=data/100%10;
  qian=data/1000%10;
  wan=data/10000%10;
	Led_data[7]&=~(0x7f);
    Led_data[8]&=~(0x7f);
    Led_data[9]&=~(0x7f);
    Led_data[10]&=~(0x7f);
    Led_data[11]&=~(0x7f);
    if(mod==1)
    {
    if(wan>0)
	Led_data[7]|=Nmb_buf[wan];
    if(data>=1000)
	Led_data[8]|=Nmb_buf[qian];
    if(data>=100)
	Led_data[9]|=Nmb_buf[bai];
    if(data>=10)
	Led_data[10]|=Nmb_buf[shi];
	Led_data[11]|=Nmb_buf[ge];
   }
   else
   {
	Led_data[7]|=Nmb_buf[wan];
	Led_data[8]|=Nmb_buf[qian];
	Led_data[9]|=Nmb_buf[bai];
	Led_data[10]|=Nmb_buf[shi];
	Led_data[11]|=Nmb_buf[ge];
   }
}

void power_level_display(uint8_t level)
{
	uint8_t j=0,k=0;
	j=level;
	Led_data[14]=0;
	Led_data[13]&=~(0x03<<5);
    Led_data[14]|=0x01;
    Led_data[13]|=0x40;
    if(level>0)
    {
      if(level==8)
      {
        Led_data[13]|=0x01<<5;
        level=7;
      }
      for(uint8_t i=0;i<level;i++)
	    Led_data[14]|=0x01<<(i+1);
      
    }
	else
       Led_data[14]&=~(0x01<<1);
	//低电压闪烁
    
}
void show_zuoxia1_data(uint8_t data)
{
	uint8_t GeWei=0,ShiWei=0;
	ShiWei=data/10%10;
	GeWei=data%10;
	Led_data[3]&=~(0x7f);
	Led_data[2]&=~(0x7f);
	Led_data[3]|=Nmb_buf[ShiWei];
	Led_data[2]|=Nmb_buf[GeWei];
	
}
void show_zuoxia2_data(uint8_t data,uint8_t mod)
{
	uint8_t GeWei=0,ShiWei=0;
	ShiWei=data/10%10;
	GeWei=data%10;
    Led_data[3]&=~(0x7f);
	Led_data[2]&=~(0x7f);
	Led_data[1]&=~(0x7f);
	Led_data[0]&=~(0x7f);
    if(mod)
    {
    if(data>9)
	Led_data[1]|=Nmb_buf[ShiWei];
	Led_data[0]|=Nmb_buf[GeWei];
    }
    else
    {
     Led_data[1]|=Nmb_buf[ShiWei];
	 Led_data[0]|=Nmb_buf[GeWei];
    }
    if(data>99)
       Led_data[2]|=Nmb_buf[1];
}
void time_dis(void)
{
  Led_data[1]&=~(0x7f);
  Led_data[0]&=~(0x7f);
  Led_data[3]&=~(0x7f);
  Led_data[2]&=~(0x7f);
  
  show_zuoxia2_data(min,0);
  show_zuoxia1_data(hour);
  MaoHao_off;
 
}
void show_voltage(uint16_t data,uint8_t mod)
{
   uint8_t GeWei=0,ShiWei=0,xiaoshu;
   ShiWei=data/100%10;
   GeWei=data/10%10;
   xiaoshu=data%10;
   Led_data[11]&=~(0x7f);
   Led_data[10]&=~(0x7f);
   Led_data[9]&=~(0x7f);
   Led_data[7]&=~(0x7f);
   Led_data[8]&=~(0x7f);
   if(mod==1)     //显示小数
   {
   Led_data[11]|=0x3e;
   Led_data[10]|=Nmb_buf[xiaoshu];
   Led_data[9]|=0x08;
   Led_data[7]|=Nmb_buf[ShiWei];
   Led_data[8]|=Nmb_buf[GeWei];
   }
   else         //不显示小数
   {
     Led_data[8]|=Nmb_buf[ShiWei];
     Led_data[9]|=Nmb_buf[GeWei];
     Led_data[10]=0x3e;      
   }
       
}
void led_step()
{
	i2c_start();
	i2c_send(0x84);
	i2c_send(0x12);
	i2c_send(0x02); // shut down

	i2c_start();
	i2c_send(0x84);
	i2c_send(0x12);
	i2c_send(0x03); 

	i2c_start();
	i2c_send(0x84);
	i2c_send(0x00);

	for (u32 i = 0; i < ET6930_MAX_SCAN_LINE; i++)
	{
		i2c_send(Led_data[i]);
	}
	i2c_start();
	i2c_send(0x84);
	i2c_send(0x10);
	i2c_send(Led_level);
	i2c_send(LED_HS);	
	i2c_stop();
	//iic2
	i2c2_start();
	i2c2_send(0x84);
	i2c2_send(0x12);
	i2c2_send(0x02); // shut down

	i2c2_start();
	i2c2_send(0x84);
	i2c2_send(0x12);
	i2c2_send(0x03); 

	i2c2_start();
	i2c2_send(0x84);
	i2c2_send(0x00);

	for (u32 i = 0; i < ET6930_MAX_SCAN_LINE; i++)
	{
		i2c2_send(Led2_data[i]);
	}

	i2c2_start();
	i2c2_send(0x84);
	i2c2_send(0x10);
	i2c2_send(Led_level);
	i2c2_send(LED_HS);	
	i2c2_stop();
	
	
}

