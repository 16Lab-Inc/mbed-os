//*****************************************************************************
//
//  am_bsp.c
//! @file
//!
//! @brief Top level functions for performing board initialization.
//!
//! @addtogroup BSP Board Support Package (BSP)
//! @addtogroup apollo3_eb_bsp BSP for the Apollo3 Engineering Board
//! @ingroup BSP
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2019, Ambiq Micro
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
// 
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision v2.0.0 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause

#include "am_bsp.h"
#include "am_util.h"

//*****************************************************************************
//
// Power tracking structures for IOM and UART
//
//*****************************************************************************
am_bsp_uart_pwrsave_t am_bsp_uart_pwrsave[AM_REG_UART_NUM_MODULES];

//*****************************************************************************
//
// Print interface tracking variable.
//
//*****************************************************************************
static uint32_t g_ui32PrintInterface = AM_BSP_PRINT_INFC_NONE;


//*****************************************************************************
//
//! @brief Prepare the MCU for low power operation.
//!
//! This function enables several power-saving features of the MCU, and
//! disables some of the less-frequently used peripherals. It also sets the
//! system clock to 24 MHz.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_low_power_init(void)
{
    //
    // Initialize for low power in the power control block
    //
    am_hal_pwrctrl_low_power_init();

    //
    // Disable the RTC.
    //
    am_hal_rtc_osc_disable();

    //
    // Stop the XTAL.
    //
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_XTAL_STOP, 0);

    //
    // Make sure SWO/ITM/TPIU is disabled.
    // SBL may not get it completely shut down.
    //
    am_bsp_itm_printf_disable();

} // am_bsp_low_power_init()

//*****************************************************************************
//
//! @brief Enable the TPIU and ITM for debug printf messages.
//!
//! This function enables TPIU registers for debug printf messages and enables
//! ITM GPIO pin to SWO mode. This function should be called after reset and
//! after waking up from deep sleep.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_debug_printf_enable(void)
{
    //FIXME: UART PRINTF ias disabled
    return;

} // am_bsp_debug_printf_enable()

//*****************************************************************************
//
//! @brief Enable the TPIU and ITM for debug printf messages.
//!
//! This function disables TPIU registers for debug printf messages and
//! enables ITM GPIO pin to GPIO mode and prepares the MCU to go to deep sleep.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_debug_printf_disable(void)
{
    return;

} // am_bsp_debug_printf_disable()

//*****************************************************************************
//
// @brief Enable printing over ITM.
//
//*****************************************************************************
//*****************************************************************************
//
// @brief Enable printing over ITM.
//
//*****************************************************************************
#ifdef DEBUG_SWO
void
am_bsp_itm_printf_enable(void)
{
    am_hal_tpiu_config_t TPIUcfg;

    //
    // Set the global print interface.
    //
    g_ui32PrintInterface = AM_BSP_PRINT_INFC_SWO;

    //
    // Enable the ITM interface and the SWO pin.
    //
    am_hal_itm_enable();
//  am_hal_gpio_pinconfig(AM_BSP_GPIO_ITM_SWO, g_AM_BSP_GPIO_ITM_SWO);

    //
    // Enable the ITM and TPIU
    // Set the BAUD clock for 1M
    //
    TPIUcfg.ui32SetItmBaud = AM_HAL_TPIU_BAUD_1M;
    am_hal_tpiu_enable(&TPIUcfg);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_ITM_SWO, g_AM_BSP_GPIO_ITM_SWO);

    //
    // Attach the ITM to the STDIO driver.
    //
    am_util_stdio_printf_init(am_hal_itm_print);
} // am_bsp_itm_printf_enable()
#else
void
am_bsp_itm_printf_enable(void);
#endif
//*****************************************************************************
//
//! @brief ITM-based string print function.
//!
//! This function is used for printing a string via the ITM.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_itm_string_print(char *pcString)
{
    return;

}

//*****************************************************************************
//
// @brief Disable printing over ITM.
//
//*****************************************************************************
void
am_bsp_itm_printf_disable(void)
{
    return;
    //
    // am_hal_gpio_pinconfig(AM_BSP_GPIO_ITM_SWO, g_AM_HAL_GPIO_DISABLE);
} // am_bsp_itm_printf_disable()

//*****************************************************************************
//
//! @brief Set up the IOM pins based on mode and module.
//!
//! This function configures up to 10-pins for MSPI serial, dual, quad,
//! dual-quad, and octal operation.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_iom_pins_enable(uint32_t ui32Module, am_hal_iom_mode_e eIOMMode)
{
    return;
    uint32_t ui32Combined;

    //
    // Validate parameters
    //
    if ( ui32Module >= AM_REG_IOM_NUM_MODULES )
    {
        //
        // FPGA supports only IOM0 and 1.
        //
        return;
    }

    ui32Combined = ((ui32Module << 2) | eIOMMode);

    switch ( ui32Combined )
    {
        case ((0 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SCL,  g_AM_BSP_GPIO_IOM0_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SDA,  g_AM_BSP_GPIO_IOM0_SDA);
            break;

        case ((1 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_SCL,  g_AM_BSP_GPIO_IOM1_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_SDA,  g_AM_BSP_GPIO_IOM1_SDA);
            break;

        case ((2 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_SCL,  g_AM_BSP_GPIO_IOM2_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_SDA,  g_AM_BSP_GPIO_IOM2_SDA);
            break;

        case ((3 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_SCL,  g_AM_BSP_GPIO_IOM3_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_SDA,  g_AM_BSP_GPIO_IOM3_SDA);
            break;

        case ((4 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SCL,  g_AM_BSP_GPIO_IOM4_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SDA,  g_AM_BSP_GPIO_IOM4_SDA);
            break;

        case ((5 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_SCL,  g_AM_BSP_GPIO_IOM5_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_SDA,  g_AM_BSP_GPIO_IOM5_SDA);
            break;

        default:
            break;
    }
} // am_bsp_iom_pins_enable()

//*****************************************************************************
//
//! @brief Disable the IOM pins based on mode and module.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_iom_pins_disable(uint32_t ui32Module, am_hal_iom_mode_e eIOMMode)
{
    return;

} // am_bsp_iom_pins_disable()

//*****************************************************************************
//
//! @brief Set up the MSPI pins based on the external flash device type.
//!
//! This function configures up to 10-pins for MSPI serial, dual, quad,
//! dual-quad, and octal operation.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_mspi_pins_enable(am_hal_mspi_device_e eMSPIDevice)
{
    return;
} // am_bsp_mspi_pins_enable()

//*****************************************************************************
//
//! @brief Disable the MSPI pins based on the external flash device type.
//!
//! This function configures up to 10-pins for MSPI serial, dual, quad,
//! dual-quad, and octal operation.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_mspi_pins_disable(am_hal_mspi_device_e eMSPIDevice)
{
    return;
} // am_bsp_mspi_pins_disable()

//*****************************************************************************
//
//! @brief Set up the IOS pins based on mode and module.
//!
//! @return None.
//
//*****************************************************************************
void am_bsp_ios_pins_enable(uint32_t ui32Module, uint32_t ui32IOSMode)
{
    return;
} // am_bsp_ios_pins_enable()

//*****************************************************************************
//
//! @brief Disable the IOS pins based on mode and module.
//!
//! @return None.
//
//*****************************************************************************
void am_bsp_ios_pins_disable(uint32_t ui32Module, uint32_t ui32IOSMode)
{
   return;
} // am_bsp_ios_pins_disable()

//*****************************************************************************
//
//! @brief UART-based string print function.
//!
//! This function is used for printing a string via the UART, which for some
//! MCU devices may be multi-module.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_uart_string_print(char *pcString)
{
    return;
} // am_bsp_uart_string_print()

//*****************************************************************************
//
// Pass-through function to let applications access the COM UART.
//
//*****************************************************************************
uint32_t
am_bsp_com_uart_transfer(const am_hal_uart_transfer_t *psTransfer)
{
    return 0;
} // am_bsp_com_uart_transfer()

//*****************************************************************************
//
// Initialize and configure the UART
//
//*****************************************************************************
void
am_bsp_uart_printf_enable(void)
{
    return;
} // am_bsp_uart_printf_enable()

//*****************************************************************************
//
// Initialize and configure the UART with a custom configuration
//
//*****************************************************************************
void
am_bsp_uart_printf_enable_custom(const am_hal_uart_config_t* p_config)
{
    return;
} // am_bsp_uart_printf_enable()

//*****************************************************************************
//
// Disable the UART
//
//*****************************************************************************
void
am_bsp_uart_printf_disable(void)
{
    return;
    //
} // am_bsp_uart_printf_disable()

#ifndef AM_BSP_DISABLE_BUFFERED_UART
//*****************************************************************************
//
// Initialize and configure the UART
//
//*****************************************************************************
void
am_bsp_buffered_uart_printf_enable(void)
{
    return;
} // am_bsp_buffered_uart_printf_enable()

//*****************************************************************************
//
// Interrupt routine for the buffered UART interface.
//
//*****************************************************************************
void
am_bsp_buffered_uart_service(void)
{
    return;

} // am_bsp_buffered_uart_service()
#endif // AM_BSP_DISABLE_BUFFERED_UART



//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
