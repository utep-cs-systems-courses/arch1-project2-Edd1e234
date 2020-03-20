	.arch msp430g2553
	#include "led.h"
	.text

	;; Routines to access stuff.

	.global turn_on
turn_on:mov.b #1, &red_led_state
	mov.b #1, &green_led_state
	ret


	.global turn_off
turn_off:
	mov.b #0, &red_led_state
	mov.b #0, &green_led_state
	ret
