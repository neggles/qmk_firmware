// Copyright 2023 Andi Powers-Holmes <aholmes@omnom.net> (@neggles)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* Enable permissive hold mode */
#define PERMISSIVE_HOLD

/* direct-mapped pins, used for rows 0 and 1 */
#define MATRIX_KEYS 16
#define MATRIX_KEY_PINS \
    { B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B13, B14, B15 }
// define to scan direct keys pin-by-pin instead of dumping the whole port
#define KEYS_PIN_BY_PIN

#ifdef TRACKBALL_ENABLE
/* Trackball */
#    define TRACKBALL_DIR_PINS \
        { C8, C9, C10, C11 }
#    define TRACKBALL_BUTTON_PIN C12
#    define TRACKBALL_MOUSE_STEP 5
#    define TRACKBALL_SCROLL_STEP 1
#    define TRACKBALL_MODE_DEFAULT TRACKBALL_MODE_MOUSE
#endif

#ifdef BACKLIGHT_ENABLE
/* Backlight is on PA8 which is TIM1_CH1 */
#    define BACKLIGHT_PWM_DRIVER PWMD1
#    define BACKLIGHT_PWM_CHANNEL 1
#    define BACKLIGHT_DEFAULT_LEVEL 0
#endif
