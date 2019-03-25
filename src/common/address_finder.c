#include <avr/io.h>

/**
read the DIP switch configured address for the device from the Lower
nybble of PORTD. Configure PORTD appropriately.
*/
int8_t get_twi_address()
{
  DDRD&=0xF0;   //lower nyblle of DDR low => lower 4 pins are inputs
  PORTD|=0x0F;  //lower nybble of PORT high => enable pull-ups
  __no_operation(); //pause for synchronisation

  return PINB & 0x0F; //blank out upper nybble and return lower
}
