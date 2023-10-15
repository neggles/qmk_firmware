// Copyright 2023 Andi Powers-Holmes (@neggles)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#ifdef TRACKBALL_ENABLE
#    include "trackball.h"
#endif

// Function prototypes
void keyboard_post_init_kb(void);
void matrix_scan_kb(void);
