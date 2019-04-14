#include <avr/io.h>
#include "led_output.h"
#include "main.h"


/**
configures the relevant output pins as outputs for driving the LEDs
*/
void setup_ports()
{
  CHANNEL_0_DDR |= CHANNEL_0_RED|CHANNEL_0_GRN;
  CHANNEL_1_DDR |= CHANNEL_1_RED|CHANNEL_1_GRN;
  CHANNEL_2_DDR |= CHANNEL_2_RED|CHANNEL_2_GRN;
  CHANNEL_3_DDR |= CHANNEL_3_RED|CHANNEL_3_GRN;
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
        CHANNEL_0_PORT|=CHANNEL_0_RED;
      } else {
        CHANNEL_0_PORT&=~CHANNEL_0_RED;
      }
      if(grn){
        CHANNEL_0_PORT|=CHANNEL_0_GRN;
      } else {
        CHANNEL_0_PORT&=~CHANNEL_0_GRN;
      }
      break;
    case 1:
      if(red){
        CHANNEL_1_PORT|=CHANNEL_1_RED;
      } else {
        CHANNEL_1_PORT&=~CHANNEL_1_RED;
      }
      if(grn){
        CHANNEL_1_PORT|=CHANNEL_1_GRN;
      } else {
        CHANNEL_1_PORT&=~CHANNEL_1_GRN;
      }
      break;
    case 2:
    if(red){
      CHANNEL_2_PORT|=CHANNEL_2_RED;
    } else {
      CHANNEL_2_PORT&=~CHANNEL_2_RED;
    }
    if(grn){
      CHANNEL_2_PORT|=CHANNEL_2_GRN;
    } else {
      CHANNEL_2_PORT&=~CHANNEL_2_GRN;
    }
      break;
    case 3:
    if(red){
      CHANNEL_3_PORT|=CHANNEL_3_RED;
    } else {
      CHANNEL_3_PORT&=~CHANNEL_3_RED;
    }
    if(grn){
      CHANNEL_3_PORT|=CHANNEL_3_GRN;
    } else {
      CHANNEL_3_PORT&=~CHANNEL_3_GRN;
    }
      break;
    default:
      break;
  }
}
