//extern globals
//use this to read the last data byte received
extern int8_t twi_data_byte;

//public functions from twi.c
void setup_twi(int8_t myaddress);

#define TWI_BUFFER_SIZE 8

//flags
#define TWI_NONE        0x00
#define TWI_RX_COMPLETE 0x01
#define TWI_RX_ABORTED  0x02
#define TWI_RX_BUSY     0x04

#define TWI_TX_COMPLETE 0x08
#define TWI_TX_ABORTED  0x0F
#define TWI_TX_BUSY     0xF0

//return codes
#define  E_OK 0
#define  E_BUSY 1 //busy, or buffer occupied
