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
#include "main.h"
#include "virtualregisters.h"

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

void dump_i2c_buffer(char *buf, int maxlen){
  int c;
  for(c=0;c<maxlen;++c){
    fprintf(stderr, "%02x ", buf[c]);
  }
  fprintf(stderr, "\n");
}

//read everything unil 0xff into a buffer, that way we know we got the lot.
char *i2c_read_buffer(int fp, int8_t devid)
{
  char *buf=(char *)malloc(32*sizeof(char));
  memset(buf,0,32);
  char ch;
  int n=0,rv=0;

  ioctl(fp, I2C_SLAVE,devid);
  rv=read(fp,buf,32);
  //dump_i2c_buffer(buf,32);
  return buf;
}

/**
read the ID byte register from the firmware device
*/
int8_t read_id_byte(int fp, int8_t devid)
{
  char *buf;
  int8_t rtn;
  ioctl(fp, I2C_SLAVE, devid);
  buf=0x00;
  write(fp, &buf, 1); //we want register 0
  fprintf(stderr,"Wrote 0x%x\n", buf);
  usleep(100);
  //read(fp, &buf,2);  //read the contents and return
  buf=i2c_read_buffer(fp, devid);
  rtn=buf[0];
  free(buf);
  fprintf(stderr, "Read 0x%x\n", rtn);
  ioctl(fp, I2C_SLAVE, 0);
  return rtn;
}

/**
write new channel flags to the given channel
fp - file descriptor for I2C bus
devid - i2c devide ID to target
channel - (zero-based) index of the channel to target
flags - new flags to set
*/
void write_channel_flags(int fp, int8_t devid, uint8_t channel, int8_t flags)
{
  char buf[4];
  int8_t rtn;
  ioctl(fp, I2C_SLAVE, devid);
  buf[0]=REG_C1_FLAGS + channel;
  buf[1]=flags;
  buf[2]=0x00;
  if(buf[0]>REG_C8_FLAGS) return; //invalid for more than 8 channels
  write(fp, buf, 2);
  fprintf(stderr, "Wrote 0x%x 0x%x", buf[0],buf[1]);
}

int16_t read_firmware_revision(int fp, int8_t devid)
{
  int8_t sendbuf;
  int16_t rtn=0;
  char *buf;

  ioctl(fp, I2C_SLAVE, devid);
  sendbuf = 0x01; //we want register 1
  write(fp, &sendbuf, 1);
  fprintf(stderr, "Wrote 0x%x\n",sendbuf);
  usleep(100);
  //read(fp, &buf, 3);
  buf=i2c_read_buffer(fp, devid);
  memcpy(&rtn, buf,2);
  free(buf);

  fprintf(stderr, "Read 0x%x\n", rtn);
  ioctl(fp, I2C_SLAVE, 0);
  return rtn;
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

int main(int argc, char *argv[])
{
  int arg_at;

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

  uint8_t current_flags;
  while(1){
    switch(current_flags){
      case CF_INACTIVE:
        current_flags=CF_ACTIVE;
        break;
      case CF_ACTIVE:
        current_flags=CF_OVERRIDDEN;
        break;
      case CF_OVERRIDDEN:
        current_flags=CF_INACTIVE;
        break;
      default:
        current_flags=CF_INACTIVE;
        break;
    }
    for(int n=0;n<4;++n){
      write_channel_flags(fd, opts->deviceid, n, current_flags);
      sleep(1);
    }

  }
}
