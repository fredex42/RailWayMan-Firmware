#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#include "tracksection-functions.h"

void error(char* desc)
{
  puts(desc);
  putchar('\n');
  exit(255);
}

int main(int argc, char *argv[])
{
  uint8_t unsigned_speed, direction;
  char errmsg[256];

  convert_signed_speed(128,&unsigned_speed,&direction);

  if(unsigned_speed!=128){
    snprintf(errmsg, 256, "Expected unsigned speed 128, got %d", unsigned_speed);
    error(errmsg);
  }

  if(direction!=0){
    snprintf(errmsg, 256, "Expected direction 0, got %d", direction);
    error(errmsg);
  }

  convert_signed_speed(-36,&unsigned_speed, &direction);
  if(unsigned_speed!=36){
    snprintf(errmsg, 256, "Expected unsigned speed 128, got %d", unsigned_speed);
    error(errmsg);
  }

  if(direction!=1){
    snprintf(errmsg, 256, "Expected direction 1, got %d", direction);
    error(errmsg);
  }

  exit(0);
}
