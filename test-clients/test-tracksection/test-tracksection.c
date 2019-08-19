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
#include <ncurses.h>

#include "../controller-common/i2c.h"
#include "test-tracksection.h"
#include "tracksection-functions.h"

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
      .name="channel",
      .has_arg=required_argument,
      .flag= NULL,
      .val='c'
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
      case 'c':
        printf("Got string %s for channel\n", optarg);
        rtn->channel=atoi(optarg);
        break;
      case 'b':
        printf("Got string %s for busnum\n", optarg);
        rtn->busnum=atoi(optarg);
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
  int16_t *raw_channel_values;

  static struct timeval timeout = {
    .tv_sec=0,
    .tv_usec=500
  };

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

  if(EXTRACT_DEVID(id_byte)!=DEVID_TRACKSECTION){
    fprintf(stderr, "This program only works with a track section device.\n");
    //exit(3);
  }

  int n;
  int16_t current_speed=0;
  int8_t occupancy=0;
  char ch;
  initscr();
  nodelay(stdscr, TRUE);

  printf("Press + to increase speed or - to decrease. Press 's' or '0' to stop.\n\n");

  while(1){
    occupancy = get_section_occupation(fd, opts->deviceid);

    printf("Occupancy byte: 0x%02x. Controlling channel %d: Current speed %d   \r", occupancy, opts->channel, current_speed);
      ch=getch();
      switch(ch){
        case '+':
        case '=': //on some keyboards + is SHIFT-= so treat it same without shift
          current_speed+=3; //roughly 1%
          //set_section(fd,opts->deviceid,opts->channel,current_speed);
          break;
        case '-':
          current_speed-=3;
          //set_section(fd,opts->deviceid,opts->channel,current_speed);
          break;
        case 's':
        case 'S':
        case '0':
          current_speed=0;
          //set_section(fd,opts->deviceid,opts->channel,current_speed);
          break;
        case 'q':
        case 'Q':
          nodelay(stdscr,FALSE);
          puts("\n\n");
          endwin();
          exit(1);
        default:
          break;
      }
    fflush(stdout);
    usleep(500);
  }
}
