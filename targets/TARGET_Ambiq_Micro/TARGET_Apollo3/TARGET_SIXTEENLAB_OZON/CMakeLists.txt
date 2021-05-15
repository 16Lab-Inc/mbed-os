# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sixteenlab-ozon INTERFACE)

target_include_directories(mbed-sixteenlab-ozon
    INTERFACE
        .
        bsp
)

target_sources(mbed-sixteenlab-ozon
    INTERFACE
        bsp/am_bsp.c
        bsp/am_bsp_pins.c
)

target_link_libraries(mbed-sixteenlab-ozon INTERFACE mbed-apollo3)
