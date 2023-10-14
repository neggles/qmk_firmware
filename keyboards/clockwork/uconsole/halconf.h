// Copyright 2023 Andi Powers-Holmes (@neggles)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#define HAL_USE_PWM TRUE

#define HAL_USE_PAL TRUE

// This enables interrupt-driven mode
#ifdef TRACKBALL_ENABLE
#    define PAL_USE_CALLBACKS TRUE
#endif

#include_next <halconf.h>
