#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

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

  decode_controller_value(0x200, &sign_flag, &value);
  printf("0x200 sign was %d, value was %d\n", sign_flag, value);

  decode_controller_value(0x020, &sign_flag, &value);
  printf("0x020 sign was %d, value was %d\n", sign_flag, value);

  if(!sign_flag) error("Sign incorrect for 0x3ff");
  if(value!=0xFF) error("Value incorrect for 0x3ff");

  puts("Tests passed\n");
  exit(0);
}
