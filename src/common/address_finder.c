#include <avr/io.h>
#include <util/delay.h>

/**
read the DIP switch configured address for the device from the Lower
nybble of PORTD. Configure PORTD appropriately.
*/
int8_t get_twi_address()
{
  DDRD&=0xF0;   //lower nyblle of DDR low => lower 4 pins are inputs
  PORTD|=0x0F;  //lower nybble of PORT high => enable pull-ups

  _delay_ms(10);  //wait for port to stabilise

  //Switch values are negative-true due to pullup, so we invert.
  //We are only reading the lower nyblle, so blank out the upper.
  //Finally, the pi only accepts i2c addresses>=0x03 so we shift left by one; that
  //way switch zero corresponds to address 2 and switch one corresponds to address 3.
  return (~PIND & 0x0F)<<1;
}
