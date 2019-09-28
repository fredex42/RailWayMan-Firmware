#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "tracksection-functions.h"
#include "test-tracksection.h"
#include "registers.h"
#include "../controller-common/i2c.h"

void set_section(int fp, int devid, int8_t channel, int16_t signed_speed)
{
  uint8_t unsigned_speed, direction_flag=0;
  int8_t section_flags=0;

  convert_signed_speed(signed_speed, &unsigned_speed, &direction_flag);
  set_section_speed(fp, devid, channel, unsigned_speed);

  section_flags = direction_flag<<SF_REVERSE;
  set_section_flags(fp, devid, channel, section_flags);
  ioctl(fp, I2C_SLAVE, 0);
}

/**
converts a signed 16-bit integer into an unsigned 8-bit integer and a direction flag.
if value is >255 or <-255 it is truncated.
direction flag 0 => forwards 1 => reverse
pass pointers to unsigned 8-bit values to receive the data.
*/
void convert_signed_speed(int16_t signed_speed, uint8_t *unsigned_speed, uint8_t *direction_flag)
{
  if(signed_speed>0){
    *unsigned_speed = (uint8_t)signed_speed;
    *direction_flag = 0;
  } else if(signed_speed<0){
    *unsigned_speed = (uint8_t)(-signed_speed);
    *direction_flag = 1;
  } else if(signed_speed==0){
    *unsigned_speed=0;
    *direction_flag=0;
  }
}

void set_section_speed(int fp, int devid, int8_t channel, int8_t speed)
{
  int8_t reg=0;
  char buf[8];

  switch(channel){
    case 0:
      reg = REG_POWER_S1;
      break;
    case 1:
      reg = REG_POWER_S2;
      break;
    case 2:
      reg = REG_POWER_S3;
      break;
    case 3:
      reg = REG_POWER_S4;
      break;
    default:
      return;
  }

  memset(buf,0,8);
  buf[0]=reg;
  buf[1]=speed;

  ioctl(fp, I2C_SLAVE, devid);
  write(fp, buf, 2);
  usleep(100);
}

void set_section_flags(int fp, int devid, int8_t channel, int8_t flags)
{
  int8_t reg=0;
  char buf[8];

  switch(channel){
    case 0:
      reg = REG_FLAGS_S1;
      break;
    case 1:
      reg = REG_FLAGS_S2;
      break;
    case 2:
      reg = REG_FLAGS_S3;
      break;
    case 3:
      reg = REG_FLAGS_S4;
      break;
    default:
      return;
  }

  memset(buf,0,8);
  buf[0]=reg;
  buf[1]=flags;

  ioctl(fp, I2C_SLAVE, devid);
  write(fp, buf, 2);
  usleep(100);
}

/**
requests section occupation byte from the hardware
*/
int8_t get_section_occupation(int fp, int devid)
{
  int8_t reg=REG_OCCUPATION;
  int8_t r=-1;
  ioctl(fp, I2C_SLAVE, devid);
  write(fp, &reg, 1);

  char *data = i2c_read_buffer(fp, devid);
  r=data[0];
  free(data);
  return r;
  ioctl(fp, I2C_SLAVE, 0);
}
