#define F_CPU 200000UL	//this seems to give approximately the right output on the hardware with internal clo

#include <avr/interrupt.h>  //for sei()
#include <avr/sleep.h>      //for set_sleep_mode
#include <util/delay.h>
#include <string.h>
#include "main.h"
#include "twi.h"
#include "pwm.h"
#include "address_finder.h"
#include "i2c_defs.h"
#include "registers.h"
#include "trackcircuit.h"
struct section_state state;

void setup_start_indicator()
{
  //startup indicator LED is on PB0 & PB1
  DDRB |= 0x03; //set data direction to OUT
  PORTB &= ~0x03; //clear the outputs
}

void show_start_phase1()
{
  PORTB |= 0x01;  //set PB0
}

void show_start_phase2()
{
  PORTB |= PORTB2;  //set PB1
}

void clear_start_indicator()
{
  PORTB &= ~0x03; //clear PB0 & PB1
}

int main(void)
{
  unsigned char input_mode=0;
  unsigned char buffer[8];
  int8_t temp;

  setup_start_indicator();
  show_start_phase1();
  setup_ports();

  _delay_ms(200);
  //load TWI address from DIP switches at PORTD
  int8_t twi_address = get_twi_address();

  memset(&state, 0, sizeof(struct section_state));

  //enable TWI
  setup_twi(twi_address);

  //setup PWM
  setup_pwm_0();
  //setup_pwm_2();

  //enable interrupts
  sei();

  while(1){
    show_start_phase2();
    input_mode=0;
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
    clear_start_indicator();

    if(twi_flags&TWI_RX_COMPLETE){
      //if we received only one byte then master is expecting a reply,
      //otherwise we expect to receive data
      if(get_rx_buffer_len()>1) input_mode=1;
      cli();  //don't interrupt by asking for more data while we are calculating
      switch(peek_rx_buffer(0)){ //first received byte should be the virtual register id
        case REG_DEVID:         //transmit device ID & channel count
          buffer[0]=DEVID_TRACKSECTION | ((int8_t)CHANNEL_COUNT<<4);
          set_tx_buffer(buffer,1);
          break;
        case REG_REVISION:      //transmit revision number
          buffer[0]=0x10;
          buffer[1]=0x01;     //dummy placeholder
          set_tx_buffer(buffer,2);
          break;
        case REG_POWER_S1:
          if(input_mode){
            state.set_speed[0] = peek_rx_buffer(1);
            set_pwm_0a(state.set_speed[0]);
          } else {
            set_tx_buffer(&state.set_speed[0],1);
          }
          break;
        case REG_FLAGS_S1:
          if(input_mode){
            state.section_flags[0] = peek_rx_buffer(1);
            if(state.section_flags[0]&SF_REVERSE){
              set_reversing_bit(0);
            } else {
              clear_reversing_bit(0);
            }
          } else {
            set_tx_buffer(&state.section_flags[0],1);
          }
          break;
        case REG_POWER_S2:
            if(input_mode){
              state.set_speed[1] = peek_rx_buffer(1);
              set_pwm_0b(state.set_speed[1]);
            } else {
              set_tx_buffer(&state.set_speed[1],1);
            }
            break;
        case REG_FLAGS_S2:
          if(input_mode){
            state.section_flags[1] = peek_rx_buffer(1);
            if(state.section_flags[1]&SF_REVERSE){
              set_reversing_bit(1);
            } else {
              clear_reversing_bit(1);
            }
          } else {
            set_tx_buffer(&state.section_flags[1],1);
          }
          break;
        case REG_POWER_S3:
          if(input_mode){
            state.set_speed[2] = peek_rx_buffer(1);
            set_pwm_2a(state.set_speed[2]);
          } else {
            set_tx_buffer(&state.set_speed[2],1);
          }
          break;
        case REG_FLAGS_S3:
          if(input_mode){
            state.section_flags[2] = peek_rx_buffer(1);
            if(state.section_flags[2]&SF_REVERSE){
              set_reversing_bit(2);
            } else {
              clear_reversing_bit(2);
            }
          } else {
            set_tx_buffer(&state.section_flags[2],1);
          }
          break;
        case REG_POWER_S4:
          if(input_mode){
            state.set_speed[3] = peek_rx_buffer(1);
            set_pwm_2b(state.set_speed[3]);
          } else {
            set_tx_buffer(&state.set_speed[3],1);
          }
          break;
        case REG_FLAGS_S4:
          if(input_mode){
            state.section_flags[3] = peek_rx_buffer(1);
            if(state.section_flags[3]&SF_REVERSE){
              set_reversing_bit(3);
            } else {
              clear_reversing_bit(3);
            }
          } else {
            set_tx_buffer(&state.section_flags[3],1);
          }
          break;
        case REG_OCCUPATION:
          if(input_mode){
            //does not support input
            temp = get_section_occupancy();
            set_tx_buffer(&temp, 1);
          } else {
            temp = get_section_occupancy();
            set_tx_buffer(&temp, 1);
          }
      }
      clear_rx_buffer();
      sei();  //re-enable interrupts
    }
  }
}
