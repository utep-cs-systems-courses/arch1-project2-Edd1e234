#ifndef stateMachine_included
#define stateMachine_included

// These should only be changed in switches.c or maybe interrupt function

extern int periods[], time_set[];
extern int value;
extern char state, state_button_1, state_button_2, state_button_3, state_button_4;
static char FINAL; 
void state_advance();

#endif
