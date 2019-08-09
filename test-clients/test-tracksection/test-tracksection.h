struct program_opts {
  int8_t deviceid;
  int8_t busnum;
  int8_t channel;
};

/* section flags values */
#define SF_OCCUPIED 1>>0
#define SF_BLOCKED  1>>1
#define SF_REVERSE  1>>2
