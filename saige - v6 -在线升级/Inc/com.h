/*
	名称：com.h
	作者：LiGuo.Li
	功能描述：通信函数头文件
	版本：V1.0
	功能修订日志：
	版本      日期      修订内容
	V1.0    2020.4.11	创建文件
*/

#ifndef _GP_COM_H
#define _GP_COM_H

#include "type.h"
#include "inc.h"
//#define COM_USART USART3
extern uint8_t P_STA,V60_STA,V72_STA;
//#define P_STA     ((BLDC_Sta[2] & 0x02))

//#define V60_STA     ((BLDC_Sta[10]&0x4)|((rx_buf[28]==1)? 1 :0))

//#define V72_STA     ((BLDC_Sta[10]&0x10)|((rx_buf[28]==2)? 1 :0))

#define SPEED_UNLOCK     ((BLDC_Sta[5] & 0x1) == 0)

#define SPEED_LOCK     (BLDC_Sta[5] & 0x1)

#define SC_STA     (BLDC_Sta[4] & 0x20)

extern u8 BLDC_Sta[11],Speed,Speedt;

extern u16 Trip_lc;

void com_task_50us(void);

void lc_add(void);

typedef struct
{
    u32 TCS :1;
    u32 Pmod :1;
    u32 carmod :2;
    u32 jidui :2;
    u32 LunJin :2;
    
    u32 QueXiang :1;
	u32 ZhuLi  :1;
	u32 XunHang :1;
	u32 qianYa :1;
    u32 control_error :1;
	u32 zhuanBa_error :1;
	u32 hull_error   :1;
	u32 qiansuan_mod :1;
	
	u32 SanSu  :2;
	u32 FangFeiChe :1;
	u32 HuaXinChongDian :1;
	u32 control_pr :1;
	u32 ShaChe :1;
	u32 moto_run :1;
	u32 SanSu_mod :1;
	
	u32 XianSu :1;
	u32 DanZiShaChe :1;
	u32 DaoChe :1;
	u32 DuZhuan :1;
	u32 GuoLiu :1;
	u32 ChangGui :1;
	u32 YunDon :1;
	u32 JieNen :1;
	uint8_t sbuf[5];
	
}STAT_T;
typedef union
{
	uint8_t buf[10];
	STAT_T data;
}stat_t;

void speedope(void);
extern stat_t car_stat;
#endif
