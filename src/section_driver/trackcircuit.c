#include <avr/io.h>
#include "trackcircuit.h"
#define _NOP() do { __asm__ __volatile__ ("nop"); } while(0);

void setup_ports()
{
  //configure the incoming trackcircuit ports.
  //trackcircuit inputs are negative-going inputs on the lower nyblle of PORTC
  //and require internal pull-ups.
  DDRC &= 0xF0;  //upper nybble set, lower nybble clear => bottom 4 pins are input (5 & 6 are used as I2C)
  PORTC |= 0x0F;  //set lower nybble of port to enable pull-up resistors on input pins. Data can be read from the PINC register.
  _NOP(); //one cycle to stabilise
}

/**
gets the section occupancy status from the onboard trackcircuits.
the result is an 8-bit value, the lower nybble is a byte-field representing
the occupancy with section 0=>LSB.
*/
int8_t get_section_occupancy()
{
  int8_t rv = PORTC;
  rv &= 0x0F;
  return rv;
}
