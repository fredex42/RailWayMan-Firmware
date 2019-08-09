#include <sys/types.h>

/* public function prototypes */
int open_i2c();
void dump_i2c_buffer(char *buf, int maxlen);
char *i2c_read_buffer(int fp, int8_t devid);
int8_t read_id_byte(int fp, int8_t devid);
int16_t read_firmware_revision(int fp, int8_t devid);
void write_channel_flags(int fp, int8_t devid, uint8_t channel, int8_t flags);
static char *device_desc_for(int8_t id);

/* useful macros for interpreting values from firmware */
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
