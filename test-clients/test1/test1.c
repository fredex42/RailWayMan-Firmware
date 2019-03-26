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

int main(int argc, char *argv[])
{
  int fd,n;
  char rdbuf[16];
  char wrbuf[16];
  memset(rdbuf,0,16);

  fd=open_i2c();
  if(fd<0) exit(1);

  ioctl(fd,I2C_SLAVE,0x04);

  read(fd, rdbuf, 8);
  for(n=0;n<8;++n){
    printf("0x%02x ", rdbuf[n]);
  }
  putchar('\n');

  sleep(1);

  //ioctl(fd,I2C_SLAVE,0x04);
  snprintf(wrbuf,8,"Hello");
  write(fd, wrbuf, strlen(wrbuf));
  sleep(1);

  //ioctl(fd,I2C_SLAVE,0x04);
  read(fd, rdbuf, 8);
  for(n=0;n<8;++n){
    printf("0x%02x ", rdbuf[n]);
  }
  putchar('\n');

  close(fd);
}
