void set_section(int fp, int devid, int8_t channel, int16_t signed_speed);
void convert_signed_speed(int16_t signed_speed, uint8_t *unsigned_speed, uint8_t *direction_flag);
void set_section_speed(int fp, int devid, int8_t channel, int8_t speed);
void set_section_flags(int fp, int devid, int8_t channel, int8_t flags);
int8_t get_section_occupation(int fp, int devid);
