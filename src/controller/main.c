#include <avr/interrupt.h>  //for sei()
#include <avr/sleep.h>      //for set_sleep_mode
#include <string.h>
#include "main.h"
#include "twi.h"
#include "address_finder.h"
#include "i2c_defs.h"
#include "registers.h"
#include "timeout.h"
#include "adc.h"

struct controller_state state;
int8_t reading_channel=0;

int main(void)
{
  unsigned char input_mode=0;
  unsigned char buffer[8];
  //load TWI address from DIP switches at PORTD
  int8_t twi_address = get_twi_address();

  memset(&state, 0, sizeof(struct controller_state));

  //enable TWI
  setup_twi(twi_address);
  //enable timer
  setup_timeout();
  //set up adc
  adc_setup();
  //enable interrupts
  sei();

  while(1){
    clear_rx_buffer();
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
    //if we have had a timeout
    if(timer_flags&TMR_FASTCLK){  //fast clock means start reading of a dial
      ++reading_channel;
      if(reading_channel>=CHANNEL_COUNT) reading_channel=0;
      adc_enable();
      adc_start_conv(reading_channel);
      timer_flags&=(~TMR_FASTCLK);
    }
    if(timer_flags&TMR_SLOWCLK){  //slow clock means flash any "spare" channels if an offer is present

      timer_flags&=(~TMR_SLOWCLK);
    }

    //if an ADC conversion is done
    if(adc_event){  //adc_event means that a reading is available
      state.dial_value[reading_channel] = adc_get_last_value();
      adc_disable();  //save power
      adc_event=0;
    }

    //if we have received data from the master
    if(twi_flags&TWI_RX_COMPLETE){
      //if we received only one byte then master is expecting a reply,
      //otherwise we expect to receive data
      if(get_rx_buffer_len()>1) input_mode=1;
      cli();  //don't interrupt by asking for more data while we are calculating
      switch(peek_rx_buffer(0)){ //first received byte should be the virtual register id
        case REG_DEVID:         //transmit device ID & channel count
          buffer[0]=DEVID_CONTROLLER | ((int8_t)CHANNEL_COUNT<<4);
          set_tx_buffer(buffer,1);
          break;
        case REG_REVISION:      //transmit revision number
          buffer[0]=0x1001;     //dummy placeholder
          set_tx_buffer(buffer,1);
          break;
        case REG_OFFER_FLAGS:   //receive flag showing if an offer is being made
          state.offer_state = peek_rx_buffer(1);
          if(state.offer_state&0x01){
            //update_panel_offer_in_progress();
          } else {
            //update_panel_clear_offer_in_progress();
          }
          break;
        case REG_OFFER_ACCEPT:  //return our offer state
          set_tx_buffer(&state.offer_reply_state, 1);
          break;
        case REG_C_ALL_VALUE:   //stream values of all controller dials
          if(input_mode){
            get_rx_buffer(state.dial_value, CHANNEL_COUNT*sizeof(int16_t));
          } else {
            set_tx_buffer(state.dial_value, CHANNEL_COUNT*sizeof(int16_t));
          }
          break;
        case REG_C1_VALUE:
          set_tx_buffer(&(state.dial_value[0]),2);
          break;
        case REG_C1_FLAGS:
          if(input_mode){
            state.controller_flags[0] = peek_rx_buffer(1);
          } else {
            set_tx_buffer((&state.controller_flags), 1);
          }
          break;
        default:
          //specific controller value requested
          if(peek_rx_buffer(0)>REG_C_ALL_VALUE && peek_rx_buffer(0)<=REG_C8_VALUE){
            int8_t idx = peek_rx_buffer(0) - REG_C1_VALUE;
            if(input_mode){
              state.dial_value[idx] = peek_rx_buffer(1) + peek_rx_buffer(2)<<8;
            } else {
              set_tx_buffer(&(state.dial_value[idx]), 2);
            }
          } else if(peek_rx_buffer(0)>=REG_C1_FLAGS && peek_rx_buffer(0)<=REG_C8_FLAGS){
            //specific controller flags requested
            int8_t idx = peek_rx_buffer(0) - REG_C1_FLAGS;
            if(input_mode){
              state.controller_flags[idx] = peek_rx_buffer(1);
            } else {
              set_tx_buffer(&(state.controller_flags[idx]), 1);
            }
          }
      }
      twi_flags&=(~TWI_RX_COMPLETE);
      sei();  //re-enable interrupts
    }
  }
}
