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

void led_update_switch() {

    char ledFlags = redVal[red_led_state] | greenVal[green_led_state];

    P1OUT &= (0xff - LEDS) | ledFlags;
    P1OUT |= ledFlags;
    switch_state_changed = 0; 
}
