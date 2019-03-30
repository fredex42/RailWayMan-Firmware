#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

extern int errno;

#define BUS_NUM 1

int open_i2c()
{
  char filename[12];
  int rv;

  snprintf(filename, 12, "/dev/i2c-%d", BUS_NUM);
  rv=open(filename, O_RDWR);
  if(rv<1){
    fprintf(stderr, "Could not open %s: error %d\n", filename,errno);
  }
  return rv;
}

const struct option* setup_options()
{
  static const struct option *option_list[] = {
    {
      "name": "deviceid",
      "flag": NULL,
      "val": 1
    },
    {
      "name": "\0",
      "flag": NULL,
      "val": 0
    }
  };
  return option_list;
}

int8_t read_id_byte(int fp, int8_t devid)
{
  int8_t buf;
  ioctl(fp, I2C_SLAVE, devid);
  buf=0x00;
  write(fp, &buf, 1); //we want register 0
  usleep(100);
  read(fp, &buf,1 );  //read the contents and return
  return buf;
}

void get_channel_values(int fp, int8_t devid, int16_t **raw_channel_values, int8_t channel_count)
{
  int8_t buf=0x04;
  ioctl(fp, I2C_SLAVE, devid);
  write(fp, &buf, 1); //register 4 is "all controller values"
  usleep(100);
  read(fp, *raw_channel_values, channel_count);
}

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

static char *device_desc_for(int8_t id){
  if(id<0 || id>4){

  }
}
int main(int argc, char *argv[])
{
  int arg_at;
  const struct option* opts = setup_options();
  int16_t *raw_channel_values;

  int8_t deviceid = getopt_long(argc,argv, "deviceid", opts, &arg_at);

  if(deviceid<3 || deviceid>254){
    fprintf(stderr, "Minimum allowed device ID on the pi is 3, max is 254.\n");
    exit(2);
  }

  int fd = open_i2c();
  if(fd<=0){
    exit(1);
  }

  int8_t id_byte = read_id_byte(fd, deviceid);
  int8_t channel_count = EXTRACT_CHANNEL_COUNT(id_byte);
  fprintf(stdout, "Got device with ID 0x%x (%s) and channel count 0x%x\n\n", EXTRACT_DEVID(id_byte), device_desc_for(EXTRACT_DEVID(id_byte)), EXTRACT_CHANNEL_COUNT(id_byte));

  if(EXTRACT_DEVID(id_byte)!=DEVID_CONTROLLER){
    fprintf(stderr, "This program only works with a control surface device.\n");
    exit(3);
  }

  raw_channel_values = (int16_t *)malloc((EXTRACT_CHANNEL_COUNT(id_byte)+1)*sizeof(int16_t));
  if(!raw_channel_values){
    fprintf(stderr, "Could not alloc memory for channels");
    exit(4);
  }
  memset(raw_channel_values, 0, (channel_count+1)*sizeof(int16_t));

  int n;
  while(1){
    get_channel_values(fd, deviceid, &raw_channel_values, channel_count);
    for(n=0;n<channel_count;++n)
      fprintf(stdout, "Channel %d: 0x%x", n, raw_channel_values[n]);
    fprintf(stdout,"\r");
    usleep(5000);
  }
}
