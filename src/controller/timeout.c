#include <avr/interrupt.h>
#include "timeout.h"

int8_t timer_flags; //externally accessed by main module via extern in timeout.h
int8_t flash_ctr;

ISR(TIMER1_COMPA_vect){
  ++flash_ctr;
  if(flash_ctr>=SLOW_DIVIDER){
    timer_flags|=TMR_SLOWCLK;
    flash_ctr=0;
  }
  timer_flags|=TMR_FASTCLK;
}

ISR(TIMER1_COMPB_vect){
  //unused
}

ISR(TIMER1_OVF_vect){
  //unused
}

ISR(TIMER1_CAPT_vect){
  //unused
}

/**
sets up TIMER1 as our timeout timer.
An interrupt is generated at a 10Hz frequency; this contains its own timer
to subdivide and generate LED flashes
*/
void setup_timeout()
{
  //assume 1Mhz clock.  so 1024 divider => 1kHz increment.
  // compare match at 512 means 1/2 second. = 2Hz
  // compare match at 128 means 1/10 second = 10Hz

  OCR1AH = 0xF0;  //combined, set compare match at 128 then we reset.
  OCR1AL = 0;
  TIMSK1 = (1<<OCIE1A) | (1<<OCIE1B); //activate interrupt at output compare match on chl A
  TCCR1A = 0; //normal waveform gen, no pin output
  //TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10); //activate 1024 divider and CTC mode
  TCCR1B = (1<<WGM12) | (1<<CS10);  //no divider. In actuality TMR_SLOWCLK is ~1/2 sec for this, so FASTCLK is ~1/10s.
  //implies actual clock is ~1kHz
}
