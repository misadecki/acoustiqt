#ifndef MICROPHONE_HH
#define MICROPHONE_HH

#include <Arduino.h>
#include "driver/i2s_std.h"
#include "protocol.hh"

class Microphone {
  static constexpr uint16_t BUFFER_SIZE = Protocol::SAMPLES_PER_PACKET;
  i2s_chan_handle_t rx_handle;
  int32_t buffer[BUFFER_SIZE];
  int32_t last_sample;
  inline void togglePin(int pin) const { digitalWrite(pin, !digitalRead(pin)); }
public:
  Microphone(int32_t pin_sck, int32_t pin_ws, int32_t pin_sd);
  inline const int32_t* get_buffer() const { return buffer; }
  inline size_t get_buffer_sample_count() const { return last_sample; }
  int32_t read_raw_data_to_buffer();
};

#endif
