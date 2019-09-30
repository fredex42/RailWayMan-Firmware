#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#include "controller-funcs.h"

void error(char* desc)
{
  puts(desc);
  putchar('\n');
  exit(255);
}

int main(int argc,char *argv)
{
  uint8_t sign_flag=0, value=0;

  decode_controller_value(0x3ff, &sign_flag, &value);
  printf("0x3ff sign was %d, value was %d\n", sign_flag, value);

  if(!sign_flag) error("Sign incorrect for 0x3ff");
  if(value!=0xFF) error("Value incorrect for 0x3ff");

  decode_controller_value(0x200, &sign_flag, &value);
  printf("0x200 sign was %d, value was %d\n", sign_flag, value);

  if(!sign_flag) error("Sign incorrect for 0x200");
  if(value!=0x00) error("Value incorrect for 0x200");

  decode_controller_value(0x020, &sign_flag, &value);
  printf("0x020 sign was %d, value was %d\n", sign_flag, value);

  if(sign_flag) error("Sign incorrect for 0x020");
  if(value!=240) error("Value incorrect for 0x020");

  decode_controller_value(0x040, &sign_flag, &value);
  printf("0x040 sign was %d, value was %d\n", sign_flag, value);

  if(sign_flag) error("Sign incorrect for 0x040");
  if(value!=224) error("Value incorrect for 0x040");

  decode_controller_value(0x060, &sign_flag, &value);
  printf("0x060 sign was %d, value was %d\n", sign_flag, value);

  if(sign_flag) error("Sign incorrect for 0x060");
  if(value!=208) error("Value incorrect for 0x060");

  decode_controller_value(0x220, &sign_flag, &value);
  printf("0x220 sign was %d, value was %d\n", sign_flag, value);

  if(!sign_flag) error("Sign incorrect for 0x220");
  if(value!=16) error("Value incorrect for 0x220");

  decode_controller_value(0x260, &sign_flag, &value);
  printf("0x260 sign was %d, value was %d\n", sign_flag, value);

  if(!sign_flag) error("Sign incorrect for 0x260");
  if(value!=48) error("Value incorrect for 0x260");

  decode_controller_value(0x2a0, &sign_flag, &value);
  printf("0x2a0 sign was %d, value was %d\n", sign_flag, value);

  if(!sign_flag) error("Sign incorrect for 0x2a0");
  if(value!=80) error("Value incorrect for 0x2a0");

  decode_controller_value(0x301, &sign_flag, &value);
  printf("0x301 sign was %d, value was %d\n", sign_flag, value);

  decode_controller_value(0x2ff, &sign_flag, &value);
  printf("0x2ff sign was %d, value was %d\n", sign_flag, value);

  decode_controller_value(0x300, &sign_flag, &value);
  printf("0x300 sign was %d, value was %d\n", sign_flag, value);

  if(!sign_flag) error("Sign incorrect for 0x300");
  if(value!=0x80) error("Value incorrect for 0x300");

  decode_controller_value(0x00, &sign_flag, &value);
  printf("0x00 sign was %d, value was %d\n", sign_flag, value);
  if(sign_flag) error("Sign incorrect for 0x00");
  if(value!=0xFF) error("Value incorrect for 0x00");

  puts("Tests passed\n");
  exit(0);
}
