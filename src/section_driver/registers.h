/**
I2C virtual registers, in addition to those in common/i2c_defs.h
*/

/**
virtual "register" identities for a section driver submodule.
*/
enum VirtualRegisterController {
  REG_POWER_S1=2,   //8 bits wide, RW. Current value of the power output for section 1. Anything below 0x10 is "stop".
  REG_FLAGS_S1,    //8 bits wide, RW.  Flags for section 1 - see section_flags.h for definitions
  REG_POWER_S2,     //8 bits wide, RW. Current value of the power output for section 2. Anything below 0x10 is "stop".
  REG_FLAGS_S2,    //8 bits wide, RW.  Flags for section 2 - see section_flags.h for definitions
  REG_POWER_S3,     //Current value of the power output for section 3. Anything below 0x10 is "stop".
  REG_FLAGS_S3,    //8 bits wide, RW.  Flags for section 3 - see section_flags.h for definitions
  REG_POWER_S4,     //Current value of the power output for section 4. Anything below 0x10 is "stop".
  REG_FLAGS_S4,    //8 bits wide, RW.  Flags for section 4 - see section_flags.h for definitions
  REG_OCCUPATION,  //8 bits wide, RO. Bitmask indicating which sections are occupied. LSB is section 1.
};
