#define RGB_GREENISH_R 0x30
#define RGB_GREENISH_G 0x99
#define RGB_GREENISH_B 0x56
#define RGB_WHITE_R 0xFF
#define RGB_WHITE_G 0xFF
#define RGB_WHITE_B 0xFF

#include QMK_KEYBOARD_H
#include "keychron_common.h"

enum layers {
    GLOWNA,
    NUMERY
};
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [GLOWNA] = LAYOUT_ansi_90(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,       KC_F10,        KC_F11,     KC_F12,     QK_MACRO_2, KC_MEDIA_NEXT_TRACK, KC_MEDIA_PLAY_PAUSE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,        KC_MINS,       KC_EQL,     KC_BSPC,    KC_INS,     KC_HOME,             KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,        KC_LBRC,       KC_RBRC,    KC_BSLS,    KC_DEL,     KC_END,              KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,     KC_QUOT,                   KC_ENT,     QK_MACRO_0, QK_MACRO_1,          KC_PRINT_SCREEN,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,      KC_SLSH,                   KC_RSFT,                KC_UP,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,     KC_MUTE,       QK_MACRO_3, KC_RCTL,    KC_LEFT,    KC_DOWN,             KC_RGHT),
	[NUMERY] = LAYOUT_ansi_90(
		_______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_KP_SLASH, KC_KP_ASTERISK,KC_KP_MINUS,KC_KP_PLUS, KC_7,       KC_8,                KC_9,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,     _______,       _______,    _______,    KC_4,       KC_5,                KC_6,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,     _______,       _______,    _______,    KC_1,       KC_2,                KC_3,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,     _______,                   _______,    KC_DEL,     KC_0,                KC_DOT,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,     _______,                   _______,                _______,
        _______,  _______,  _______,                                _______,                                _______,     _______,       _______,    _______,    _______,    _______,             _______),
};

// clang-format on
bool macro_0(void) {
    tap_code16(C(KC_INS)); // Ctrl + INS
    return false;
}

bool macro_1(void) {
    tap_code16(S(KC_INS)); // Shift + INS
    return false;
}

bool macro_2(void) {
    tap_code(KC_CALCULATOR);
    layer_invert(1);
    return false;
}

bool macro_3(void) {
    layer_invert(1);
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }

    if (record->event.pressed) {
        switch (keycode) {
            case QK_MACRO_0: return macro_0();
            case QK_MACRO_1: return macro_1();
            case QK_MACRO_2: return macro_2();
            case QK_MACRO_3: return macro_3();
        }
    }
    return true;
}

bool rgb_matrix_indicators_user(void) {
    uint8_t layer = get_highest_layer(layer_state);
    uint8_t total_leds = RGB_MATRIX_LED_COUNT;
    uint8_t special_index = total_leds - 5;

    if (layer == 0) {
        for (uint8_t i = 0; i < total_leds; i++) {
            if (i == special_index) {
                rgb_matrix_set_color(i, RGB_WHITE_R * 0.4, RGB_WHITE_G * 0.4, RGB_WHITE_B * 0.4);
            } else {
                rgb_matrix_set_color(i, RGB_GREENISH_R * 0.4, RGB_GREENISH_G * 0.4, RGB_GREENISH_B * 0.4);
            }
        }
    } else if (layer == 1) {
        snled27351_set_color_all(0, 0, 0);
        snled27351_flush();
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                uint8_t index = g_led_config.matrix_co[row][col];
                if (index >= total_leds) continue;

                uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){ .row = row, .col = col });
                if (keycode != KC_TRNS) {
                    rgb_matrix_set_color(index, RGB_WHITE_R, RGB_WHITE_G, RGB_WHITE_B);
                }
            }
        }

        rgb_matrix_set_color(special_index, RGB_GREENISH_R, RGB_GREENISH_G, RGB_GREENISH_B);
    }

    return false;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return state;
}
