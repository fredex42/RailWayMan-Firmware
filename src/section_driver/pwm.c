#include <avr/io.h>

/**
initialise PWM channel 2
*/
void setup_pwm_2(){
  //set data direction to OUT for port b pins 1,2 (OCR1A/OCR1B)
  DDRB = (1<<DDB3);
  DDRD = (1<<DDD3);

  OCR2A = 0xAA; //channel A half-power
  OCR2B = 0x01; //channel B 99% power

  TCNT2 = 0;
  TIMSK2 = 0;

  //clock select internal (128kHz) with 1024 divider => 125Hz or 8ms total
  //clock isn't 128! trying a /8 divider instead
  TCCR2B = 0x2;
  //set control registers. Inverting mode fast PWM on channels A and B
  TCCR2A = 0xF1;
}

void set_pwm_2a(uint8_t value){
  OCR2A = value;
}

void set_pwm_2b(uint8_t value){
  OCR2B = value;
}

/**
initialise PWM channel 0
*/
void setup_pwm_0(){
  //set data direction to OUT for port b pins 1,2 (OCR0A/OCR0B);
  DDRD = DDRD | (1<<DDD6) | (1<<DDD5);

  OCR0A = 0xAA; //channel A half-power
  OCR0B = 0x01; //channel B 99% power

  TCNT0 = 0;
  TIMSK0 = 0;

  //clock select internal (128kHz) with 1024 divider => 125Hz or 8ms total
  //clock isn't 128! trying a /8 divider instead
  TCCR0B = 0x3;
  //set control registers. Inverting mode fast PWM on channels A and B
  TCCR0A = 0xF1;
}

/**
disable clock source for pwm0, thereby turning it off withut removing other settings
*/
void disable_pwm_0()
{
  //TCCR0B = TCCR0B & (~0x04);
  TCCR0B = 0;
}

void enable_pwm_0()
{
  TCCR0B = 0x3;
}

void set_pwm_0a(uint8_t value){
  OCR0A = value;
}

void set_pwm_0b(uint8_t value){
  OCR0B = value;
}
