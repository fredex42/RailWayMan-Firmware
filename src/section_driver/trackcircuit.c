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

  //configure the outgoing reversing switch ports.
  //reversing switch outs are a bit jumbled. S1 is on PB6, S2 is on PB7. S3 is on PD7 and S4 is on PB0.
  DDRB |= 1<<DDB7 | 1<<DDB6;
  DDRD |= 1<<DDD7;
  DDRB |= 1<<DDB0;

  //clear reversing bits on startup
  PORTB &= ~(1<<PORTB7 | 1<<PORTB6);
  PORTD &= ~(1<<PORTD7);
  //PORTB &= ~(1<<PORTB0);

  _NOP(); //one cycle to stabilise
}

/**
gets the section occupancy status from the onboard trackcircuits.
the result is an 8-bit value, the lower nybble is a byte-field representing
the occupancy with section 0=>LSB.
*/
int8_t get_section_occupancy()
{
  int8_t rv = PINC;
  rv &= 0x0F;
  return rv;
}

void set_reversing_bit(int8_t channel)
{
  switch(channel){
    case 0:
      PORTB |= 1<<PORTB6;
      break;
    case 1:
      PORTB |= 1<<PORTB7;
      break;
    case 2:
      PORTD |= 1<<PORTD7;
      break;
    case 3:
      PORTB |= 1<<PORTB0;
      break;
    default:
      break;
  }
}

void clear_reversing_bit(int8_t channel)
{
  switch(channel){
    case 0:
      PORTB &= ~(1<<PORTB6);
      break;
    case 1:
      PORTB &= ~(1<<PORTB7);
      break;
    case 2:
      PORTD &= ~(1<<PORTD7);
      break;
    case 3:
      PORTB &= ~(1<<PORTB0);
      break;
    default:
      break;
  }
}
