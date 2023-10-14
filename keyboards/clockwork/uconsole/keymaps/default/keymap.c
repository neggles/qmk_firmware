// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "modifiers.h"
#ifdef POINTING_DEVICE_ENABLE
#    include "trackball.h"
#endif

/*
 *              ┌───────┐         ┌───────┐                                       ┌───────┬───────┐
 *      ┌───────┤  UP   ├───────┐ │ BTN1  │                                       │ JS_3  │ JS_2  │
 *      │ LEFT  ├───────┤ RGHT  │ └───┬───┴───┐                                   └───┬───┴───┬───┴───┐
 *      └───────┤ DOWN  ├───────┘     │ BTN2  │                                       │ JS_1  │ JS_0  │
 *              └───────┘             └───────┘                                       └───────┴───────┘
 *  ┌───────────┐ ┌─────────┬─────────┐           ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┐
 *  │   ESC     │ │  SLCT   │  APP    │           │ VOLU  │  [    │  ]    │  /    │  -    │  =    │  \    │
 *  └─┬───────┬─┴─┴───┬─────┴─┬───────┼───────┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───────┤
 *    │  `    │  1    │  2    │  3    │  4    │  5    │  6    │  7    │  8    │  9    │  0    │ BACKSPACE │
 *  ┌─┴───────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────────┘
 *  │   TAB     │  Q    │  W    │  E    │  R    │  T    │  Y    │  U    │  I    │  O    │  P    │
 *  └─┬───────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────────┐
 *    │  '    │  A    │  S    │  D    │  F    │  G    │  H    │  J    │  K    │  L    │  ;    │  ENTER    │
 *  ┌─┴───────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─────┬─────┘
 *  │  SHIFT    │  Z    │  X    │  C    │  V    │  B    │  N    │  M    │  ,    │  .    │  SHIFT    │
 *  └─────┬─────┴───┬───┴─────┬─┴───────┼───────┴───────┴───────┴─────┬─┴───────┼───────┴─┬─────────┤
 *        │ OSL(1)  │  CTRL   │  OPT    │           SPACE             │  OPT    │  CTRL   │ OSL(1)  │
 *        └─────────┴─────────┴─────────┴─────────────────────────────┴─────────┴─────────┴─────────┘
 *
 *              ┌───────┐         ┌───────┐                                       ┌───────┬───────┐
 *      ┌───────┤       ├───────┐ │       │                                       │       │       │
 *      │       ├───────┤       │ └───┬───┴───┐                                   └───┬───┴───┬───┴───┐
 *      └───────┤       ├───────┘     │       │                                       │       │       │
 *              └───────┘             └───────┘                                       └───────┴───────┘
 *  ┌───────────┐ ┌─────────┬─────────┐           ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┐
 *  │           │ │  PSCR   │  PAUS   │           │ MUTE  │       │       │       │ F11   │ F12   │       │
 *  └─┬───────┬─┴─┴───┬─────┴─┬───────┼───────┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───────┤
 *    │       │  F1   │  F2   │  F3   │  F4   │  F5   │  F6   │  F7   │  F8   │  F9   │ F10   │   DEL     │
 *  ┌─┴───────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────────┘
 *  │ CAPSLOCK  │       │       │       │       │       │       │ PGUP  │ INS   │       │       │
 *  └─┬───────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────┬─┴─────────┐
 *    │       │       │       │       │       │       │ HOME  │ PGDN  │ END   │       │       │           │
 *  ┌─┴───────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─────┬─────┘
 *  │           │       │       │       │       │       │       │       │ BRID  │ BRIU  │           │
 *  └─────┬─────┴───┬───┴─────┬─┴───────┼───────┴───────┴───────┴─────┬─┴───────┼───────┴─┬─────────┤
 *        │         │         │  CMD    │          BL_STEP            │  CMD    │         │         │
 *        └─────────┴─────────┴─────────┴─────────────────────────────┴─────────┴─────────┴─────────┘
 */

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_DEF] = LAYOUT(
                    KC_UP,                  KC_BTN1,                                                            JS_3,       JS_2,
        KC_LEFT,    KC_DOWN,    KC_RIGHT,       KC_BTN2,                                                                JS_1,       JS_0,
        KC_ESC,          KC_SLCT,          KC_APP,                      KC_VOLU,    KC_LBRC,   KC_RBRC, KC_SLSH,    KC_MINS,   KC_EQL,   KC_BSLS,
        KC_GRV,      KC_1,      KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_BSPC,
        KC_TAB,          KC_Q,      KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,
        KC_QUOT,     KC_A,      KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_RETN,
        KC_LSFT,         KC_Z,      KC_X,       KC_C,       KC_V,      KC_B,        KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_RSFT,
            OSL(_FN),       KC_LCTL,    KC_LALT,                            KC_SPC,                     KC_RALT,    KC_RCTL,    OSL(_FN)
    ),
    [_FN] = LAYOUT(
                    _______,                _______,                                                            _______,    _______,
        _______,    _______,    _______,        _______,                                                                _______,    _______,
        _______,         KC_PSCR,          KC_PAUS,                     KC_MUTE,    _______,   _______, _______,    KC_F11,   KC_F12,    _______,
        _______,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,       KC_F7,       KC_F8,       KC_F9,    KC_F10,    KC_DEL,
        KC_CAPS,         _______,   _______,    _______,    _______,    _______,    _______,    KC_PGUP,    KC_INS,     _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    KC_HOME,    KC_PGDN,    KC_END,     _______,    _______,    _______,
        _______,         _______,   _______,    _______,    _______,    _______,    _______,    _______,    KC_BRID,    KC_BRIU,    _______,
            _______,        _______,    KC_LGUI,                            BL_STEP,                    KC_RGUI,    _______,    _______
    ),
};
// clang-format on

// Override volume up to be volume down when shift is held
const key_override_t vol_up_override = ko_make_basic(MOD_MASK_SHIFT, KC_VOLU, KC_VOLD);

// Assign overrides (null-terminated array of pointers!)
// clang-format off
const key_override_t **key_overrides = (const key_override_t *[]){
    &vol_up_override,
    NULL
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
layer_state_t layer_state_set_kb(layer_state_t state) {
    // Switch trackball mode when layer is turned on
    if (IS_LAYER_ON_STATE(state, _FN)) {
        trackball_mode_set(TRACKBALL_MODE_SCROLL);
    } else {
        trackball_mode_set(TRACKBALL_MODE_MOUSE);
    }
    return state;
}
#endif
