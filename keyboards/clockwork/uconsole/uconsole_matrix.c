// Copyright 2023 Andi Powers-Holmes (@neggles)
// SPDX-License-Identifier: GPL-2.0-or-later
#include "quantum.h"
#include <hal_pal.h>
#include "matrix.h"
#include "debug.h"
#include "uconsole.h"

//----------------------------------------------------------
// Pin definitions
#ifdef MATRIX_ROW_PINS
static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
#endif // MATRIX_ROW_PINS
#ifdef MATRIX_COL_PINS
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
#endif // MATRIX_COL_PINS
#ifdef MATRIX_KEY_PINS
static const pin_t key_pins[DIRECT_ROWS][MATRIX_COLS] = MATRIX_KEY_PINS;
#endif

/* matrix state(1:on, 0:off) */
extern matrix_row_t raw_matrix[MATRIX_ROWS]; // raw values
extern matrix_row_t matrix[MATRIX_ROWS];     // debounced values

//----------------------------------------------------------
// Inlined helpers
static inline void setPinOutput_writeLow(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinLow(pin);
    }
}

static inline void setPinOutput_writeHigh(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinHigh(pin);
    }
}

static inline void setPinInputHigh_atomic(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinInputHigh(pin);
    }
}

static inline uint8_t readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return (readPin(pin) == MATRIX_INPUT_PRESSED_STATE) ? 0 : 1;
    } else {
        return 1;
    }
}

//----------------------------------------------------------
// Generics copied from quantum/matrix.c
static bool select_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
        setPinOutput_writeLow(pin);
        return true;
    }
    return false;
}

static void unselect_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
        setPinInputHigh_atomic(pin);
    }
}

void matrix_init_custom(void) {
    wait_ms(500);

    /* Set up row pins */
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        unselect_row(i);
    }

    /* Set up column pins */
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        pin_t pin = col_pins[i];
        if (pin != NO_PIN) {
            setPinInputHigh_atomic(pin);
        }
    }
    /* Set up direct pins */
    for (uint8_t x = 0; x < DIRECT_ROWS; x++) {
        for (uint8_t y = 0; y < MATRIX_COLS; y++) {
            pin_t pin = key_pins[x][y];
            if (pin != NO_PIN) {
                dprintf("Setting direct pin %d.%d to input high\n", x, y);
                setPinInputHigh_atomic(pin);
            }
        }
    }
}

static void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    if (!select_row(current_row)) {
        uint8_t row_offset = (current_row * MATRIX_COLS);
        current_row_value  = ~(uint8_t)((palReadPort(GPIOB) >> row_offset) & 0xff);
    } else {
        // Row selection succeeded
        matrix_output_select_delay();

        // For each col...
        matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
        for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1) {
            uint8_t pin_state = readMatrixPin(col_pins[col_index]);
            // Populate the matrix row with the state of the col pin
            current_row_value |= pin_state ? 0 : row_shifter;
        }

        // Unselect row
        unselect_row(current_row);
        matrix_output_unselect_delay(current_row, current_row_value != 0);
    }
    // Update the matrix
    current_matrix[current_row] = current_row_value;
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    static matrix_row_t temp_matrix[MATRIX_ROWS] = {0};

    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        matrix_read_cols_on_row(temp_matrix, current_row);
    }

    // Check if we've changed, return the last-read data
    bool changed = memcmp(current_matrix, temp_matrix, sizeof(temp_matrix)) != 0;
    if (changed) {
        memcpy(current_matrix, temp_matrix, sizeof(temp_matrix));
    }
    return changed;
}
