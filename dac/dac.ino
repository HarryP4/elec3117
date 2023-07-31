#include <driver/dac.h>
#include <math.h>

#define DAC_RESOLUTION 8    // Set the DAC resolution (8-bit)
#define DAC_CHANNEL DAC_CHANNEL_1   // Use DAC channel 1
#define SAMPLE_RATE 100000   // Set the desired sample rate (in Hz)
#define OFFSET 1       // Set the amplitude of the sine wave
#define FREQUENCY 3000


void setup() {
  dac_output_enable(DAC_CHANNEL); // Enable DAC channel 1
  dac_cw_generator_enable();

  dac_cw_config_t cos_config = {
    .en_ch = DAC_CHANNEL,    /*!< Enable the cosine wave generator of DAC channel. */
    .scale = DAC_CW_SCALE_1,   /*!< Set the amplitude of the cosine wave generator output. */
    .phase = DAC_CW_PHASE_0,   /*!< Set the phase of the cosine wave generator output. */
    .freq = FREQUENCY,          /*!< Set frequency of cosine wave generator output. Range: 130(130Hz) ~ 55000(100KHz). */
    .offset = OFFSET          /*!< Set the voltage value of the DC component of the cosine wave generator output.
                                 Note: Unreasonable settings can cause waveform to be oversaturated. Range: -128 ~ 127. */
  };

  dac_cw_config_t *cos_ptr = &cos_config;

  dac_cw_generator_config(cos_ptr);
}

void loop() {
  /*static unsigned long lastSampleTime = 0;
  unsigned long currentTime = micros();

  // Check if it's time to output the next sample
  if (currentTime - lastSampleTime >= (1000000UL / SAMPLE_RATE)) {
    lastSampleTime = currentTime;

    // Calculate the phase of the sine wave based on the current time and frequency
    float frequency = 3000.0;  // Set the desired frequency of the sine wave (in Hz)
    float phase = (TWO_PI * frequency * currentTime) / 1000000.0;

    // Calculate the next sample value (scaled to DAC_RESOLUTION)
    uint8_t sample = (uint8_t)((AMPLITUDE * sin(phase)) + AMPLITUDE);

    // Output the sample to the DAC
    dac_output_voltage(DAC_CHANNEL, sample);
  }*/
}
