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
#include "../controller-common/i2c.c"

extern int errno;

#define BUS_NUM 1

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
