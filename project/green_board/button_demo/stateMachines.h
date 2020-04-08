#ifndef stateMachine_included
#define stateMachine_included

// These should only be changed in switches.c.
extern const int periods[], periods_inv[], time_set[];

/* States will only be changed in switches.c*/
extern char state, state_button_2;
extern char state_button_3, state_button_4;
extern char value, blink_count_end;
const char FINAL; 
void state_advance();

#endif
