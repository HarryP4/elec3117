#include <driver/i2s.h>

// I2S configuration
const int I2S_NUM = 0;               // I2S port number
const int SAMPLE_RATE = 44100;       // Sample rate (Hz)
const int BITS_PER_SAMPLE = 64;      // Bits per sample
const int CHANNELS = 2;              // Number of channels (stereo)

void setup() {
  Serial.begin(115200);

  // Configure I2S
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT,  // Use 24-bit I2S format
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,  // Receive both channels
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };

  i2s_driver_install((i2s_port_t)I2S_NUM, &i2s_config, 0, NULL);

  // Set pin configurations
  i2s_pin_config_t pin_config = {
    .bck_io_num = 26,      // BCKL
    .ws_io_num = 25,       // LRCL
    .data_out_num = -1,    // Not used in receiver mode
    .data_in_num = 33      // DOUT
  };
  i2s_set_pin((i2s_port_t)I2S_NUM, &pin_config);
}

void loop() {
  uint8_t sample[3];
  size_t bytes_read = 0;

  // Read a sample from I2S
  i2s_read((i2s_port_t)I2S_NUM, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);

  // Adjust the sample to keep only the 18-bit value
  int32_t adjustedSample = ((sample[0] << 16) | (sample[1] << 8) | sample[2]) >> 6;

  // Print the sample value
  Serial.println(adjustedSample);
//  delay(100);
}
