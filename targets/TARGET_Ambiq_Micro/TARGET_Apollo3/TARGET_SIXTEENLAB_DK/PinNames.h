/* 
 * Copyright (c) 2019-2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "am_bsp.h"
#include "objects_gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NC_VAL (int)0xFFFFFFFF

typedef enum
{
    LED_CTRL  =      29,
    LED_EN =    NC_VAL,
    DEBUG_P29_P9  =   45, /* GPIO45 connected to pin9 in header P29*/

/*
        CHRG                     ACPR    STAT1   STAT2   STATUS
        -----------------------+--------+-------+------+----------------
        Hi-Impedance             0        X         X    Not Charging, No Power,
        Pulled LOW               1        0         0    Done Charging
        Blink Slow (1.14Hz)      1        0         1    Charging
        Blink Fast (4.58Hz)      1        1         0    Temperature Fault/Bad Battery
        Blink Fast (4.58Hz)      1        1         1    Differential Undervoltage Current Limit (DUVCL)

        ACPR STAT1 STAT2    STATUS
        ----+-----+------+--------------------------------------
        0       0   0       VBAT < 3.2V, Low Battery Alert 3
        0       0   1       3.2V < VBAT < 3.3V
        0       1   0       3.3V < VBAT < 3.6V
        0       1   1       VBAT > 3.6V
*/
    /* charger pins */
    CHG_STAT_1 = 4 ,
    CHG_STAT_2 = 24 ,
    CHG_ACPR = 23 ,
    CHG_OUT = 7, // This pin should be used only with DK, as a hack to make the stats pins provide output

    /* IMU pins */
    IMU_SDA = 6 ,
    IMU_SCL = 5 ,
    IMU_VDD = 13 ,
    IMU_INT1 = 10 ,
    IMU_INT2 = 15 ,

    /* LRA pins*/
    LRA_EN = 26, 
    LRA_SDA = 9, 
    LRA_SCL = 8, 
    LRA_TRIG = 12, 

    /*Touch pins*/
    TOUCH_RST = 14,
    TOUCH_SDA = 40,
    TOUCH_SCL = 39,
    TOUCH_IRQ = 28,
    TOUCH_VDD = 25,

// Other defines
    LED1 = 0,
    LED2 = NC_VAL,
    LED3 = NC_VAL,
    LED4 = NC_VAL,

    // UART
    SERIAL_TX = 37,
    SERIAL_RX = 38,
    CONSOLE_TX = SERIAL_TX,
    CONSOLE_RX = SERIAL_RX,


    SE_EN           = 31,
    //...pins 45-46 are missing from WLCP device...
    SE_EN_RF        = 32,
    SE_SCL          = 33,
    SE_SDA          = 34,


    // Not connected
    NC = NC_VAL
} PinName;

#define STDIO_UART_TX SERIAL_TX
#define STDIO_UART_RX SERIAL_RX

#define IMU_I2C_ADDR    0x68
#define HAPTIC_I2C_ADDR 0x5a
#define TOUCH_I2C_ADDR 0x41

#ifdef __cplusplus
}
#endif

#endif
