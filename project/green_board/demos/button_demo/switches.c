#include <msp430.h>
#include "led.h"
#include "switches.h"
#include "stateMachines.h"

char switch_state_down_button_1, switch_state_down_button_2, switch_state_down_button_3;
char switch_state_down_button_4, switch_state_changed; /* effectively boolean */

static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
  state = 2; 
  led_update_switch();
}

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  
  switch_state_down_button_1 = (p2val & SW1) ? 0: 1;
  switch_state_down_button_2 = (p2val & SW2) ? 0: 1;
  switch_state_down_button_3 = (p2val & SW3) ? 0: 1;
  switch_state_down_button_4 = (p2val & SW4) ? 0: 1;

  if (switch_state_down_button_1) {
    state_button_1 = 0;           /* This is to reset the binary state machine.*/
    state = 0;                    /* Setting to the first state.*/ 
  }
  if (switch_state_down_button_2) {
    state = 1; 
  }
  if (switch_state_down_button_3) {
    state = 2;
  }
  if (switch_state_down_button_4) {
    state = 3;
  }
  switch_state_changed = 1;
}
