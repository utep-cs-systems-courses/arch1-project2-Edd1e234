# Lab 2 Toy

This program is using the button demo as the a skeleton, hence the directory
name being 'button_demo'. 

This program utilizes all four buttons and the speaker on the MSP430 to do everything from blink to dim to even playing an actual song. To facilitate the various features or "states", you press a button of your choosing and it'll just activate a specific feature. Below we'll go through each button and how the code works. The MSP430 labels each button from SW1 to SW4, this how we will describe each button.

### States

To go from button to button I implemented a state machine, for example the fist button is state 0. If the user hits lets say button 3 then we'd go to state 2. Now when the program is set to a certain state the program is then routed to a substate of that state, this substate is what the output of the button pressed will be.   

## SW1

The first button activates a binary state machine.

_States_:  
* State 0
    * Both LEDS are off. To signal 0.
* State 1:
    * Red LED on. To signal 1.
* State 2:
    * Green Led on. To signal 2.
* State 3:
    * Both LED on. To signal 3. Goes back to state 0.

Will change states every 185 blink counts.

## SW2

Button 2 just blinks from Red led to Green led. This runs at about a 185 count.

The state machine is very simple.
_States_:  
* State 0
    * Red on, green off.
* State 1:
    * Red off, green on.

Goes back to state 0.


## SW3

This dims both LEDs. To do this it sets blink count to 1, every 4 it changes sets both red and green to on, but only for only a single blink count.

## SW4

This state utilizes the speaker to play a song, the song being played is runaway WestWorld addition. The actual version here: https://www.youtube.com/watch?v=AHnqvZBwk0Y.

Since every note is converted to a frequency and that frequency is
then converted to a period each period must be set. To do this is I
created 3 separate static arrays, the first being named period. This
will be the main melody. Since beats only last a certain amount of
time, the second array contains the amount of blink counts to hold a
certain pitch. Now the third array acts as the notes that play at the
same time, but since the speaker can't play two notes at the same time, these periods will play as soon blink count reaches the end of the cycle. For example, if the main note is 705, for 43 blink counts, and the second note is 1000. You'll hear 705 for 43 blink counts, then 1000 for 43 seconds.
