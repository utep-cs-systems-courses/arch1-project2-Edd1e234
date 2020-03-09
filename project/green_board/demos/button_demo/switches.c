#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "stateMachines.h"

char switch_state_down_button_1, switch_state_changed; /* effectively boolean */

static char 
switch_update_interrupt_sense()
{
  char p1val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p1val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p1val | ~SWITCHES);	/* if switch down, sense up */
  return p1val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
  led_update_switch();
}

void
switch_interrupt_handler()
{
  char p1val = switch_update_interrupt_sense();
  //  switch_state_down = (p1val & SW1) ? 0 : 1; /* 0 when SW1 is up */
  switch_state_down_button_1 = (p1val & SW2) ? 0 : 1;
  switch_state_changed = 1;
  state = 0;
}
