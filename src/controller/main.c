#include <avr/interrupt.h>  //for sei()
#include <avr/sleep.h>      //for set_sleep_mode
#include <util/delay.h>
#include <string.h>
#include "main.h"
#include "twi.h"
#include "adc.h"
#include "timeout.h"
#include "address_finder.h"
#include "i2c_defs.h"
#include "registers.h"
#include "led_output.h"

#define F_CPU 200000UL	//this seems to give approximately the right output on the hardware with internal clo
struct controller_state state;
int8_t current_reading_index=0;

int main(void)
{
  unsigned char input_mode=0;
  unsigned char buffer[8];
  int16_t temp;

  setup_ports();

  //show all red
  set_indicator(0,1,0);
  set_indicator(1,1,0);
  set_indicator(2,1,0);
  set_indicator(3,1,0);

  _delay_ms(200);
  //load TWI address from DIP switches at PORTD
  int8_t twi_address = get_twi_address();

  memset(&state, 0, sizeof(struct controller_state));

  //enable TWI
  setup_twi(twi_address);

  //enable ADC
  adc_setup();
  //enable timeout
  setup_timeout();
  //setup I/O
  setup_ports();
  //enable interrupts
  sei();

  //init completed - show all green
  _delay_ms(200);
  set_indicator(0,0,1);
  set_indicator(1,0,1);
  set_indicator(2,0,1);
  set_indicator(3,0,1);

  state.ready_for_adc = 1;
  adc_enable();
  adc_start_conv(current_reading_index);  //kick off ADC conversion here

  //clear indicators and enter normal operation
  _delay_ms(200);
  set_indicator(0,0,0);
  set_indicator(1,0,0);
  set_indicator(2,0,0);
  set_indicator(3,0,0);

  while(1){
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();

    if(adc_event){  //a conversion finished
      //FIXME: handle ERR_BUSY response
      temp = adc_get_last_value();
      // if(temp==ERR_BUSY){
      //   //not sure what to do here
      // } else {
        state.dial_value[current_reading_index] = temp;
        ++current_reading_index;
        if(current_reading_index>CHANNEL_COUNT-1) current_reading_index=0;
        adc_start_conv(current_reading_index);
        //state.ready_for_adc=1;
//      }
      //state.dial_value[current_reading_index] = current_reading_index;

      adc_event=0;
    }

    // if(timer_flags&TMR_SLOWCLK){
    //   if(state.ready_for_adc){
    //     adc_start_conv(current_reading_index);
    //     state.ready_for_adc=0;
    //   }
    //   timer_flags&=~(TMR_SLOWCLK);
    // }

    input_mode=0;
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
          buffer[0]=0x10;
          buffer[1]=0x01;     //dummy placeholder
          set_tx_buffer(buffer,2);
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
              set_from_channel_flag(idx, state.controller_flags[idx]);  //update LED state
            } else {
              set_tx_buffer(&(state.controller_flags[idx]), 1);
            }
          }
      }
      clear_rx_buffer();
      sei();  //re-enable interrupts
    }
  }
}
