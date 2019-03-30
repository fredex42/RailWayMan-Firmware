#define EXTRACT_DEVID(id_byte) id_byte&0x0F
#define EXTRACT_CHANNEL_COUNT(id_byte) (id_byte>>4)&0x0F

//from firmware/src/common/i2c_defs.h
enum DeviceID {
  DEVID_INVALID=0,      //NOT a valid device ID (000)
  DEVID_TRACKSECTION,   //a track section control device (001)
  DEVID_CONTROLLER,     //a control surface device (010)
  DEVID_PMOTOR,         //a point/semaphore signal control device (011)

};

static char *device_desc[] = {
  "Not valid",
  "Track section driver",
  "Control surface",
  "Point motor driver",
  "\0"
};

struct program_opts {
  int8_t deviceid;
  int8_t busnum;
};
