#define CHANNEL_COUNT 4

struct controller_state {
  int8_t offer_state;
  int8_t offer_reply_state;
  int16_t dial_value[CHANNEL_COUNT];
  int8_t controller_flags[CHANNEL_COUNT];
  int8_t ready_for_adc;
};

/* controller flags definitions */
#define CF_INACTIVE   0x0    //controller is not routed to anything
#define CF_ACTIVE     0x1    //controller is in control of something
#define CF_OVERRIDDEN 0x2    //controller is nominally in control but has been overriden by the system
