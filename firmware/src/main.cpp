#include <Arduino.h>
#include "microphone.hh"
#include "wifi.hh"

Microphone *mic;
WiFi_UDP *wifi;

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(1000);
  wifi = new WiFi_UDP();
  wifi->init_wifi();
  mic = new Microphone(12, 13, 14);
  Serial.println("[ID] ESP32 S3 DEV KIT");
}

void loop() {
  if (!wifi->is_connected()) {
    delay(100);
    return;
  }

  int32_t samples = mic->read_raw_data_to_buffer();

  if (samples == 0) {
    delay(500);
    return;
  }

  if (samples > 0) {
    const int32_t* data = mic->get_buffer();
    wifi->send_audio_data(data);
  }
} 
