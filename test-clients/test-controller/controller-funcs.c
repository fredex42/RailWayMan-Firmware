#include <stdint.h>
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

  *sign_flag = (raw_value & signmask) >> 10;
  int16_t intermediate = raw_value - 512;

  *output_value = (uint8_t) (intermediate & 0xFF);
}
