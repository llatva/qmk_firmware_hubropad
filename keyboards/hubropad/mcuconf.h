// Copyright 2026 Lasse Latva OH3HZB
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include_next <mcuconf.h>

// PLL: 12 MHz HSE × 6 = 72 MHz system clock
// USB clock: 72 MHz / 1.5 = 48 MHz (correct for USB full-speed)
#undef STM32_PLLMUL_VALUE
#define STM32_PLLMUL_VALUE 6

// Enable TIM1 PWM for backlight on PA8 (TIM1_CH1)
#undef STM32_PWM_USE_TIM1
#define STM32_PWM_USE_TIM1 TRUE
