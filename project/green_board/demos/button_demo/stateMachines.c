#include <msp430.h>
#include "stateMachines.h"
#include "stateMachinesUtil.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"

char state = 0;
char state_button_1 = 0;  // This will only be changed by the switch interrupt handler.
char state_button_2 = 0;
char state_button_3 = 0;
char state_button_4 = 0;

static char start = 125;
static char FINAL = 137;

/*First part ends at 16.*/
const int periods[] = {758, 758, 758, 1517, 803, 803, 1607, 902, 902, 902, 1804, 1136, 1136, 758,
		 4816, 6428, 4816, 4050, // Set 17
		 4816, 6428, 4816, 4050,
		 4816, 6428, 4816, 4050, // Set 18
		 4816, 6428, 4816, 4050, // This is also 30.
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
		 6068, 7216, 6068, 4816, // 94
		       804, 0, 5000										 // 95, Final for now.

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
			94, 43,         // End of 32 and 33
			188							// Final for now.

};

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
					12130, 0, 0, 0, // Set 29
					1136, 0, 0, 0,
					12130, 0, 0, 0, // Set 30
					1136, 0, 0, 0,
					12130, 0, 0, 0, // Set 31
					1204, 0, 0, 804,
			   0, 0, 0  // Final: Set 32
};
int value = 5;

void state_advance()
{
  switch (state) {
  case 0:
    value = start;
    buzzer_set_period(0);
    leds_changed = toggle_button_1();
    break;
  case 1:
    value = start;
    buzzer_set_period(0);
    leds_changed = toggle_button_2();
    break;
  case 2:
    value = start;
    buzzer_set_period(0);
    leds_changed = toggle_button_3();
    break;
  case 3:
    leds_changed = toggle_button_4();
  }
  led_update_switch();
}
