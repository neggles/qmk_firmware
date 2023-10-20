#include "quantum.h"
#include "uconsole.h"

static const uint32_t backlight_timeout_ms = (BACKLIGHT_TIMEOUT * 1000);
static bool           backlight_restore    = false;

//----------------------------------------------------------
// Initialisation

void keyboard_post_init_kb(void) {
    keyboard_post_init_user();
}

void matrix_scan_kb(void) {
    matrix_scan_user();
}

//----------------------------------------------------------
// Housekeeping (backlight timeout etc)
#ifdef BACKLIGHT_ENABLE
static void backlight_check(void) {
    uint32_t last_input   = last_input_activity_elapsed();
    bool     backlight_on = is_backlight_enabled();

    if (last_input > backlight_timeout_ms) {
        if (backlight_on) {
            backlight_disable();
            backlight_restore = true;
        }
    } else {
        if (backlight_restore && !backlight_on) {
            backlight_enable();
            backlight_restore = false;
        }
    }
}
#endif

void post_process_record_kb(uint16_t keycode, keyrecord_t *record) {
    post_process_record_user(keycode, record);
}

void housekeeping_task_kb(void) {
#if defined(BACKLIGHT_ENABLE) && BACKLIGHT_TIMEOUT > 0
    backlight_check();
#endif
}
