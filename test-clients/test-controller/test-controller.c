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
#include "test_controller.h"

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
  // do {
  //   rv=read(fp,&ch,1);
  //   if(rv==0xff){
  //     buf[n]=0;
  //     dump_i2c_buffer(buf, n);
  //     return buf;
  //   } else {
  //     buf[n]=ch;
  //   }
  //   ++n;
  // } while(n<32 && rv!=-1);
  // buf[32]=0;
  // dump_i2c_buffer(buf, 32);
  // return buf;
}

//next thing to try - read everything unil 0xff into a buffer, that way we know we got the lot.
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
    for(n=0;n<channel_count;++n)
      fprintf(stdout, "Channel %d: 0x%x\t", n, raw_channel_values[n]);
    fprintf(stdout,"\r");
    usleep(50000);
  }
}
