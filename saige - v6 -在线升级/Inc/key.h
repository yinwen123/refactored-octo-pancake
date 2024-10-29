


#ifndef _GP_KEY_H_
#define _GP_KEY_H_

#include "type.h"

#define KEY_MASK_ACC 1
#define KEY_MASK_XWI 2
#define KEY_MASK_QKI 4

#define set_status_mask(x)	Key_status |= (u8)x
#define clr_status_mask(x)	Key_status &= (~(u8)x)
#define read_status_mask(x)	(Key_status & x)

extern u8 Key_status,M_key;

void quake_check (void);

void key_task_10ms (void);

#endif
