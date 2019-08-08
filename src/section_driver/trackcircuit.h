void setup_ports();

int8_t get_section_occupancy(); //returns the occupancy status of the sections LSB first. Zero-padded to the right

void set_reversing_bit(int8_t channel);
void clear_reversing_bit(int8_t channel);
