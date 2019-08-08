
//public functions from twi.c
void setup_twi(int8_t myaddress);

/**
read a single byte from the receive buffer
*/
char peek_rx_buffer(uint8_t idx);
/**
read the receive buffer by copying its data to the provided pointer
*/
int get_rx_buffer(char *data, int8_t max_len);
/**
push a single byte onto the TX buffer
*/
int push_tx_buffer(char data);
/**
set the transmit buffer data by copying in the provided buffer.
*/
int set_tx_buffer(char* data, int8_t len);

int8_t get_rx_buffer_len();

/*
clear any outstanding RX buffer content ready for next transfer
*/
void clear_rx_buffer();

extern int8_t twi_flags;

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
#define  E_OK           0
#define  E_BUSY         1 //busy, or buffer occupied
#define  E_RANGE        2 //trying to put or get more data than the buffer can take
#define  E_NOTREADY     3 //trying to read before transfer has been made
