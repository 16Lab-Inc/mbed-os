# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sxtlab-oz-june-21 INTERFACE)

target_include_directories(mbed-sxtlab-oz-june-21
    INTERFACE
        .
        bsp
)

target_sources(mbed-sxtlab-oz-june-21
    INTERFACE
        bsp/am_bsp.c
        bsp/am_bsp_pins.c
)

target_link_libraries(mbed-sxtlab-oz-june-21 INTERFACE mbed-apollo3)
