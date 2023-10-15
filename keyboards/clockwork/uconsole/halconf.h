// Copyright 2023 Andi Powers-Holmes (@neggles)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Backlight PWM driver
#ifdef BACKLIGHT_ENABLE
#    define HAL_USE_PWM TRUE
#endif

// Needed for trackball interrupts
#ifdef TRACKBALL_ENABLE
#    define HAL_USE_PAL TRUE
#    define PAL_USE_CALLBACKS TRUE
#endif

// Disable SPI (conflicts with direct-mapped pins)
#define HAL_USE_SPI FALSE

#include_next <halconf.h>
