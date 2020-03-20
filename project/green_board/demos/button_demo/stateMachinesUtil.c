#include <msp430.h>
#include "led.h"

void turn_on() {
  red_led_state = 1;
  green_led_state = 1;
}

void turn_off() {
  red_led_state = 0;
  green_led_state = 0;
}
