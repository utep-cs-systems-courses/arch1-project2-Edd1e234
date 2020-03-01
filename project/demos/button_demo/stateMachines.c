#include <msp430.h>
#include "stateMachines.h"
#include "led.h"

char toggle() {
  static char state = 0;

  switch(state) {
  case 0:
    red_led_state = 0;
    green_led_state = 0;
    state = 1;
    break;
  case 1:
    red_led_state = 1;
    state = 2;
    break;
  case 2:
    red_led_state = 0;
    green_led_state = 1;
    state = 3;
    break;
  case 3:
    red_led_state = 1;
    green_led_state = 1;
    state = 0;
    break;
  }
  return 1;
}

void state_advance()
{
  leds_changed = toggle();
  led_update();
}
