#ifndef stateMachine_included
#define stateMachine_included

// These should only be changed in switches.c.
extern const short periods[], periods_inv[], time_set[];

/* States will only be changed in switches.c*/
extern short state_button_1, state_button_2, state_button_3, state_button_4;
extern char state;
extern char value, blink_count_end;
const char FINAL; 
void state_advance();

#endif
