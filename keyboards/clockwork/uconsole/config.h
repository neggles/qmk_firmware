// Copyright 2023 Andi Powers-Holmes <aholmes@omnom.net> (@neggles)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

/* Enable permissive hold mode */
#define PERMISSIVE_HOLD

/* direct-mapped pins, used for rows 0 and 1 */
#define DIRECT_ROWS 2
#define MATRIX_KEY_PINS \
    { {B0, B1, B2, B3, B4, B5, B6, B7}, {B8, B9, B10, B11, B12, B13, B14, B15}, }

/* Trackball config */
#define TRACKBALL_DIR_PINS \
    { C8, C9, C10, C11 }
#define TRACKBALL_BUTTON_PIN C12
#define TRACKBALL_MOUSE_STEP 2
#define TRACKBALL_SCROLL_STEP 1
#define TRACKBALL_MODE_DEFAULT TRACKBALL_MODE_MOUSE
#define TRACKBALL_ACCEL_FACTOR 1.5f
#define TRACKBALL_ACCEL_MAX 20.0f

#ifdef TRACKBALL_ENABLE
#    define MOUSE_EXTENDED_REPORT
#    define POINTING_DEVICE_TASK_THROTTLE_MS 1
#endif

#ifdef BACKLIGHT_ENABLE
/* Backlight is on PA8 which is TIM1_CH1 */
#    define BACKLIGHT_PWM_DRIVER PWMD1
#    define BACKLIGHT_PWM_CHANNEL 1
#    define BACKLIGHT_DEFAULT_LEVEL 0
/* Backlight timeout in seconds, set to 0 to disable */
#    define BACKLIGHT_TIMEOUT 60
#endif
