#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#define F_CPU 2000000UL
#include <util/delay.h>

#define I2CADDRESS 0x03
#include "twi.h"

char tx_buffer[TWI_BUFFER_SIZE];
int8_t tx_buffer_ptr;
int8_t tx_buffer_len;
char rx_buffer[TWI_BUFFER_SIZE];
int8_t rx_buffer_ptr;
int8_t rx_buffer_len;
int8_t twi_flags;

/**
set the transmit buffer data by copying in the provided buffer.
params:
 - (char *)data - pointer to buffer to copy
 - (int8_t)len  - length of buffer.
returns:
  - E_OK - copied in OK
  - -E_BUSY - a TX is already in progress
  - -E_RANGE - len is longer than the maxumim TX buffer size as set in twi.h
*/
int set_tx_buffer(char* data, int8_t len)
{
  if(twi_flags&TWI_TX_BUSY) return -E_BUSY;
  if(len>TWI_BUFFER_SIZE) return -E_RANGE;

  memcpy(tx_buffer, data, len);
  tx_buffer_len = len;
  twi_flags&= (~TWI_TX_ABORTED)&(~TWI_TX_COMPLETE); //new transfer ready, remove completed twi_flags
  return E_OK;
}

/**
push a single byte onto the TX buffer
*/
int push_tx_buffer(char data)
{
  if(twi_flags&TWI_TX_BUSY) return -E_BUSY;
  if(tx_buffer_len+1>TWI_BUFFER_SIZE) return -E_RANGE;
  tx_buffer[tx_buffer_len] = data;
  ++tx_buffer_len;
  return E_OK;
}

/**
read the receive buffer by copying its data to the provided pointer
*/
int get_rx_buffer(char *data, int8_t max_len)
{
  if(twi_flags&TWI_RX_BUSY) return -E_BUSY;
  if(!twi_flags&TWI_RX_COMPLETE) return -E_NOTREADY;
  if(max_len>rx_buffer_len) return -E_RANGE;

  memcpy(data, rx_buffer, rx_buffer_len);
  return rx_buffer_len;
}

int8_t get_rx_buffer_len()
{
  if(twi_flags&TWI_RX_BUSY) return -E_BUSY;
  if(!twi_flags&TWI_RX_COMPLETE) return -E_NOTREADY;
  return rx_buffer_len;
}

/**
read a single byte from the receive buffer
*/
char peek_rx_buffer(int8_t idx)
{
  if(twi_flags&TWI_RX_BUSY) return -E_BUSY;
  if(!twi_flags&TWI_RX_COMPLETE) return -E_NOTREADY;
  if(idx>rx_buffer_len) return -E_RANGE;
}
//see https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
//service the TWI (===I2C) interrupt
ISR(TWI_vect){
  switch(TWSR){
    //slave receiver codes
    case 0x60:  //own address has been received, wake-up
      #ifdef DEBUG
      PORTD = 0x01;
      #endif
      TWCR = TWCR | (1<<TWINT) | (1 << TWEA); //set TWEA bit to ACK incoming byte
      rx_buffer_ptr=0;
      twi_flags|=TWI_RX_BUSY;
      break;
    case 0x68:  //arbitration lost, own address received
      TWCR = TWCR | (1<<TWINT) | (1<<TWEN);
      #ifdef DEBUG
      PORTD=0x02;
      #endif
      break;
    case 0x70:  //general call address received
      #ifdef DEBUG
      PORTD=0x03;
      #endif
      break;
    case 0x78:  //arbitration lost, general call received
      #ifdef DEBUG
      PORTD=0x04;
      #endif
      break;
    case 0x80:  //own call address received, data ready, ACKED
      if(rx_buffer_ptr<rx_buffer_len && rx_buffer_ptr<TWI_BUFFER_SIZE){
        rx_buffer[rx_buffer_ptr] = TWDR;  //receive data register into the is_running flag
        TWCR = TWCR | (1<<TWINT) | (1<<TWEA); //set TWEA bit to ACK the incoming byte
      } else {
        TWCR = TWCR | (1<<TWINT) & ~(1<<TWEA); //clear TWEA bit to NACK the incoming byte, we ran out of space
      }
      break;
    case 0x88:  //own call address received, data ready, NACKED
      #ifdef DEBUG
      PORTD=0x05;
      #endif
      break;
    case 0x90:  //general call address received, data ready, ACKED
      #ifdef DEBUG
      PORTD=0x06;
      #endif
      break;
    case 0x98:  //general call address received, data ready, NACKED
      #ifdef DEBUG
      PORTD=0x07;
      #endif
      break;
    case 0xA0:  //STOP condition or repeated START condition received
      #ifdef DEBUG
      PORTD=0x08;
      #endif
      twi_flags&=~TWI_RX_BUSY;  //clear the busy flag.
      TWCR = TWCR | (1<<TWINT) | (1<< TWEA); //set the TWEA bit, clear STA and STO to switch to not-addressed-listening
      break;

    //slave transmitter codes
    case 0xA8:  //own address has been received, ACK returned, need data byte.
      #ifdef DEBUG
      PORTD=0x09;
      #endif
      twi_flags|=TWI_TX_BUSY;
      tx_buffer_ptr=0;
      TWDR = tx_buffer[0];
      TWCR = TWCR | (1 << TWINT) | (0 << TWEA);  //this is the last data byte so TWEA should be 0
      break;
    case 0xB0:  //arbitration lost, then addressed for read. Not needed since we only operate as slave
      #ifdef DEBUG
      PORTD=0x0A;
      #endif
      break;
    case 0xB8:  //data byte transmitted, ACK received, load next byte.
      #ifdef DEBUG
      PORTD=0x0B;
      #endif
      ++tx_buffer_ptr;
      if(tx_buffer_ptr<tx_buffer_len && tx_buffer_ptr<TWI_BUFFER_SIZE){
        TWDR = tx_buffer[tx_buffer_ptr];
        TWCR = TWCR | (1<<TWINT) | (1<<TWEA);
      } else {
        TWCR = TWCR | (1<<TWINT) & ~(1<<TWEA);
      }
      break;
    case 0xC0:  //data byte transmitted, NOT ACK received, stop
      #ifdef DEBUG
      PORTD=0x0C;
      #endif
      twi_flags&= ~TWI_TX_BUSY;
      twi_flags|= TWI_TX_ABORTED;
      TWCR = TWCR | (1 << TWINT) | (1 << TWEA);  //switch to not addressed slave mode, will recognise own address
      break;
    case 0xC8:  //last data byte transmitted, ACK received.
      #ifdef DEBUG
      PORTD=0x0D;
      #endif
      TWCR = TWCR | (1 << TWINT) | (1 << TWEA);  //switch to not addressed slave mode, will recognise own address
      break;

    default:
      #ifdef DEBUG
      PORTD=0xFF;
      #endif
      TWCR = TWCR | (1 << TWINT) | (1 << TWEA);
      break;
  }
}

void setup_twi(int8_t myaddress){
  //see p. 223 of datasheet
  //set up own address, and respond to global as well
  int8_t respondglobal = 0x00;

  TWAR = (myaddress << 1) | (respondglobal && 0x01);

  //set up Two-Wire Control Register
  TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
  _delay_ms(50);
}
