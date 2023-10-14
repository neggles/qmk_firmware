#include "trackball.h"
#include "uconsole.h"
#include "quantum.h"

#define CONSTRAIN_HID(amt) ((amt) < INT8_MIN ? INT8_MIN : ((amt) > INT8_MAX ? INT8_MAX : (amt)))
#define CONSTRAIN_HID_XY(amt) ((amt) < XY_REPORT_MIN ? XY_REPORT_MIN : ((amt) > XY_REPORT_MAX ? XY_REPORT_MAX : (amt)))

#ifdef TRACKBALL_DIR_PINS
static const pin_t trackball_dir_pins[4] = TRACKBALL_DIR_PINS;
#endif // TRACKBALL_DIR_PINS

#define TB_UP_BIT (1U << 0)
#define TB_RT_BIT (1U << 1)
#define TB_DN_BIT (1U << 2)
#define TB_LT_BIT (1U << 3)
#define TB_PAL_MASK PAL_GROUP_MASK(4)

volatile int16_t trackball_dx = 0;
volatile int16_t trackball_dy = 0;

static void trackball_isr(void *arg);

// Initialize trackball state
void trackball_init(void) {
    // Initialize state struct
    trackball_state.mode           = TRACKBALL_MODE_DEFAULT;
    trackball_state.mouseStep      = TRACKBALL_MOUSE_STEP;
    trackball_state.scrollStep     = TRACKBALL_SCROLL_STEP;
    trackball_state.state          = 0;
    trackball_state.report.x       = 0;
    trackball_state.report.y       = 0;
    trackball_state.report.v       = 0;
    trackball_state.report.h       = 0;
    trackball_state.report.buttons = 0;

    // Configure direction pins as inputs
    for (uint8_t i = 0; i < 4; i++) {
        setPinInput(trackball_dir_pins[i]);
    }
    // Configure button pin as input with pull-up
    setPinInputHigh(TRACKBALL_BUTTON_PIN);

    // Enable the trackball interrupts
    osalSysLock();
    for (uint8_t i = 0; i < 4; i++) {
        palDisableLineEventI(trackball_dir_pins[i]);
        palEnableLineEventI(trackball_dir_pins[i], PAL_EVENT_MODE_BOTH_EDGES);
        palSetLineCallbackI(trackball_dir_pins[i], &trackball_isr, NULL);
    }
    osalSysUnlock();
}

static void trackball_isr(void *arg) {
    (void)arg;

    chSysLockFromISR();
    // Read the direction pins
    uint8_t pinState = (uint8_t)((palReadPort(GPIOC) >> 8) & TB_PAL_MASK);
    chSysUnlockFromISR();

    // Accumulate the deltas
    if ((pinState & TB_UP_BIT) != (trackball_state.state & TB_UP_BIT)) trackball_dy -= trackball_state.mouseStep;
}

void trackball_process(void) {
    // Read the button and update the report
    uint8_t btn_state              = palReadLine(TRACKBALL_BUTTON_PIN);
    trackball_state.report.buttons = pointing_device_handle_buttons(trackball_state.report.buttons, btn_state, POINTING_DEVICE_BUTTON3);

    // Read and reset the interrupt counters
    osalSysLock();
    uint16_t dx  = trackball_dx;
    trackball_dx = 0;
    uint16_t dy  = trackball_dy;
    trackball_dy = 0;
    osalSysUnlock();

    // Load the report values into global state
    trackball_state.report.x = (dx * trackball_state.mouseStep);
    trackball_state.report.y = (dy * trackball_state.mouseStep);
    trackball_state.report.v = (dx * trackball_state.scrollStep);
    trackball_state.report.h = (dy * trackball_state.scrollStep);
}

// Mode setting functions
trackball_mode_t trackball_mode_get(void) {
    return trackball_state.mode;
};

void trackball_mode_set(trackball_mode_t mode) {
    trackball_state.mode = mode;
}

void trackball_mode_cycle(bool reverse) {
    trackball_mode_t mode = trackball_mode_get();
    if (reverse) {
        mode = (mode == 0) ? (_TRACKBALL_MODE_LAST - 1) : (mode - 1);
    } else {
        mode = (mode == (_TRACKBALL_MODE_LAST - 1)) ? 0 : (mode + 1);
    }
    trackball_mode_set(mode);
}

// Pointing device functions
void pointing_device_driver_init(void) {
    trackball_init();
}

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    // Read trackball state
    trackball_process();
    // Set report values
    switch (trackball_state.mode) {
        case TRACKBALL_MODE_MOUSE:
            mouse_report.x = CONSTRAIN_HID_XY(trackball_state.report.x);
            mouse_report.y = CONSTRAIN_HID_XY(trackball_state.report.y);
            break;
        case TRACKBALL_MODE_SCROLL:
            mouse_report.v = CONSTRAIN_HID(trackball_state.report.v);
            mouse_report.h = CONSTRAIN_HID(trackball_state.report.h);
            break;
        default:
            break;
    }
    // set button state
    mouse_report.buttons = trackball_state.report.buttons;

    return mouse_report;
}
