/**
I2C virtual registers, in addition to those in common/i2c_defs.h
*/

/**
virtual "register" identities for a controller submodule. This goes higher than reasonably expected
for an actual controller, so that there should always be definitions available.
*/
enum VirtualRegisterController {
  REG_OFFER_FLAGS=2,   //8 bits wide, WO.  1 if an offer is being made and a response is required.
  REG_OFFER_ACCEPT,    //8 bits wide, RO.  If an accept button is pressed the (1-based) controller number that was pressed.
  REG_C_ALL_VALUE,     //stream all controller values. 16 bits x channel count.
  REG_C1_VALUE,        //16 bits wide, RO.  Current setting (unsigned) of controller dial 1
  REG_C2_VALUE,        //16 bits wide, RO.  Current setting (unsigned) of controller dial 2
  REG_C3_VALUE,        //16 bits wide, RO.  Current setting (unsigned) of controller dial 3
  REG_C4_VALUE,        //16 bits wide, RO.  Current setting (unsigned) of controller dial 4
  REG_C5_VALUE,        //16 bits wide, RO.  Current setting (unsigned) of controller dial 1
  REG_C6_VALUE,        //16 bits wide, RO.  Current setting (unsigned) of controller dial 2
  REG_C7_VALUE,        //16 bits wide, RO.  Current setting (unsigned) of controller dial 3
  REG_C8_VALUE,        //16 bits wide, RO.  Current setting (unsigned) of controller dial 4
  REG_C1_FLAGS,        //8 bits wide, RW. Controller flags (as below) for controller dial 1
  REG_C2_FLAGS,        //8 bits wide, RW. Controller flags (as below) for controller dial 2
  REG_C3_FLAGS,        //8 bits wide, RW. Controller flags (as below) for controller dial 3
  REG_C4_FLAGS,        //8 bits wide, RW. Controller flags (as below) for controller dial 4
  REG_C5_FLAGS,        //8 bits wide, RW. Controller flags (as below) for controller dial 1
  REG_C6_FLAGS,        //8 bits wide, RW. Controller flags (as below) for controller dial 2
  REG_C7_FLAGS,        //8 bits wide, RW. Controller flags (as below) for controller dial 3
  REG_C8_FLAGS,        //8 bits wide, RW. Controller flags (as below) for controller dial 4
};

/* controller flags definitions */
#define CF_INACTIVE   0x0;    //controller is not routed to anything
#define CF_ACTIVE     0x1;    //controller is in control of something
#define CF_OVERRIDDEN 0x2;    //controller is nominally in control but has been overriden by the system
