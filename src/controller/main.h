#define CHANNEL_COUNT 4

struct controller_state {
  int8_t offer_state;
  int8_t offer_reply_state;
  int16_t dial_value[CHANNEL_COUNT];
  int8_t controller_flags[CHANNEL_COUNT];
};
