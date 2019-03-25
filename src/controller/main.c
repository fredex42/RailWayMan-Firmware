#include <avr/interrupt.h>  //for sei()
#include <avr/sleep.h>      //for set_sleep_mode
#include <string.h>
#include "twi.h"
#include "address_finder.h"
#include "i2c_defs.h"
#include "registers.h"

#define CHANNEL_COUNT 4

int main(void)
{
  unsigned char input_mode=0;
  unsigned char buffer[8];
  //load TWI address from DIP switches at PORTD
  int8_t twi_address = get_twi_address();
  //enable TWI
  setup_twi(my_address);
  //enable interrupts
  sei();

  while(1){
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
    if(twi_flags&TWI_RX_COMPLETE){
      //if we received only one byte then master is expecting a reply,
      //otherwise we expect to receive data
      if(get_rx_buffer_len()>1) input_mode=1;
      switch(peek_rx_buffer(0)){ //first received byte should be the virtual register id
        case REG_DEVID:
          buffer[0]=DEVID_CONTROLLER | ((int8_t)CHANNEL_COUNT<<4);
          set_tx_buffer(buffer,1);
          break;
        case REG_REVISION:
          buffer[0]=0x1001;
          set_tx_buffer(buffer,1);
          break;
          
      }
    }
  }
}
