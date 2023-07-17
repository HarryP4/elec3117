const int leftMicrophonePin = 34;   // Analog input pin for the left microphone
const int rightMicrophonePin = 35;  // Analog input pin for the right microphone
const int samplingFrequency = 44100; // Sampling frequency in Hz (e.g., 44100 Hz for CD quality audio)
const int timerIntervalMicros = 1000000 / samplingFrequency;

void setup() {
  Serial.begin(115200);

  // Configure ADC
  analogReadResolution(12); // 12-bit resolution (0-4095)

  // Set up the timer to trigger the sampling at the specified frequency
  timerBegin(0, 80, true);  // Timer 0, prescaler 80 (1MHz), count up
  timerAttachInterrupt(0, sampleAudio, true);  // Attach the interrupt
  timerAlarmWrite(0, timerIntervalMicros, true);  // Set the interrupt period
  timerAlarmEnable(0);  // Enable the timer interrupt
}

void loop() {
  // Your main loop code if needed, but no need to do anything here for sampling audio.
  //sampleAudio();
}

void sampleAudio() {
  // Read analog values from the microphones
  int leftRawValue = analogRead(leftMicrophonePin);
  int rightRawValue = analogRead(rightMicrophonePin);

  // Print the raw values to the serial monitor
  //Serial.print("Left Microphone: ");
  Serial.println(leftRawValue);

  //Serial.print("Right Microphone: ");
  Serial.println(rightRawValue);

  // You can add additional code here to process the raw audio data, save it to a file, or send it to other devices.
}
