#include <avr/io.h>
#include "led_output.h"
#include "main.h"
#include "registers.h"

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
set appropriate LED state for controller channel flags
*/
void set_from_channel_flag(uint8_t channel, int8_t flag)
{
  switch(flag){
    case CF_INACTIVE:
      set_indicator(channel, 0,0);
      break;
    case CF_ACTIVE:
      set_indicator(channel, 0, 1);
      break;
    case CF_OVERRIDDEN:
      set_indicator(channel, 1, 0);
      break;
    default: break;
  }
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

/**
update channels indicator based on controller flags.
*/
void update_indicator_flags(uint8_t channel, uint8_t flags, uint8_t flashvalue)
{
  if(flags&CF_ACTIVE) set_indicator(channel, 0, 1); //active => green light
  if(flags&CF_OVERRIDDEN) set_indicator(channel, 1, 0); //overriden => red light
}

/**
update channel's indicator if inactive, to the given flashvalue.
this is used to flash aspects on and off when an offer is pending.
*/
void update_indicator_flashing(uint8_t channel, uint8_t flags, uint8_t flashvalue)
{
  if(flags==CF_INACTIVE) set_indicator(channel, flashvalue, 0);
}
