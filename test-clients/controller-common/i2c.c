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
#include "registers.h"

#include "i2c.h"

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

int8_t read_id_byte(int fp, int8_t devid)
{
  char *buf;
  char tbuf;
  int8_t rtn;
  ioctl(fp, I2C_SLAVE, devid);

  tbuf=0x00;
  write(fp, &tbuf, 1); //we want register 0

  fprintf(stderr,"Wrote 0x%x\n", tbuf);
  usleep(100);
  //read(fp, &buf,2);  //read the contents and return
  buf=i2c_read_buffer(fp, devid);
  rtn=buf[0];
  free(buf);
  fprintf(stderr, "Read 0x%x\n", rtn);
  ioctl(fp, I2C_SLAVE, 0);
  return rtn;
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
  fprintf(stderr, "Wrote 0x%x 0x%x\n", buf[0],buf[1]);
}

/**
returns a static string containing a text description of the device for the given ID
*/
static char *device_desc_for(int8_t id)
{
  if(id<0 || id>4){
    return "Invalid device ID";
  }
  return device_desc[id];
}
