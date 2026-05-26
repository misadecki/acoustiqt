#include "microphone.hh"
#include "driver/i2s_common.h"
#include "audio_config.hh"

Microphone::Microphone(int32_t pin_sck, int32_t pin_ws, int32_t pin_sd) {
  i2s_chan_config_t chan_config = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0,
                                                          I2S_ROLE_MASTER);
  i2s_new_channel(&chan_config, NULL, &rx_handle);

  i2s_std_config_t std_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(AudioConfig::SAMPLE_RATE),
    .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT,
                                                I2S_SLOT_MODE_MONO),
    .gpio_cfg = {
      .mclk = I2S_GPIO_UNUSED,
      .bclk = static_cast<gpio_num_t>(pin_sck),
      .ws = static_cast<gpio_num_t>(pin_ws),
      .dout = I2S_GPIO_UNUSED,
      .din = static_cast<gpio_num_t>(pin_sd)
    }
  };
  std_cfg.slot_cfg.slot_mask = I2S_STD_SLOT_LEFT;

  i2s_channel_init_std_mode(rx_handle, &std_cfg);
  i2s_channel_enable(rx_handle);
  Serial.println("Stworzono obiekt mikrofonu");
}

int32_t Microphone::read_raw_data_to_buffer() {
  size_t bytes_read = 0;
  esp_err_t err = i2s_channel_read(rx_handle, buffer, sizeof(int32_t) *
                                   BUFFER_SIZE, &bytes_read, pdMS_TO_TICKS(100)); 
  if (err != ESP_OK) {
    Serial.printf("I2S: read_raw: %s\n", esp_err_to_name(err));
    last_sample = 0;
    return -1;
  }
  last_sample = static_cast<int32_t>(bytes_read / sizeof(int32_t));
  return last_sample;
}
