	.arch msp430g2553
	#include "led.h"
	#include "stateMachnies.h"

	;; Routines for stateMachnies
	.text

	;; Turns on both led lights by moving 1s into them.
	.global turn_on
turn_on:mov.b #1, &red_led_state
	mov.b #1, &green_led_state
	ret
	
	;; Turns off both led lights by moving 0s into them.   
	.global turn_off
turn_off:
	mov.b #0, &red_led_state
	mov.b #0, &green_led_state
	ret
;;; Below is the state machine for toggle_button_1, with the first state
;;; 	being in routine 'toggle_button_1'. In simple terms its a binary
;;; 	state machine from 0 to 3.
	
end:
	mov.b #1, r12
	ret
	
state_three:
	cmp.b #3, &state_button_1
	JNE toggle_button_1
	mov.b #1, &red_led_state
	mov.b #1, &green_led_state
	mov.b #0, &state_button_1
	JMP end
	ret
	
state_two:
	cmp.b #2, &state_button_1
	JNE state_three
	mov.b #0, &red_led_state
	mov.b #1, &green_led_state
	mov.b #3, &state_button_1
	JMP end
	ret
	
state_one:
	cmp.b #1, &state_button_1
	JNE state_two
	mov.b #1, &red_led_state
	mov.b #2, &state_button_1
	JMP end
	ret

	;; First state. 
	.global toggle_button_1
toggle_button_1:
	cmp.b #0, &state_button_1
	JNE state_one
	mov.b #0, &red_led_state
	mov.b #0, &green_led_state
	mov.b #1, state_button_1
	JMP end
	ret
