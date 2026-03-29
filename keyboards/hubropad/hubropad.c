// Copyright 2026 Lasse Latva OH3HZB
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

#ifndef USB_HUB_ENUMERATION_DELAY_MS
#    define USB_HUB_ENUMERATION_DELAY_MS 500
#endif

#define HUBROPAD_DEBUG_LED_PIN B15

void keyboard_pre_init_kb(void) {
    // Wait for the integrated USB hub to come online before the USB driver
    // calls usbConnectBus() and asserts the D+ pull-up via PB3.
    // keyboard_pre_init_kb() executes before protocol_pre_init(), so this
    // delay runs before enumeration begins.
    wait_ms(USB_HUB_ENUMERATION_DELAY_MS);

    // Debug LED: output, initially off
    gpio_set_pin_output(HUBROPAD_DEBUG_LED_PIN);
    gpio_write_pin_low(HUBROPAD_DEBUG_LED_PIN);

    keyboard_pre_init_user();
}

void keyboard_post_init_kb(void) {
    // Turn on debug LED to signal successful init
    gpio_write_pin_high(HUBROPAD_DEBUG_LED_PIN);

    keyboard_post_init_user();
}
