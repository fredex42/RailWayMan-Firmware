//misc useful functions for controller.
//Need to include stdint.h somewhere before this.

void decode_controller_value(uint16_t raw_value, uint8_t *sign_flag, uint8_t *output_value);
