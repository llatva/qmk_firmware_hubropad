// Copyright 2026 Lasse Latva OH3HZB
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include_next <board.h>

// Override HSE clock frequency for 12 MHz crystal (default board assumes 8 MHz)
#undef STM32_HSECLK
#define STM32_HSECLK 12000000

// Configure PB3 as output push-pull at startup (USB pull-up control).
// Default board sets all PB0-PB7 as input with pull-up (0x88888888).
// PB3 nibble changed from 8 (input pull-up) to 2 (output push-pull 2 MHz).
#undef VAL_GPIOBCRL
#define VAL_GPIOBCRL 0x88882888

// Clear PB3 output data at startup (USB pull-up de-asserted at boot).
// This prevents premature enumeration before the integrated hub is ready.
#undef VAL_GPIOBODR
#define VAL_GPIOBODR 0xFFFFFFF7

// USB bus activation: drive PB3 high to assert D+ pull-up
#undef usb_lld_connect_bus
#define usb_lld_connect_bus(usbp)                       \
    do {                                                \
        palSetPadMode(GPIOB, 3, PAL_MODE_OUTPUT_PUSHPULL); \
        palSetPad(GPIOB, 3);                            \
    } while (0)

// USB bus de-activation: drive PB3 low to de-assert D+ pull-up
#undef usb_lld_disconnect_bus
#define usb_lld_disconnect_bus(usbp)                    \
    do {                                                \
        palSetPadMode(GPIOB, 3, PAL_MODE_OUTPUT_PUSHPULL); \
        palClearPad(GPIOB, 3);                          \
    } while (0)
