// Copyright 2026 Lasse Latva OH3HZB
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Delay in ms before asserting USB D+ pull-up.
// Gives the integrated USB hub time to initialize before the keyboard enumerates.
#define USB_HUB_ENUMERATION_DELAY_MS 500

// Backlight PWM: PA8 = TIM1_CH1
#define BACKLIGHT_PWM_DRIVER PWMD1
#define BACKLIGHT_PWM_CHANNEL 1
#define BACKLIGHT_PAL_MODE PAL_MODE_ALTERNATE_PUSHPULL
