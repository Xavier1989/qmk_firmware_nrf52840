#include "spacebarracecar.h"

#ifdef GERMAN_ENABLE
bool lshift = false;
bool rshift = false;
bool lshiftp = false;
bool rshiftp = false;
uint16_t lshift_timer = 0;
uint16_t rshift_timer = 0;

uint8_t prev_indx = 0;
uint16_t prev_kcs[6] = {0, 0, 0, 0, 0, 0};

bool esct = false;

void add_to_prev(uint16_t kc){
  for (int i=0; i<prev_indx; i++){
    if (kc == prev_kcs[i])
      return;
  }
  if (prev_indx == 6){
    for (int i=5; i>0; i--){
      prev_kcs[i] = prev_kcs[i-1];
    }
    prev_kcs[0] = kc;
  } else {
    prev_kcs[prev_indx] = kc;
    prev_indx++;
  }
}

void unreg_prev(void){
  if (prev_indx == 0)
    return;
  for (int i=0; i<prev_indx; i++){
    unregister_code(prev_kcs[i]);
  }
  prev_indx = 0;
}
#endif

// stuff for nav esc
bool navesc = false;
uint16_t navesc_timer = 0;
bool game = false;

void timer_timeout(void){
  #ifdef GERMAN_ENABLE
  lshiftp = false;
  rshiftp = false;
  #endif
  navesc = false;
}

bool process_record_userspace(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case CU_GAME:
    if(record->event.pressed) {
      game = !game;
    }
    return false;
  case KC_LGUI:
  case KC_RGUI:
    if (game)
      return false;
    else
      return true;
  case CU_NAV:
    if(record->event.pressed) {
      navesc = true;
      navesc_timer = timer_read();
      layer_on(_NAV);
    } else {
      if (timer_elapsed(navesc_timer) < 200 && navesc) {
        register_code(KC_ESC);
        unregister_code(KC_ESC);
      }
      layer_off(_NAV);
    }
  return false;

  #ifdef RGBLIGHT_ENABLE
  case CU_RGBV:
    if(record->event.pressed) {
      if (rgblight_get_val()+32>255)
        rgblight_sethsv(rgblight_get_hue(), rgblight_get_sat(), 31);
      else
        rgblight_sethsv(rgblight_get_hue(), rgblight_get_sat(), rgblight_get_val()+32);
    }
    return false;
  #endif

  #ifdef GERMAN_ENABLE
  case CU_LSFT:
    if(record->event.pressed) {
      lshiftp = true;
      lshift_timer = timer_read();
      unregister_code(KC_LSFT);
      register_code(KC_LSFT);
      lshift = true;
    } else {
      if (timer_elapsed(lshift_timer) < 200 && lshiftp && !game) {
        register_code(KC_LSFT);
        register_code(KC_8);
        unregister_code(KC_8);
        unregister_code(KC_LSFT);
      }
      unreg_prev();
      if (!rshift)
        unregister_code(KC_LSFT);
      lshift = false;
    }
    return false;
  case CU_RSFT:
    if(record->event.pressed) {
      rshiftp = true;
      rshift_timer = timer_read();
      unregister_code(KC_LSFT);
      register_code(KC_LSFT);
      rshift = true;
    } else {
      if (timer_elapsed(rshift_timer) < 200 && rshiftp && !game) {
        register_code(KC_LSFT);
        register_code(KC_9);
        unregister_code(KC_9);
        unregister_code(KC_LSFT);
      }
      unreg_prev();
      if (!lshift)
        unregister_code(KC_LSFT);
      rshift = false;
    }
    return false;
  case CU_ESCT:
    if(record->event.pressed) {
      esct = !esct;
    }
    return false;
  case CU_AE:
    UML(DE_AE)
  case CU_OE:
    UML(DE_OE)
  case CU_UE:
    UML(DE_UE)
  case CU_SS:
    if(record->event.pressed) {
      timer_timeout();
      unregister_code(KC_LSFT);
      register_code(DE_SS);
      unregister_code(DE_SS);
      if (lshift || rshift)
        register_code(KC_LSFT);
      layer_off(_DEADKEY);
    }
    return false;
  case CU_DDQ:
    if(record->event.pressed) {
      timer_timeout();
      register_code(KC_LSFT);
      register_code(KC_2);
      unregister_code(KC_2);
      if (!lshift && !rshift)
        unregister_code(KC_LSFT);
      layer_off(_DEADKEY);
    }
    return false;
  case CU_ED:
    if(record->event.pressed) {
      timer_timeout();
      layer_off(_DEADKEY);
    }
    return false;
  case CU_GRV:
    if(record->event.pressed) {
      timer_timeout();
      if (lshift || rshift){
        unregister_code(KC_LSFT);
        register_code(DE_ALGR);
        unregister_code(DE_PLUS);
        register_code(DE_PLUS);
        unregister_code(DE_PLUS);
        unregister_code(DE_ALGR);
        register_code(KC_LSFT);
      } else {
        register_code(KC_LSFT);
        unregister_code(DE_ACUT);
        register_code(DE_ACUT);
        unregister_code(DE_ACUT);
        unregister_code(KC_LSFT);
        if (!esct) {
          register_code(KC_SPC);
          unregister_code(KC_SPC);
        }
      }
    }
    return false;
  case CU_CIRC:
    if(record->event.pressed) {
      timer_timeout();
      unregister_code(KC_LSFT);
      unregister_code(DE_CIRC);
      register_code(DE_CIRC);
      unregister_code(DE_CIRC);
      if (!esct) {
          register_code(KC_SPC);
          unregister_code(KC_SPC);
      }
      if (lshift || rshift)
        register_code(KC_LSFT);
    }
    return false;
  case CU_QUOT:
    if(record->event.pressed){
      timer_timeout();
      register_code(KC_LSFT);
      if (lshift || rshift){
        layer_on(_DEADKEY);
      } else {
        unregister_code(DE_HASH);
        register_code(DE_HASH);
        add_to_prev(DE_HASH);
      }
    } else {
      unregister_code(DE_HASH);
      unreg_prev();
      if (lshift || rshift)
        register_code(KC_LSFT);
      else
        unregister_code(KC_LSFT);
    }
    return false;
  case CU_6:
    if(record->event.pressed){
      timer_timeout();
      unregister_code(KC_LSFT);
      if (lshift || rshift){
        unregister_code(DE_CIRC);
        register_code(DE_CIRC);
        unregister_code(DE_CIRC);
        if (!esct) {
          register_code(KC_SPC);
          unregister_code(KC_SPC);
        }
        register_code(KC_LSFT);
      } else {
        register_code(DE_6);
      }
    } else {
      unregister_code(DE_6);
    }
    return false;
  case CU_COMM:
    SHIFT_NO(DE_COMM, DE_LESS)
  case CU_DOT:
    SHIFT_NORM(DE_DOT, DE_LESS)
  case CU_SLSH:
    SHIFT_ALL(DE_7, DE_SS)
  case CU_SCLN:
    SHIFT_ALL(DE_COMM, DE_DOT)
  case CU_3:
    SHIFT_NO(DE_3, DE_HASH)
  case CU_7:
    SHIFT_NORM(DE_7, DE_6)
  case CU_8:
    SHIFT_NORM(DE_8, DE_PLUS)
  case CU_9:
    SHIFT_NORM(DE_9, DE_8)
  case CU_0:
    SHIFT_NORM(DE_0, DE_9)
  case CU_EQL:
    SHIFT_SWITCH(DE_0, DE_PLUS)
  case CU_LBRC:
    SHIFT_ALGR(DE_8, DE_7)
  case CU_RBRC:
    SHIFT_ALGR(DE_9, DE_0)
  case CU_BSLS:
    SHIFT_ALGR(DE_SS, DE_LESS)
  case CU_Z:
    CTRL(DE_Z, KC_Z)
  case CU_Y:
    CTRL(DE_Y, KC_Y)
  case KC_LCTL:
  case KC_RCTL:
    if(!record->event.pressed) {
      unregister_code(KC_Z);
      unregister_code(KC_Y);
    }
    return true;
  #endif

  default:
    if(record->event.pressed) {
      timer_timeout();

      #ifdef GERMAN_ENABLE
      if (lshift || rshift)
        register_code(KC_LSFT);
      else
        unregister_code(KC_LSFT);
      #endif

    }
    return true;
  }
}