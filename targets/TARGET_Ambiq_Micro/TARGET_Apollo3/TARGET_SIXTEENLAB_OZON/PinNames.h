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
    LED_CTRL  =      27,

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
    CHG_STAT_1 = 32 ,
    CHG_STAT_2 = 30 ,
    CHG_ACPR = 34 ,
    CHG_OUT = 31,


    IMU_SDA = 6 ,
    IMU_SCL = 5 ,
    IMU_VDD = 13 ,
    IMU_INT1 = 10 ,
    IMU_INT2 = 29 ,

    LRA_EN = 44, 
    LRA_SDA = 9, 
    LRA_SCL = 8, 
    LRA_TRIG = 38,

    TOUCH_IRQ = 3,
    TOUCH_VDD = 25,
    TOUCH_SCL = 39,
    TOUCH_SDA = 40,
    TOUCH_RST = 45,


// Other defines


     // mbed original LED naming
    LED1 = NC_VAL,
    LED2 = NC_VAL,
    LED3 = NC_VAL,
    LED4 = NC_VAL,

    // I2C
    I2C_SCL = NC_VAL,
    I2C_SDA = NC_VAL,

    // Qwiic
    QWIIC_SCL = NC_VAL,
    QWIIC_SDA = NC_VAL,

    // SPI
    // The SFE_EDGE does not expose a complete IOM peripheral for SPI

    // UART
    SERIAL_TX = AM_BSP_PRIM_UART_TX_PIN,
    SERIAL_RX = AM_BSP_PRIM_UART_RX_PIN,
    USBTX = SERIAL_TX,
    USBRX = SERIAL_RX,

    // Not connected
    NC = NC_VAL
} PinName;

#define STDIO_UART_TX USBTX
#define STDIO_UART_RX USBRX

#define IMU_I2C_ADDR    0x68
#define HAPTIC_I2C_ADDR 0x5a
#define TOUCH_I2C_ADDR  0x41

#ifdef __cplusplus
}
#endif

#endif
