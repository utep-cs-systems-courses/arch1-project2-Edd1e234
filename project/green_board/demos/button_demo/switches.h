#ifndef switches_included
#define switches_included

#define SW1 BIT0 /* switch1 is p1.3 */
#define SW2 BIT1
#define SWITCHES (SW1 | SW2)		/* only 1 switch on this board */

void switch_init();
void switch_interrupt_handler();

extern char switch_state_down_button_1, switch_state_down_button_2, switch_state_changed; /* effectively boolean */

#endif // included
