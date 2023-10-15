// Copyright 2023 Andi Powers-Holmes (@neggles)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include_next <mcuconf.h>

// Backlight PWM uses TIM1
#undef STM32_PWM_USE_TIM1
#define STM32_PWM_USE_TIM1 TRUE

// SPI not needed
#undef STM32_SPI_USE_SPI2
#define STM32_SPI_USE_SPI2 FALSE
