	.arch msp430g2553
	#include "led.h"
	#include "stateMachnies.h"

	;; extern ints for stateMachnies(RAM)
	.data
state_button_1:
	.word 0

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

out:
	pop r0
	
	.text
t1:	.word t1_default	; t1[0]
	.word t1_option1	; t1[1]
	.word t1_option2	; t1[2]
	.word t1_option3	; t1[3]
	
	.global toggle_button_1
toggle_button_1:
	;; range check on selector (state_button_1)
	cmp #4, &state_button_1 ; state_button_1-4
	jnc t1_default		; doesn't borrow if s > 3

	;; index into t1
	mov &state_button_1, r12
	add r12, r12		; r12=2*state_button_1
	mov t1(r12), r0 	;jmp jt(state_button_1)
	
t1_option1:	
	mov.b #0, &red_led_state
	mov.b #0, &green_led_state
	mov #1, &state_button_1
	JMP out
	
t1_option2:
	mov.b #1, &red_led_state
	mov #2, &state_button_1
	JMP out
	
t1_option3:
	mov.b #0, &red_led_state
	mov.b #1, &green_led_state
	mov #3, &state_button_1
	JMP out
	
t1_default:
	mov.b #1, &red_led_state
	mov 0, &state_button_1
	JMP out
