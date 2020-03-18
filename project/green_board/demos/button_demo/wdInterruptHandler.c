#include <msp430.h>
#include "stateMachines.h"

void
__interrupt_vec(WDT_VECTOR) WDT() {
  static char blink_count = 0;
  if (state==2) {
    if (++blink_count==1){
      state_advance();
      blink_count = 0;
    }
  } else if (state==3) {
    if (++blink_count==time_set[value]) {
      state_advance();
      blink_count = 0;
    }
  } else {
    if (++blink_count==185) {
      state_advance();
      blink_count = 0;
    }
  }
}
