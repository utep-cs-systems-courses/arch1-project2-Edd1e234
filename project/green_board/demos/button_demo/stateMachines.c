#include <msp430.h>
#include "stateMachines.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"

char state = 0; 
char state_button_1 = 0;  // This will only be changed by the switch interrupt handler.
char state_button_2 = 0;
char state_button_3 = 0;
char state_button_4 = 0;

int periods[] = {804, 804, 804};
int time_set[] = {188, 188, 188};
int value = 0; 

/* Binary State machine, only for button one and is the starting case.*/
char toggle_button_1() {
  switch(state_button_1) {
  case 0:
    red_led_state = 0;
    green_led_state = 0;
    state_button_1 = 1;
    break;
  case 1:
    red_led_state = 1;
    state_button_1 = 2;
    break;
  case 2:
    red_led_state = 0;
    green_led_state = 1;
    state_button_1 = 3;
    break;
  case 3:
    red_led_state = 1;
    green_led_state = 1;
    state_button_1 = 0;
    break;
  }
  return 1;
}

char toggle_button_2() {
    if (state_button_2) {
      green_led_state = 1;
      red_led_state = 0;
      state_button_2 = 0; 
    } else {
      red_led_state = 1;
      green_led_state = 0;
      state_button_2 = 1;
    }
  return 1; 
}

char toggle_button_3() {
  switch (state_button_3) {
  case 0:
    green_led_state = 0;
    red_led_state = 0;
    state_button_3 = 1; 
    break;
  case 1:
    green_led_state = 0;
    red_led_state = 0;
    state_button_3 = 2; 
    break;
  case 2:
    green_led_state = 0;
    red_led_state = 0;
    state_button_3 = 3;
    break;
  case 3:
    green_led_state = 0;
    red_led_state = 0;
    state_button_3 = 4;
    break;
  case 4:
    green_led_state = 0;
    red_led_state = 0;
    state_button_3 = 5;
    break;
  case 5:
    green_led_state = 1;
    red_led_state = 1;
    state_button_3 = 0;
  }
  return 1;
}

char toggle_button_4() {
  if (value==3) {
    buzzer_set_period(0);
  }else {
    switch(state_button_4) {
    case 0:
      buzzer_set_period(periods[value]);
      state_button_4 = 1;
      break;
    case 1:
      buzzer_set_period(0);
      value++;
      state_button_4 = 0; 
      break;
    }
  }
  return 1;
}

void state_advance()
{
  switch (state) {
  case 0:
    value = 0; 
    buzzer_set_period(0);
    leds_changed = toggle_button_1();
    break;
  case 1:
    value = 0;
    buzzer_set_period(0);
    leds_changed = toggle_button_2();
    break;
  case 2:
    value = 0; 
    buzzer_set_period(0);
    leds_changed = toggle_button_3();
    break;
  case 3:
    leds_changed = toggle_button_4();
  }
  led_update_switch();
}
