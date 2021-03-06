#include <avr/interrupt.h>  //for sei()
#include <avr/sleep.h>      //for set_sleep_mode
#include "twi.h"

int main(void)
{
  static char initial_buffer[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x00};
  char buffer[8];
  int8_t received_bytes;
  int rv;

  setup_twi(0x04);
  sei();

  set_tx_buffer(initial_buffer, 7);
  while(1){
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();

    if(twi_flags&TWI_RX_COMPLETE){
      get_rx_buffer(buffer,8);

      set_tx_buffer(buffer,get_rx_buffer_len());
      clear_rx_buffer();
    }
  }

  cli();
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_mode();
}
