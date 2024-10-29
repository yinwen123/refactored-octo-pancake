/*
名称：port.h
功能：IO口头文件
日期：20151119
*/

#ifndef _GP_PORT_H
#define _GP_PORT_H

#include "type.h"

#define uGPIO(x)			(1 << x)

#define MUSIC_PORT_OUT    		0
#define MUSIC_PORT_STOP   		0
#define MUSIC_PORT_XOR	  		0
#define MUSIC_PORT_XOR_ONE		0

//#define ACC_ON          GPIOA->BSRR |= uGPIO(7)
//#define ACC_OFF         GPIOA->BRR  |= uGPIO(7)
//#define ACC_STA        (GPIOA->ODR & uGPIO(7))

//#define SDJ_ON          GPIOC->BSRR |= uGPIO(0)
//#define SDJ_OFF         GPIOC->BRR  |= uGPIO(0)

//#define ZX_H          	GPIOB->BSRR |= uGPIO(3)
//#define ZX_L         	  GPIOB->BRR  |= uGPIO(3)
//#define ZX_STA        	(GPIOB->ODR & uGPIO(3))

//#define XW_IN           (GPIOB->IDR & uGPIO(1))
//#define QK_IN           (GPIOA->IDR & uGPIO(1))

#define RF_IN           (CW_GPIOA->IDR & uGPIO(0))

//#define ZX_IN           (GPIOB->IDR & uGPIO(4))
//#define DD_IN           (GPIOB->IDR & uGPIO(0))
//#define SKD_IN          (GPIOC->IDR & uGPIO(2))

//#define COM_IN          (GPIOB->IDR & uGPIO(2))

//#define KEY_LEARN       (GPIOA->IDR & uGPIO(5))
#define COM_IN  (CW_GPIOB->IDR & uGPIO(2))
#define SKD_IN  (CW_GPIOC->IDR & uGPIO(0))
#define OUT_NFC_H   CW_GPIOB->BSRR |= uGPIO(6)
#define OUT_NFC_L   CW_GPIOB->BRR  |= uGPIO(6) 
#define RE_485_H    CW_GPIOA->BSRR |= uGPIO(4)
#define RE_485_L    CW_GPIOA->BRR  |= uGPIO(4)
#define LED_SCL_H       CW_GPIOA->BSRR |= uGPIO(0)
#define LED_SCL_L       CW_GPIOA->BRR  |= uGPIO(0)

#define LED_SDA_H       CW_GPIOB->BSRR |= uGPIO(5)
#define LED_SDA_L       CW_GPIOB->BRR  |= uGPIO(5)

#define LED2_SDA_H       CW_GPIOB->BSRR |= uGPIO(4)
#define LED2_SDA_L       CW_GPIOB->BRR  |= uGPIO(4)

#define LED2_SDA_I       CW_GPIOB->DIR |= uGPIO(4)
#define LED2_SDA_O       CW_GPIOB->DIR &= ~uGPIO(4)

#define LED_SDA_I       CW_GPIOB->DIR |= uGPIO(5)
#define LED_SDA_O       CW_GPIOB->DIR &= ~uGPIO(5)

#define LOGO_ON         CW_GPIOB->BSRR |= uGPIO(0)
#define LOGO_OFF        CW_GPIOB->BRR  |= uGPIO(0)

#define RF_RST_H        CW_GPIOC->BSRR |= uGPIO(5)
#define RF_RST_L        CW_GPIOC->BRR  |= uGPIO(5)

#define SPI_MISO        (CW_GPIOC->IDR & uGPIO(1))

#define SPI_MO_H        CW_GPIOC->BSRR |= uGPIO(2)
#define SPI_MO_L        CW_GPIOC->BRR  |= uGPIO(2)

#define SPI_CK_H        CW_GPIOC->BSRR |= uGPIO(0)
#define SPI_CK_L        CW_GPIOC->BRR  |= uGPIO(0)

#define SPI_CS_H        CW_GPIOB->BSRR |= uGPIO(1)
#define SPI_CS_L        CW_GPIOB->BRR  |= uGPIO(1)

#define ZZ_IN           (CW_GPIOC->IDR & uGPIO(1))
#define YZ_IN           (CW_GPIOC->IDR & uGPIO(2))
#define DD_IN           (CW_GPIOB->IDR & uGPIO(1))

#define SKD_IN          (CW_GPIOC->IDR & uGPIO(0))

#define ACC_IN           (CW_GPIOA->IDR & uGPIO(6))
#define ACC_ON          CW_GPIOB->BSRR |= uGPIO(6)
#define ACC_OFF         CW_GPIOB->BRR  |= uGPIO(6)
#define ACC_STA        (CW_GPIOB->ODR & uGPIO(6))
void gpio_init (void);

void nfc_afinit(u8 t);

#endif

//结束
