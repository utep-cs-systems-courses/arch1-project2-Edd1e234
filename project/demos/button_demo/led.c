#include <msp430.h>
#include "led.h"
#include "switches.h"

unsigned char red_on = 0, green_on = 0;
unsigned char led_changed = 0;

static char redVal[] = {0, LED_RED}, greenVal[] = {0, LED_GREEN};

void led_init()
{
  P1DIR |= LEDS;		// bits attached to leds are output
  switch_state_changed = 1;
  led_update();
}

void led_update(){
  if (switch_state_changed) {
    char ledFlags = 0; /* by default, no LEDs on */

    ledFlags |= switch_state_down ? LED_GREEN : 0;
    ledFlags |= switch_state_down ? 0 :LED_RED;

    P1OUT &= (0xff - LEDS) | ledFlags; // clear bits for off leds
    P1OUT |= ledFlags;         // set bits for on leds
  }
  switch_state_changed = 0;
}

char toggle()
{
  static char state = 0; 
  if (switch_state_changed) {
    switch (state) {
    case 0:
      state = 1; 
      return LED_GREEN;
    case 1:
      state = 0;
      return LED_RED;
    }
  }
}    

void led_update_switch() {

  if (switch_state_changed) {
    char ledFlags = redVal[red_on] | greenVal[green_on];

    P1OUT &= (0xff - LEDS) | ledFlags;
    P1OUT |= ledFlags;
  }
  switch_state_changed = 0; 
}
