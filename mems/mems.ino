#include <driver/i2s.h>
#include "soc/i2s_reg.h"
#include <driver/dac.h>

// I2S configuration
const int I2S_NUM = 0;               // I2S port number
const int SAMPLE_RATE = 44100;       // Sample rate (Hz)
const int BITS_PER_SAMPLE = 32;      // Bits per sample
const int CHANNELS = 1;              // Number of channels (stereo)

volatile int32_t buff[10000] = {0};
volatile int i = 0;

// Map a 16-bit signed sample to an 8-bit unsigned value for DAC output
uint8_t mapToDACValue(int32_t sample) {
  return map(sample, -2147483648, 2147483647, 0, 255);
}



void setup() {
  Serial.begin(115200);
  // Configure I2S
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT,  // Use 24-bit I2S format
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,  // Receive both channels
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .bits_per_chan = I2S_BITS_PER_CHAN_32BIT
  };

  REG_SET_BIT(I2S_TIMING_REG(I2S_NUM), BIT(9));
  REG_SET_BIT(I2S_CONF_REG(I2S_NUM), I2S_RX_MSB_SHIFT);

  // Configure DAC1
  dac_output_enable(DAC_CHANNEL_1);


  i2s_driver_install((i2s_port_t)I2S_NUM, &i2s_config, 0, NULL);

  // Set pin configurations
  i2s_pin_config_t pin_config = {
    .bck_io_num = 14,      // BCKL
    .ws_io_num = 12,       // LRCL
    .data_out_num = -1,    // Not used in receiver mode
    .data_in_num = 33      // DOUT
  };
  i2s_set_pin((i2s_port_t)I2S_NUM, &pin_config);
}

void loop() {
  uint8_t sample[8] = {0};
  size_t bytes_read = 0;

  // Read a sample from I2S
  i2s_read((i2s_port_t)I2S_NUM, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);

  // Adjust the sample to keep only the 18-bit value
  int32_t adjustedSampleL = ((sample[0] << 16) | (sample[1] << 8) | sample[2]) >> 6;
  int32_t adjustedSampleR = ((sample[4] << 16) | (sample[5] << 8) | sample[6]) >> 6;


  // Print the sample values
  //if (adjustedSampleL && adjustedSampleL != -1 && adjustedSampleL != 1) {
    Serial.print(adjustedSampleL, BIN);
    Serial.print("\n");
  //}
  //if (adjustedSampleR && adjustedSampleR != -1 && adjustedSampleR != 1) {
    Serial.print(adjustedSampleR, BIN);
    Serial.print("\n");
  //}
  //delay(100);

  if (i + 1 < 1000) {
    buff[i] = adjustedSampleL;
    buff[i+1] = adjustedSampleR;
    i += 2;
  } else {
    i = 0;
    while (i < 1000) {
      dac_output_voltage(DAC_CHANNEL_1, mapToDACValue(buff[i]));
      i++;
    }
    i = 0;
  }

}
