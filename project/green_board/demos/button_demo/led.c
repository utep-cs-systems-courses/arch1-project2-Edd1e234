#include <msp430.h>
#include "led.h"
#include "switches.h"

unsigned char green_led_state = 0, red_led_state = 0;
unsigned char leds_changed = 0;

static char redVal[] = {0, LED_RED}, greenVal[] = {0, LED_GREEN};

void led_init()
{
  P1DIR |= LEDS;		// bits attached to leds are output
  switch_state_changed = 1;
  led_update_switch();
}

void led_update(){
  if (switch_state_changed) {
    char ledFlags = 0; /* by default, no LEDs on */

    ledFlags |= switch_state_down_button_1 ? LED_GREEN : 0;
    ledFlags |= switch_state_down_button_1 ? 0 :LED_RED;

    P1OUT &= (0xff - LEDS) | ledFlags; // clear bits for off leds
    P1OUT |= ledFlags;         // set bits for on leds
  }
  switch_state_changed = 0;
}    

void led_update_switch() {

    char ledFlags = redVal[red_led_state] | greenVal[green_led_state];

    P1OUT &= (0xff - LEDS) | ledFlags;
    P1OUT |= ledFlags;
  switch_state_changed = 0; 
}
