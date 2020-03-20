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

static char start = 0;
static char FINAL = 195;

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
		 0, 0, 									// End 32 and 33
		 2408, 2408,							// 34
		 1517, 2025, 0, 0, 				// 35
		 2273, 2273, 2408, 2408,
		 1517, 2025, 0, 0, 				// 36
		 2273, 2273, 2408, 2408,
		 1607, 1804, 1607, 0,			// 37
		 0, 0, 0, 0,
		 0, 0, 0, 0,							// 38
		 0, 0, 1204, 1204,
		 1204, 1351, 1351, 1351,	// 39
		 1351, 1517, 1517, 1351,
		 1351, 1351, 1517, 1517, // 40
		 1351, 1351, 1204, 1351,
		 1517, 0, 0, 0, 				// 41
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
			94, 43,         // End of 32 and 33
			15, 15,					//34
			30, 43, 43, 43,	// 35
			43, 43, 43, 43,
			43, 43, 43, 43, // 36
			43, 43, 43, 43,
			43, 43, 43, 43, // 37
			43, 43, 43, 43,
			43, 43, 43, 43, // 38
			43, 43, 43, 43,
			43, 43, 43, 43, // 39
			43, 43, 43, 43,
			43, 43, 43, 43, // 40
			43, 43, 43, 43,
			43, 43, 43, 43, // 41
			43, 43, 43, 43
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
					14290, 0, 0, 0, // Set 29
					1136, 0, 0, 0,
					14290, 0, 0, 0, // Set 30
					1136, 0, 0, 0,
					14290, 0, 0, 0, // Set 31
					1204, 0, 0, 804,
			    0, 0,	      		// Final set 32 33
					0, 0,  					// 34, 139
					3822, 3822, 1517, 1204, // 35
					1517, 1910, 1517, 1204,
					1517, 1910, 1517, 1204, // 36
					1517, 1910, 1517, 1204,
					3822, 1607, 1517, 1204, // 37
					1517, 2025, 1517, 1204,
					1517, 2025, 1517, 1204, // 38
					1517, 2025, 1517, 1204,
					1517, 2025, 1517, 1204, // 39
					1517, 2025, 1517, 1204,
					1517, 2025, 1517, 1204, // 40
					1517, 2025, 1517, 1204,
					14290, 14290, 6068, 4816, // 41 Final....
					6068, 7216, 6068, 4816
};
int value = 5;

/* Binary State machine, only for button one and is the starting case.*/
char toggle_button_1() {
  switch(state_button_1) {
  case 0:
    red_led_state = 0;
    green_led_state = 0;
    state_button_1 = 1;
    break;
  case 1:
    red_led_state = 1;
    state_button_1 = 2;
    break;
  case 2:
    red_led_state = 0;
    green_led_state = 1;
    state_button_1 = 3;
    break;
  case 3:
    red_led_state = 1;
    green_led_state = 1;
    state_button_1 = 0;
    break;
  }
  return 1;
}

char toggle_button_2() {
    if (state_button_2) {
      green_led_state = 1;
      red_led_state = 0;
      state_button_2 = 0;
    } else {
      red_led_state = 1;
      green_led_state = 0;
      state_button_2 = 1;
    }
  return 1;
}

char toggle_button_3() {
  switch (state_button_3) {
  case 0:
    green_led_state = 0;
    red_led_state = 0;
    state_button_3 = 1;
    break;
  case 1:
    green_led_state = 0;
    red_led_state = 0;
    state_button_3 = 2;
    break;
  case 2:
    green_led_state = 0;
    red_led_state = 0;
    state_button_3 = 3;
    break;
  case 3:
    green_led_state = 0;
    red_led_state = 0;
    state_button_3 = 4;
    break;
  case 4:
    green_led_state = 0;
    red_led_state = 0;
    state_button_3 = 5;
    break;
  case 5:
    green_led_state = 1;
    red_led_state = 1;
    state_button_3 = 0;
  }
  return 1;
}

char toggle_button_4() {
  if (value==FINAL) {
    buzzer_set_period(0);
  }else {
    switch(state_button_4) {
    case 0:
      buzzer_set_period(periods[value]);
      state_button_4 = 1;
      turn_off();
      break;
    case 1:
      buzzer_set_period(periods_inv[value]);
      value++;
      state_button_4 = 0;
      turn_on();
      break;
    }
  }
  return 1;
}

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
