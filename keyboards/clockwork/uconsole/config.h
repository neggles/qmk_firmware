// Copyright 2023 Andi Powers-Holmes <aholmes@omnom.net> (@neggles)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* Enable permissive hold mode */
#define PERMISSIVE_HOLD

/* direct-mapped pins, used for rows 0 and 1 */
#define DIRECT_KEYS 16
#define DIRECT_KEY_PINS \
    { B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B13, B14, B15 }

/* Trackball pins */
#define TB_PINS \
    { C8, C9, C10, C11, C12 }
#define TB_BTN_PIN C12

/* Backlight is on PA8 which is TIM1_CH1 */
#define BACKLIGHT_PWM_DRIVER PWMD1
#define BACKLIGHT_PWM_CHANNEL 1
#define BACKLIGHT_DEFAULT_LEVEL 0
