#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include "controller-funcs.h"
#include "test_controller.h"
#include "../controller-common/i2c.h"

extern int errno;

static const struct option* setup_options()
{
  static const struct option option_list[] = {
    {
      .name="deviceid",
      .has_arg=required_argument,
      .flag= NULL,
      .val='d'
    },
    {
      .name="busnum",
      .has_arg=required_argument,
      .flag= NULL,
      .val='b'
    },
    {
      .name=NULL,
      .has_arg=0,
      .flag=NULL,
      .val=0
    }
  };
  return &option_list;
}


void get_channel_values(int fp, int8_t devid, int16_t **raw_channel_values, int8_t channel_count)
{
  int8_t sendbuf=0x04;
  char *buf;
  ioctl(fp, I2C_SLAVE, devid);
  write(fp, &sendbuf, 1); //register 4 is "all controller values"
  usleep(100);
  buf = i2c_read_buffer(fp, devid);
  memcpy(*raw_channel_values, buf, channel_count*sizeof(int16_t));
  free(buf);
  //read(fp, *raw_channel_values, channel_count);
}

static char *device_desc_for(int8_t id){
  if(id<0 || id>4){
    return "Invalid device ID";
  }
  return device_desc[id];
}

const struct program_opts *get_program_opts(int argc, char *argv[])
{
  const struct option* opts = setup_options();
  struct program_opts* rtn = (struct program_opts *)malloc(sizeof(struct program_opts));
  memset(rtn, 0, sizeof(struct program_opts));

  char ch;
  while((ch=getopt_long(argc, argv, "db", opts, NULL))!=-1){
    switch(ch){
      case 'd':
        printf("Got string %s for deviceid\n", optarg);
        rtn->deviceid=atoi(optarg);
        break;
      default:
        printf("unrecognised arg: 0x%x\n", ch);
        break;
    }
    if(ch==0xff) break;
  }

  return (const struct program_opts *)rtn;
}

const char* direction_string(int8_t sign_flag, uint8_t decoded_speed_value)
{
  if(is_stop(decoded_speed_value)) return "STOP     ";
  return sign_flag ? "BACKWARDS" : "FORWARDS ";
}

int main(int argc, char *argv[])
{
  int arg_at;

  int16_t *raw_channel_values;

  const struct program_opts* opts = get_program_opts(argc, argv);

  if(opts->deviceid<3 || opts->deviceid>254){
    fprintf(stderr, "Minimum allowed device ID on the pi is 3, max is 254. You specified %d\n",opts->deviceid);
    exit(2);
  }

  int fd = open_i2c();
  if(fd<=0){
    exit(1);
  }

  int16_t fwrev = read_firmware_revision(fd, opts->deviceid);
  int8_t id_byte = read_id_byte(fd, opts->deviceid);

  int8_t channel_count = EXTRACT_CHANNEL_COUNT(id_byte);
  fprintf(stdout, "Got device with ID 0x%x (%s) and channel count 0x%x with firmware revision 0x%x (%d)\n\n", EXTRACT_DEVID(id_byte), device_desc_for(EXTRACT_DEVID(id_byte)), EXTRACT_CHANNEL_COUNT(id_byte), fwrev, fwrev);

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
    get_channel_values(fd, opts->deviceid, &raw_channel_values, channel_count);
    // for(n=0;n<channel_count;++n)
    //   fprintf(stdout, "Channel %d: 0x%04x\t", n, raw_channel_values[n]);
    fprintf(stdout,"\r");
    for(n=0;n<channel_count;++n){
      uint8_t sign_flag, value;
      decode_controller_value(raw_channel_values[n], &sign_flag, &value);
      const char* dir_string = direction_string(sign_flag, value);
      fprintf(stdout, "Channel %d: 0x%04x => 0x%02x %s\t", n, raw_channel_values, value, dir_string);
    }
    fprintf(stdout,"\r");
    fflush(stdout);
    usleep(5000);
  }
}
