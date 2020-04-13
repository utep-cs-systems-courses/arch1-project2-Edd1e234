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
	.word t1_default		; t1[0]
	.word t1_option1		; t1[1]
	.word t1_option2		; t1[2]
	.word t1_option3		; t1[3]

	.global toggle_button_1
toggle_button_1:
	cmp #4, &state_button_1 	; state_button_1-4
	jc t1_default
	
	mov &state_button_1, r12
	add r12, r12			; r12=2*state_button_1
	mov t1(r12), r0 	
t1_default:
	mov.b #0, &red_led_state
	mov.b #0, &green_led_state
	mov #1, &state_button_1
	jmp out
t1_option1:				; turns on red led
	mov.b #1, &red_led_state
	mov #2, &state_button_1
	jmp out

t1_option2:				; turns off red led, turns on green 
	mov.b #0, &red_led_state
	mov.b #1, &green_led_state
	mov #3, &state_button_1
	jmp out

t1_option3:				; turns on both led 
	mov.b #1, &red_led_state
	mov.b #1, &green_led_state
	mov #0, &state_button_1
	jmp out

	.align 2
t2:
	.word t2_default 		;t2[0]
	.word t2_option			;t2[1]

	.global toggle_button_2
toggle_button_2:
	cmp #2, &state_button_2		;2-state_button_2
	jc t2_default			; if state_button_2 == 0

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

;;; These sets of functions dim ligts. Keeps lights off for the first four
;;; states. On the final state turns lights on. Turns both LEDs on, on final
;;; states. 

t3:
	.word t3_default 		;t3[0]
	.word t3_option1		;t3[1]
	.word t3_option2		;t3[2]
	.word t3_option3		;t3[3]
	.word t3_option4		;t4[4]

	.global toggle_button_3
toggle_button_3:
	cmp #5, &state_button_3		; 5-state_button_3	
	jc t3_default			; if state_button_3 = 0

	mov &state_button_3, r12
	add r12, r12
	mov t3(r12), r0			; puts places next case into stack 

t3_default:				; sets to 0. 
	mov.b 0, &red_led_state
	mov.b 0, &green_led_state
	mov #1, &state_button_3
	jmp out

t3_option1:				; maintains the state
	mov #2, &state_button_3
	jmp out
	
t3_option2:				; maintains the state 
	mov #3, &state_button_3
	jmp out
	
t3_option3:				; maintains the state 
	mov #4, &state_button_3
	jmp out

t3_option4:				; turns on LEDS 
	mov.b #1, &green_led_state
	mov.b #1, &red_led_state
	mov #0, &state_button_3
	jmp out

;;; this function contains the state machine that transitions
;;; the song from note to note. On donw beats lights will turn on.

	.align 2

t4:
	.word t4_default		;t4[0]
	.word t4_option			;t4[1]

	.global toggle_button_4
toggle_button_4:
	cmp.b &value, &FINAL		;if value == FINAL
	jeq value_is_FINAL		;if true jumps to value_is_FINAL

	cmp #2, &state_button_4		;state_button_4-2
	jc t4_default

	mov &state_button_4, r12
	add r12, r12			;r12=2*state_button_4
	mov t4(r12), r0 

	;; movs 0 into value
value_is_FINAL:
	mov.b #0, &value
	ret

t4_default:
	mov.b &value, r13	;value is index.
	add.b r13, r13		;value is char, must go twice as far.
	mov periods(r13), r12
	call #buzzer_set_period

	call #turn_off

	mov #1, &state_button_4
	jmp out
	
t4_option:				;second case. 
	mov.b &value, r13		; movs value into r13
	add.b r13, r13			; r13=2*value, this is because value is char 
	mov periods_inv(r13), r12	; accesses periods_inv[value] movs into r12
	call #buzzer_set_period	

	add.b #1, &value		; increamnts value 

	call #turn_on
	mov #0, &state_button_4		; movs 0 into state_button 
	jmp out

	.global set_values
set_values:				; used as a helper function
	mov.b r12, &value		; sets value
	mov #0, r12
	call #buzzer_set_period		; makes sure buzzer is off
	mov.b r13, &blink_count_end 	; sets blink_count

out:
	pop r0
