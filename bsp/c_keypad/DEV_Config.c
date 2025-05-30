/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Luckfox team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2019-07-08
* | Info        :   Basic version
*
******************************************************************************/
#include "DEV_Config.h"


/*****************************************
                GPIO
*****************************************/
void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{
    SYSFS_GPIO_Write(Pin, Value);
}

UBYTE DEV_Digital_Read(UWORD Pin)
{
    UBYTE Read_value = 0;
    Read_value = SYSFS_GPIO_Read(Pin);
    return Read_value;
}

void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
{
    SYSFS_GPIO_Export(Pin);
    if(Mode == 0 || Mode == SYSFS_GPIO_IN){
        SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_IN);
        printf("IN Pin = %d\r\n",Pin);
    }else{
        SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_OUT);
        printf("OUT Pin = %d\r\n",Pin);
    }
}

/**
 * delay x ms
**/
void DEV_Delay_ms(UDOUBLE xms)
{
    UDOUBLE i;
    for(i=0; i < xms; i++){
        usleep(1000);
    }
}

static void DEV_GPIO_Init(void)
{
    DEV_GPIO_Mode(KEY_UP_PIN, 0);
    DEV_GPIO_Mode(KEY_DOWN_PIN, 0);
    DEV_GPIO_Mode(KEY_LEFT_PIN, 0);
    DEV_GPIO_Mode(KEY_RIGHT_PIN, 0);
    DEV_GPIO_Mode(KEY_PRESS_PIN, 0);
    DEV_GPIO_Mode(KEYA_PIN, 0);
    DEV_GPIO_Mode(KEYB_PIN, 0);
    DEV_GPIO_Mode(KEYX_PIN, 0);
    DEV_GPIO_Mode(KEYY_PIN, 0);
}

/******************************************************************************
function:	Module Initialize, the library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
UBYTE DEV_ModuleInit(void)
{
    DEV_GPIO_Init();
    return 0;
}
