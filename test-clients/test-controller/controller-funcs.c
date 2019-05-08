#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include "test_controller.h"
/**
the contorller sends us an unsigned 10-bit value representing the location of the dial.
The upper half is negative and the lower half is positive, with the mid-point being zero.
We therefore have 1 sign bit and 9 data bits.
By subtracting 512 we get a signed 9-bit value with zero at the centre, then
 truncate to 8 bits to get our final value
*/
void decode_controller_value(uint16_t raw_value, uint8_t *sign_flag, uint8_t *output_value)
{
  uint16_t signmask = 0x200;  //bit 10 set
  uint16_t valuemask = 0x1FF; //bits 1-9 set

  *sign_flag = (raw_value & signmask) >> 9;
  int16_t intermediate = raw_value - 512;
  //printf("intermediate value is %d (0x%03x)\n", intermediate, intermediate);
  //printf("abs intermediate value is %d (0x%03x)\n", abs(intermediate>>1), abs(intermediate>>1));

  if(intermediate==-512){ //we overflow by 1 bit at the bottom, deal with this
    *output_value = 0xFF; //this would otherwise be 0x100, which then gets truncated to 0x00.
  } else {
    *output_value = (uint8_t) (abs(intermediate>>1) & 0xFF);
  }
}

/**
returns true if the decoded value should be interpreted as a STOP
*/
uint8_t is_stop(uint8_t decoded_speed_value)
{
  if(decoded_speed_value<10) return 1;
  return 0;
}
