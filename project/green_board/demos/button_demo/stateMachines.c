#include <msp430.h>
#include "stateMachines.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"

char state = 0; 
char state_button_1 = 0;  // This will only be changed by the switch interrupt handler.
char state_button_2 = 0;
char state_button_3 = 0;
char state_button_4 = 0;

static char FINAL = 66;
static int HALF_NOTE = 188;
static int eighth_note = 35;

/*First part ends at 16.*/
int periods[] = {758, 758, 758, 1517, 803, 803, 1607, 902, 902, 902, 1804, 1136, 1136, 758,
		 4816, 4900, 4816, 4050, // Set 17
		 4816, 4900, 4816, 4050,
		 4816, 4900, 4816, 4050, // Set 18
		 4816, 4900, 4816, 4050, // This is also 30.
		 4816, 4900, 4816, 4050, // Set 19
		 4816, 4900, 4816, 4050,
		 4816, 4900, 4816, 4050, // Set 20
		 4816, 4900, 4816, 4050, // 46
		 4816, 4900, 4816, 4050, // Set 21
		 4816, 4900, 4816, 4050, // 54
		 4816, 4900, 4816, 4050, // Set 22
		 4816, 4900, 4816, 4050, // 58
		 4816, 4900, 4816, 4050, // Set 23
		 4816, 4900, 4816, 4050, // 62
		 4816, 4900, 4816, 4050, // Set 24
		 4816, 4900, 4816, 4050  // 66
		 
};
// First set is 14.
int time_set[] = {188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188,
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
		  43, 43, 43, 43
};

static int periods_inv[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			    5000, 0, 0, 0, // Set 17
			    758, 0, 0, 0, 
			    5000, 0, 0, 0, // Set 18
			    758, 0, 0, 0,
			    5000, 0, 0, 0, // Set 19
			    758, 0, 0, 0,
			    5000, 0, 0, 0, // Set 20
			    1517, 0, 0, 0,
};
int value = 5;

void turn_on() {
  red_led_state = 1;
  green_led_state = 1;
}

void turn_off() {
  red_led_state = 0;
  green_led_state = 0;
}

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
    value = 0; 
    buzzer_set_period(0);
    leds_changed = toggle_button_1();
    break;
  case 1:
    value = 0;
    buzzer_set_period(0);
    leds_changed = toggle_button_2();
    break;
  case 2:
    value = 0; 
    buzzer_set_period(0);
    leds_changed = toggle_button_3();
    break;
  case 3:
    leds_changed = toggle_button_4();
  }
  led_update_switch();
}
