#pragma once

#include "pointing_device.h"

// Typedefs
typedef enum {
    TRACKBALL_MODE_MOUSE = 0,
    TRACKBALL_MODE_SCROLL,
    _TRACKBALL_MODE_LAST // Do not use, except for looping through enum values
} trackball_mode_t;

typedef struct {
    trackball_mode_t mode;
    uint8_t          mouseStep;
    uint8_t          scrollStep;
    uint8_t          state;
    report_mouse_t   report;
} trackball_state_t;

// Global/shared state variables
trackball_state_t trackball_state;

// Init and process functions
void trackball_init(void);
void trackball_process(void);

// Mode setting functions
trackball_mode_t trackball_mode_get(void);
void             trackball_mode_set(trackball_mode_t mode);
void             trackball_mode_cycle(bool reverse);
