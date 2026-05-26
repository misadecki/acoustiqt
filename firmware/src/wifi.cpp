#include "wifi.hh"
#include "protocol.hh"

void WiFi_UDP::connect_to_wifi(const char *ssid, const char *pwd) {
  rgbLedWriteOrdered(LED, LED_COLOR_ORDER_RGB, 0, 0, 0);
  Serial.println("Connecting to WiFi network: " + String(ssid));
  WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
    if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
      this->handle_connected();
    } else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
      Serial.print("WiFi lost connection. Reason:");
      Serial.println(info.wifi_sta_disconnected.reason);
      this->handle_disconnected();
    }
  });
  WiFi.begin(ssid, pwd);

  uint8_t attempts = 0, max_attempts = 30;

  while ((WiFi.status() != WL_CONNECTED) && (attempts < max_attempts)) {
    rgbLedWriteOrdered(LED, LED_COLOR_ORDER_RGB, 0, 0, 255);
    delay(250);
    rgbLedWriteOrdered(LED, LED_COLOR_ORDER_RGB, 0, 0, 0);
    delay(250);

    ++attempts;
  }

  if (WiFi.status() != WL_CONNECTED) {
    while (true) {
      rgbLedWriteOrdered(LED, LED_COLOR_ORDER_RGB, 0, 0, 255);
      delay(1000);
      rgbLedWriteOrdered(LED, LED_COLOR_ORDER_RGB, 0, 0, 0);
      delay(2000);
    }
  }
  rgbLedWriteOrdered(LED, LED_COLOR_ORDER_RGB, 0, 50, 0);

  return;
}

void WiFi_UDP::handle_connected() {
  Serial.print("WiFi connected, IP: ");
  Serial.println(WiFi.localIP());

  if (Protocol::PORT < 1024 || Protocol::PORT > 65535) {
    Serial.println("handle_connected: bad port.");
    return;
  }

  connected = true;
  udp.begin(Protocol::PORT);
}

void WiFi_UDP::handle_disconnected() {
  Serial.println("WiFi lost connection.");
  connected = false;
  udp.stop();
}

void WiFi_UDP::send_audio_data(const int32_t *raw_samples) {
  if (!connected || raw_samples == nullptr) return;

  AudioFrame frame;
  frame.start = Protocol::START_BYTES;
  frame.count = packet_count++;
  for (int32_t i = 0; i < Protocol::SAMPLES_PER_PACKET; ++i)
    frame.samples[i] = raw_samples[i];

  frame.crc = Protocol::calculate_crc8(reinterpret_cast<const uint8_t*>(&frame),
                                       sizeof(AudioFrame) - 1,
                                       Protocol::POLYNOMIAL, Protocol::INIT_VAL);

  int begin_res = udp.beginPacket(UDP_ADDR, Protocol::PORT);
  if (begin_res == 0) {
    Serial.println("beginPacket error");
    return;
  }
  udp.write(reinterpret_cast<const uint8_t*>(&frame), sizeof(AudioFrame));
  udp.endPacket();
  Serial.println("Koniec send_audio_data");
}
