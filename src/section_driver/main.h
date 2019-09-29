#define CHANNEL_COUNT 4

struct section_state {
  int8_t section_flags[CHANNEL_COUNT];
  int8_t set_speed[CHANNEL_COUNT];
};

/* section flags values */
#define SF_OCCUPIED 1<<0
#define SF_BLOCKED  1<<1
#define SF_REVERSE  1<<2
