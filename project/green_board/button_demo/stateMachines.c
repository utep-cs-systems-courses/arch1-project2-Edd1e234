#include "stateMachines.h"
#include "stateMachinesUtil.h"
#include "led.h"
#include "buzzer.h"

char state = 0;
short state_button_1 = 0; 
short state_button_2 = 0;
short state_button_3 = 0;
short state_button_4 = 0;

// Index for all three arrays. 
char value = 0;

// Should only be changed in stateMachines.c, used in wdInterruptHandler.c 
char blink_count_end = 185;

// When to restart song.
const char FINAL = 195; 
const int periods[] = {758, 758, 758, 1517, 803, 803, 1607, 902, 902, 902, 1804, 1136, 1136, 758,
		 4816, 6428, 4816, 4050, // Set 17
		 4816, 6428, 4816, 4050,
		 4816, 6428, 4816, 4050, // Set 18
		 4816, 6428, 4816, 4050, 
		 4816, 6428, 4816, 4050, // Set 19
		 4816, 6428, 4816, 4050,
		 4816, 6428, 4816, 4050, // Set 20
		 4816, 6428, 4816, 4050, // 46
		 4816, 6428, 4816, 4050, // Set 21
		 4816, 6428, 4816, 4050, // 54
		 4816, 6428, 4816, 4050, // Set 22
		 4816, 6428, 4816, 4050, // 58
		 4816, 6428, 4816, 4050, // Set 23
		 4816, 6428, 4816, 4050, // 62
		 4816, 6428, 4816, 4050, // Set 24
		 4816, 6428, 4816, 4050, // 66
		 6068, 7216, 6068, 4816, // Set 25
		 6068, 7216, 6068, 4816, // 70
		 6068, 7216, 6068, 4816, // Set 26
		 6068, 7216, 6068, 4816, // 74
		 6068, 7216, 6068, 4816, // Set 27
		 6068, 7216, 6068, 4816, // 78
		 6068, 7216, 6068, 4816, // Set 28
		 6068, 7216, 6068, 4816, // 82
		 6068, 7216, 6068, 4816, // Set 29
		 6068, 7216, 6068, 4816, // 86
		 6068, 7216, 6068, 4816, // Set 30
		 6068, 7216, 6068, 4816, // 90
		 6068, 7216, 6068, 4816, // Set 31
		 6068, 7216, 6068, 4816,
		 0, 0, 			 // End Set 32 and Set 33
		 2408, 2408,		 // Set 34
		 1517, 2025, 0, 0, 	 // Set 35
		 2273, 2273, 2408, 2408,
		 1517, 2025, 0, 0, 	 // Set 36
		 2273, 2273, 2408, 2408,
		 1607, 1804, 1607, 0,	 // Set 37
		 0, 0, 0, 0,
		 0, 0, 0, 0,		 // Set 38
		 0, 0, 1204, 1204,
		 1204, 1351, 1351, 1351, // Set 39
		 1351, 1517, 1517, 1351,
		 1351, 1351, 1517, 1517, // Set 40
		 1351, 1351, 1204, 1351,
		 1517, 0, 0, 0,          // Set 41
		 0, 0, 0, 0
};
// First set is 14.
const int time_set[] = {188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188,
		  43, 43, 43, 43, // Set 17
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 18
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 19
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 20
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 21
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 22
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 23
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 24
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 25
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 26
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 27
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 28
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 29
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 30
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 31
		  43, 43, 43, 43,
		  94, 43,         // End of Set 32 and Set 33
		  15, 15,	  // Set 34
		  30, 43, 43, 43, // Set 35
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 36
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 37
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 38
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 39
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 40
		  43, 43, 43, 43,
		  43, 43, 43, 43, // Set 41
		  43, 43, 43, 43
};

// Down Beats. 
const int periods_inv[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			   9631, 0, 0, 0, // Set 17
			   758, 0, 0, 0,
			   9631, 0, 0, 0, // Set 18
			   758, 0, 0, 0,
			   9631, 0, 0, 0, // Set 19
			   758, 0, 0, 0,
			   9631, 0, 0, 0, // Set 20
			   1517, 0, 0, 0,
			   10000, 0, 0, 0, // Set 21
			   803, 0, 0, 0,
			   10000, 0, 0, 0, // Set 22
			   803, 0, 0, 0,
			   10000, 0, 0, 0, // Set 23
			   803, 0, 0, 0,
			   10000, 0, 0, 0, // Set 24
			   1607, 0, 0, 0,
			   12130, 0, 0, 0, // Set 25
			   902, 0, 0, 0,
			   12130, 0, 0, 0, // Set 26
			   902, 0, 0, 0,
			   12130, 0, 0, 0, // Set 27
			   902, 0, 0, 0,
			   12130, 0, 0, 0, // Set 28
			   1804, 0, 0, 0,
			   14290, 0, 0, 0, // Set 29
			   1136, 0, 0, 0,
			   14290, 0, 0, 0, // Set 30
			   1136, 0, 0, 0,
			   14290, 0, 0, 0, // Set 31
			   1204, 0, 0, 804,
			   0, 0,	   // Final set Set 32 Set 33
			   0, 0,  	   // Set 34, Set 139
			   3822, 3822, 1517, 1204, // Set 35
			   1517, 1910, 1517, 1204,
			   1517, 1910, 1517, 1204, // Set 36
			   1517, 1910, 1517, 1204,
			   3822, 1607, 1517, 1204, // Set 37
			   1517, 2025, 1517, 1204,
			   1517, 2025, 1517, 1204, // Set 38
			   1517, 2025, 1517, 1204,
			   1517, 2025, 1517, 1204, // Set 39
			   1517, 2025, 1517, 1204,
			   1517, 2025, 1517, 1204, // Set 40
			   1517, 2025, 1517, 1204,
			   14290, 14290, 6068, 4816, // Set 41 Final....
			   6068, 7216, 6068, 4816
};

/* Just sets values based on current state.*/
void set_values(char v, char buzzer, char blink) {
  value = v;
  buzzer_set_period(0);
  blink_count_end = blink; 
}

void state_advance()
{
  switch (state) {
  case 0:
    set_values(0, 0, 185);
    toggle_button_1();
    break;
  case 1:
    set_values(0, 0, 185); 
    toggle_button_2();
    break;
  case 2:
    set_values(0, 0, 1);
    toggle_button_3();
    break;
  case 3:
    toggle_button_4();
    blink_count_end = time_set[value];
  }
  leds_changed = 1;
  led_update_switch();
}
