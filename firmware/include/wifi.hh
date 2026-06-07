#ifndef WIFI_HH
#define WIFI_HH

#include <Arduino.h>
#include <NetworkUdp.h>
#include <WiFi.h>
#include "secrets.hh"

class WiFi_UDP {
  volatile bool connected = false;
  uint8_t packet_count = 0;
  bool target_found = false;
  IPAddress target_ip;
  NetworkUDP udp, discovery_udp;
  static constexpr uint8_t LED = 38;
  void connect_to_wifi(const char * ssid, const char *pwd);
  bool find_target();
public:
  inline void init_wifi() { connect_to_wifi(NETWORK_NAME, NETWORK_PASSWORD);
    WiFi.setSleep(false); };
  void handle_connected();
  void handle_disconnected();
  void send_audio_data(const int32_t *raw_samples);
  inline const bool is_connected() const {return connected;}
  inline const bool is_target_found() const {return target_found;}
};

#endif
