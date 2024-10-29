/*
名称：inc.h
功能：包含头文件
日期：20151119
*/

#ifndef _GP_INC_H_
#define _GP_INC_H_

#define SYS_CLK 48

#include "type.h"
#include "cw32f003.h"
#include "port.h"
#include "tim.h"
#include "flash.h"
#include "adc.h"

#include "com.h"
#include "fdq.h"
#include "key.h"
#include "music.h"
#include "led.h"
#include "rf.h"
#include "cw32f003_uart.h"
#include "cw32f003_gpio.h"
#define STATE_UNDEFINE      0xFF
#define STATE_LOCK          0xAA
#define STATE_UNLOCK        0xA5
#define STATE_START         0xA0

#define START_TYPE_KEY      1
#define START_TYPE_ACC      2
#define START_TYPE_NFC      3
#define START_TYPE_BLE      4
#define START_TYPE_RF       5
#define START_TYPE_MK       6
#define START_TYPE_KAC      7
#define UD_ADCREF	  ADC_ref
#define INIT_MODE     Init_mode

#define NVIC_VOTR  *(volatile u32*)0xE000ED08

#define CLR_WDT    IWDT->KR = 0xAAAA

#define CLR_BIT(x) (~(1 << x))

#define GET_REG_BIT(reg,bit)  (reg & ((u32)0x00000001u << bit))
#define SET_REG_BITS(reg,bit,bs,dat)  reg = reg & (((u32)0xFFFFFFFFu << (bit + bs)) | (((u32)0xFFFFFFFFu >> (31u - bit)) >> 1)) | (dat << bit)
void nfc_loop(void);
void set_mode(u8 mode);
void UART2_INIT(uint16_t baud);
void modbus_ack(void);
void recv_deal(void);
u8* alm_send(u8 len,u8 cmd);
extern uint8_t Led_data[16];
extern uint8_t Led2_data[16];
extern stat_t car_stat;
extern u8 Tim_write,Nfc_af,yxt_vch,motor_STA,tcs_sta;
extern u8 Set_card[12];
extern uint8_t STEP_485,run_step;
extern uint8_t rx_buf[48];
extern uint8_t over_time,rx_nub,Led_level;
extern u16 Cnt_hall,cut485;
extern uint8_t hour,min;
extern uint16_t percent,voltage;
extern uint8_t yxt_rcv,yxt_rcv1,R_mod;
extern uint16_t timeover;
extern u8 Start_type,Alm_type,Alm_state,ALM_s,card_learn,ALM_c,GuoBiao_mod,car_dt,v485;
extern volatile u16 Tim_out;
extern uint16_t soc_485;
extern uint16_t trip_485,odo_485,voltage_485,speed_485,last_trip;
#endif

//结束
