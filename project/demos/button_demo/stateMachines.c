
#include <msp430.h>
#include "stateMachines.h"
#include "led.h"

char toggle() {
  static char state = 0;
}

void state_advance()
{
  led_changed = toggle();
  led_update(); 
}
