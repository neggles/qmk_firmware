#include "quantum.h"
#include "uconsole.h"

//----------------------------------------------------------
// Initialisation

void keyboard_post_init_kb(void) {
    keyboard_post_init_user();
}

void matrix_scan_kb(void) {
    matrix_scan_user();
}
