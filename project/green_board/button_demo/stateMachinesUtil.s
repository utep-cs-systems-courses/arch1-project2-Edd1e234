.arch msp430g2553
;; #include "led.h"
;; #include "stateMachnies.h"
	;; extern ints for stateMachnies(RAM)
	;; This is just is a state
	
	.text
;; Turns on both led lights by moving 1s into them.
	.global turn_on
turn_on:
	mov.b #1, &red_led_state
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

	.align 2
t1:
	.word t1_default	; t1[0]
	.word t1_option1	; t1[1]
	.word t1_option2	; t1[2]
	.word t1_option3	; t1[3]

	.global toggle_button_1
toggle_button_1:
	cmp #4, &state_button_1 ; state_button_1-4
	jhs t1_default

	mov &state_button_1, r12
	add r12, r12		; r12=2*state_button_1
	mov t1(r12), r0 	;

t1_option1:
	mov.b #1, &red_led_state
	mov #2, &state_button_1
	jmp out

t1_option2:
	mov.b #0, &red_led_state
	mov.b #1, &green_led_state
	mov #3, &state_button_1
	jmp out

t1_option3:
	mov.b #1, &red_led_state
	mov.b #1, &green_led_state
	mov #0, &state_button_1
	jmp out

t1_default:			; first case
	mov.b #0, &red_led_state
	mov.b #0, &green_led_state
	mov #1, &state_button_1
	jmp out

	.align 2
t2:
	.word t2_default 	;t2[0]
	.word t2_option1	;t2[1]

	.global
toggle_button_2:
	cmp #2, &state_button_2
	jhs t2_default

	mov &state_button_2, r12
	add r12, r12
	mov t2(r12), r0

t2_option:			; this is the second state
	mov.b #0, &green_led_state
	mov.b #1, &red_led_state
	mov #0, &state_button_2
	jmp out
	
t2_default:
	mov.b #1, &green_led_state
	mov.b #0, &red_led_state
	mov #1, &state_button_2
	jmp out
	
	
out:
	pop r0
