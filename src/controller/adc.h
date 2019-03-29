//error codes
#define ERR_OK      0
#define ERR_RANGE   1 //an argument was out of range
#define ERR_BUSY    2 //a conversion is already in progress

//internal status flags
#define ST_NONE     0
#define ST_BUSY     (1<<0)
#define ST_DISABLE  (1<<1)
#define ST_READY    (1<<2)

extern int8_t adc_event;

//function prototypes
int16_t adc_get_last_value();
uint8_t adc_start_conv(uint8_t channel);

void adc_disable();
void adc_enable();
void adc_setup();
