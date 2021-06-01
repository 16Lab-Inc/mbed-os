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
    GPIO0           = 0,
    GPIO1           = 1,
    GPIO2           = 2,
    TOUCH_IRQ       = 3,
    GPIO4           = 4,
    IMU_SCL         = 5,
    IMU_SDA         = 6,
    HAPTICS_EN      = 7,
    LRA_EN          = 7,
    HAPTICS_SCL     = 8,
    LRA_SCL         = 8,
    HAPTICS_SDA     = 9,
    LRA_SDA         = 9,
    IMU_INT2        = 10,
    GPIO11          = 11,
    HAPTICS_TRIG    = 12,
    IMU_VDD         = 13,
    CHG_STAT_2       = 14,
    CHG_EN          = 15,
    CHG_OUT         = 15,
    CHG_ACPR        = 16,
    CHG_STAT_1       = 17,
    CHG_CHRG        = 18,
    GPIO19          = 19,
    SWDCK           = 20,
    SWDIO           = 21,
    SWO             = 22,
    GPIO23          = 23,
    SE_RF_DET       = 24,
    TOUCH_VDD       = 25,
    SE_IRQ          = 26,
    LED_CTRL        = 27,
    SE_RSTN         = 28,
    IMU_INT1        = 29,
    //...pins 30-38 are missing from WLCP device...
    TOUCH_SCL       = 39,
    TOUCH_SDA       = 40,
    TOUCH_RST       = 41,
    //...pins 42-43 are missing from WLCP device...
    SE_EN           = 44,
    //...pins 45-46 are missing from WLCP device...
    SE_EN_RF        = 47,
    SE_SCL          = 48,
    SE_SDA          = 49,


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
    CONSOLE_TX = NC_VAL,
    CONSOLE_RX = NC_VAL,

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
