/*****************************************************************************
* | File      	:   DEV_Config.h
* | Author      :   Luckfox team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2019-07-08
* | Info        :   Basic version
*
******************************************************************************/
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include "Debug.h"

#include "sysfs_gpio.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

#define KEY_UP_PIN      57
#define KEY_DOWN_PIN    66
#define KEY_LEFT_PIN    65
#define KEY_RIGHT_PIN   67
#define KEY_PRESS_PIN   68 
#define KEYA_PIN        69
#define KEYB_PIN        54
#define KEYX_PIN        64
#define KEYY_PIN        55

#define GET_KEY_UP       		DEV_Digital_Read(KEY_UP_PIN)
#define GET_KEY_DOWN     		DEV_Digital_Read(KEY_DOWN_PIN)
#define GET_KEY_LEFT     		DEV_Digital_Read(KEY_LEFT_PIN)
#define GET_KEY_RIGHT    		DEV_Digital_Read(KEY_RIGHT_PIN)
#define GET_KEY_PRESS    		DEV_Digital_Read(KEY_PRESS_PIN)
#define GET_KEYA         		DEV_Digital_Read(KEYA_PIN)
#define GET_KEYB         		DEV_Digital_Read(KEYB_PIN)
#define GET_KEYX         		DEV_Digital_Read(KEYX_PIN)
#define GET_KEYY         		DEV_Digital_Read(KEYY_PIN) 

/*------------------------------------------------------------------------------------------------------*/
UBYTE DEV_ModuleInit(void);

void DEV_GPIO_Mode(UWORD Pin, UWORD Mode);
void DEV_Digital_Write(UWORD Pin, UBYTE Value);
UBYTE DEV_Digital_Read(UWORD Pin);
void DEV_Delay_ms(UDOUBLE xms);

#ifdef __cplusplus
}
#endif

#endif
