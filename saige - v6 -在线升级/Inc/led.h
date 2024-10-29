


#ifndef _GP_LED_H_
#define _GP_LED_H_

#include "type.h"
#include "inc.h"
extern uint8_t Led_data[16];
extern uint8_t Led2_data[16];
typedef struct
{
	unsigned ECU :1;
	unsigned ZhiDong :1;
	unsigned motor :1;
	unsigned ZhuanBa :1;
	unsigned JieNen :1;
	unsigned BiaoZhun :1;
	unsigned XueHua :1;
    unsigned DongLi :1;
}state;
typedef union	
{
	state STA;
	uint8_t data;
}STA_T;
extern uint8_t Led_data[16];
extern uint8_t Led2_data[16];
extern STA_T temp;
#define XA_ON Led_data[2]|=0x01<<7
#define XA_OFF Led_data[2]&=~(0x01<<7)
#define MA_ON Led_data[3]|=0x01<<7
#define MA_OFF Led_data[3]&=~(0x01<<7)
#define MaoHao_on  (Led_data[0]|=0x01<<7,Led_data[1]|=0x01<<7)
#define MaoHao_off (Led_data[0]&=~(0x01<<7),Led_data[1]&=~(0x01<<7))
#define ABS_ON  Led_data[7]|=0x01<<7
#define ABS_OFF Led_data[7]&=~(0x01<<7)
#define BaiFenHao_on Led_data[4]|=0x01<<5
#define BaiFenHao_off Led_data[4]&=~(0x01<<5)
#define V_ON Led_data[4]|=0x01<<6
#define V_OFF Led_data[4]&=~(0x01<<6)
#define QianYa_on Led_data[4]|=0x01<<7
#define QianYa_off Led_data[4]&=~(0x01<<7)
#define ODO_ON Led_data[9]|=0x01<<7
#define ODO_OFF Led_data[9]&=~(0x01<<7)
#define TRIP_ON Led_data[10]|=0x01<<7
#define TRIP_OFF Led_data[10]&=~(0x01<<7)
#define KM_ON  Led_data[11]|=(0x01<<7)
#define KM_OFF Led_data[11]&=~(0x01<<7)
#define KM_H_ON Led2_data[6]|=0x01<<7
#define KM_H_OFF Led2_data[6]&=~(0x01<<7)
#define FangHua_ON Led2_data[7]|=0x01<<7
#define FangHua_OFF Led2_data[7]&=~(0x01<<7)
#define turn_left_on (Led2_data[5]|=0x01<<7,Led2_data[0]|=0x01<<7)
#define turn_left_off (Led2_data[5]&=~(0x01<<7),Led2_data[0]&=~(0x01<<7))
#define DD_ON  Led2_data[4]|=0X01<<7
#define DD_OFF Led2_data[4]&=~(0X01<<7)
#define XunHang_on Led2_data[11]|=0x01<<4
#define XunHang_off Led2_data[11]&=~(0x01<<4)
#define SKD_ON Led2_data[11]|=0x01<<5
#define SKD_OFF Led2_data[11]&=~(0x01<<5)
#define turn_right_on (Led2_data[10]|=0x01<<6,Led2_data[10]|=0x01<<7)
#define turn_right_off (Led2_data[10]&=~(0x01<<6),Led2_data[10]&=~(0x01<<7))
#define R_ON (Led2_data[3]|=0X01<<6)
#define R_OFF (Led2_data[3]&=~(0X01<<6))
void led_step(void);
void show_speed(uint16_t data);
void show_gear(uint8_t data);
void show_mileage(uint32_t data,uint8_t mod);
void power_level_display(uint8_t level);
void show_zuoxia1_data(uint8_t data);
void show_zuoxia2_data(uint8_t data,uint8_t mod);
void show_voltage(uint16_t data,uint8_t mod);
void time_dis(void);
void show_card(uint8_t data);
#endif
