


#ifndef _GP_MUSIC_H_
#define _GP_MUSIC_H_

#include "type.h"

#define SF_MUID         248
#define JF_MUID         247
#define YBJ_MUID        252
#define BJ_MUID         255
#define ST_MUID         254
#define CL_MUID         253
#define GB_MUID         251
#define XC_MUID         246
#define DLB_MUID        245
#define PD_MUID         244
#define PD1_MUID        243
#define OF_MUID         0

#define MUSIC_SF        music_set(SF_MUID)
#define MUSIC_JF        music_set(JF_MUID)
#define MUSIC_YBJ       music_set(YBJ_MUID)
#define MUSIC_BJ        music_set(BJ_MUID)
#define MUSIC_ST        music_set(ST_MUID)
#define MUSIC_CL        music_set(CL_MUID)
#define MUSIC_GB        music_set(GB_MUID)
#define MUSIC_XC        music_set(XC_MUID)
#define MUSIC_OF        music_set(OF_MUID)
#define MUSIC_DLB       music_set(DLB_MUID)
#define MUSIC_PD        music_set(PD_MUID)
#define MUSIC_PD1       music_set(PD1_MUID)
#define MUSIC_T(x)      music_set(x)

#define MUSIC_STA       (CW_BTIM1->BCR_f.EN)

#define M_CYCLE(x)     CW_BTIM1->ARR = x * 48; Music_cycle = x
#define M_TIE          CW_BTIM1->IER_f.OV


typedef union
{
    u32 Word;
    struct
    {
        u8 Hcycle;
        u8 Cycle;
        u8 Num;
        u8 Wait_Cycle;
    }Bytes;
}MU_REG0;

typedef union
{
    u32 Word;
    struct
    {
        u8 Music_wait;
        u8 Music_pos;
        u8 Music_alm;
        u8 Music_end;
    }Bytes;
}MU_REG1;

extern volatile MU_REG0 Mu_reg0;
extern volatile MU_REG1 Mu_reg1;

extern volatile u16 Cnt_dlb_tim;

void music_set (u8 mic);

#endif
