#include <avr/io.h>
#include "led_output.h"
#include "main.h"


/**
configures the relevant output pins as outputs for driving the LEDs
*/
void setup_ports()
{

}

/**
sets the led state for the given channel. Channel index is zero-based.
*/
void set_indicator(uint8_t channel, uint8_t red, uint8_t grn)
{
  if(channel>=CHANNEL_COUNT) return;

  switch(channel){
    case 0:
      if(red){
        CHANNEL_0_PORT&=CHANNEL_0_RED;
      } else {
        CHANNEL_0_PORT|=~CHANNEL_0_RED;
      }
      if(grn){
        CHANNEL_0_PORT&=CHANNEL_0_GRN;
      } else {
        CHANNEL_0_PORT|=~CHANNEL_0_GRN;
      }
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      break;
  }
}
