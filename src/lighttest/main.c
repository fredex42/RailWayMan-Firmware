#include <avr/io.h>
#define F_CPU 1000000L
#include <util/delay.h>
int main(void)
{
  DDRB=0xFF;
  DDRD=0xFF;

  while(1){
    PORTB=0x05;
    PORTD=(1<<5)|(1<<6);
    _delay_ms(1000);
    PORTB=0x09;
    PORTD=(1<<4)|(1<<7);
    _delay_ms(1000);
    PORTB=0x00;
    PORTD=0x00;
    _delay_ms(1000);
  }
}
