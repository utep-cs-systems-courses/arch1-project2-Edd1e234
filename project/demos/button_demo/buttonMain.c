#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "switches.h"

void main(void) 
{  
  configureClocks();
  switch_init();
  led_init();
  enableWDTInterrupts();
  
  or_sr(0x18);  // CPU off, GIE on
}
