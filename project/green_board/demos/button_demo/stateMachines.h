#ifndef stateMachine_included
#define stateMachine_included

extern char state, state_button_1, state_button_2, state_button_3; // These should only be changed in switches.c intterupt function.  
void state_advance();

#endif
