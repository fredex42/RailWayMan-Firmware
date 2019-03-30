
enum led_display {
    LED_OFF=0,
    LED_RED,
    LED_GRN
};

#define CHANNEL_0_PORT PORTB
#define CHANNEL_0_DDR  DDRB
#define CHANNEL_0_GRN (1<<0)
#define CHANNEL_0_RED (1<<1)

#define CHANNEL_1_PORT PORTB
#define CHANNEL_1_DDR  DDRB
#define CHANNEL_1_GRN (1<<2)
#define CHANNEL_1_RED (1<<3)

#define CHANNEL_2_PORT PORTD
#define CHANNEL_2_DDR  DDRD
#define CHANNEL_2_GRN (1<<4)
#define CHANNEL_2_RED (1<<5)

#define CHANNEL_3_PORT PORTD
#define CHANNEL_3_DDR  DDRD
#define CHANNEL_3_GRN (1<<6)
#define CHANNEL_3_RED (1<<7)

/**
sets the LED output state for the given channel
*/
void set_indicator(uint8_t channel, uint8_t red, uint8_t grn);
