// Copyright 2023 Andi Powers-Holmes (@neggles)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include "quantum.h"

#ifdef TRACKBALL_ENABLE
#    include "trackball.h"
#endif

// Enums for keycodes and layers
enum uconsole_layers {
    _DEF = 0,
    _FN,
};

#define KEYS_PIN_BY_PIN
