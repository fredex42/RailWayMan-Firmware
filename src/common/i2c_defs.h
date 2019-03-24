/**
I2c virtual registers.
All modules have the first two "registers" in common, which are the device ID/channels and firmwire revision numbers.
Beyond this each device class has its own "registers" allowing read/write of specific values.
*/

enum VirtualRegisterCommon {
  REG_DEVID=0,        //8 bits wide, RO. Upper 3 bits are the channel count. Lower 3 bits are the device ID (from enum DeviceID).
  REG_REVISION        //16 bits wide, RO. Firmware revision (build) number.
};

enum DeviceID {
  DEVID_INVALID=0,      //NOT a valid device ID (000)
  DEVID_TRACKSECTION,   //a track section control device (001)
  DEVID_CONTROLLER,     //a control surface device (010)
  DEVID_PMOTOR,         //a point/semaphore signal control device (011)

};
