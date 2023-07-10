import struct
import wave

# Audio parameters
sample_rate = 8000
channels = 2
sample_width = 2  # 16-bit audio

# Read the data from the text file
with open('data.txt', 'r') as file:
    audio_data = [int(line.strip()) for line in file]

# Open a WAV file for writing
with wave.open('audio.wav', 'wb') as wav_file:
    wav_file.setnchannels(channels)
    wav_file.setsampwidth(sample_width)
    wav_file.setframerate(sample_rate)

    # Convert and write the audio data to the WAV file
    for sample in audio_data:
        # Split the sample into left and right channels
        left_sample = sample & 0xFFFF
        right_sample = (sample >> 16) & 0xFFFF

        # Handle signed conversion for 16-bit range (-32768 to 32767)
        if left_sample >= 32768:
            left_sample -= 65536
        if right_sample >= 32768:
            right_sample -= 65536

        # Pack the samples as little-endian 16-bit signed integers
        left_bytes = struct.pack('<h', left_sample)
        right_bytes = struct.pack('<h', right_sample)

        # Write the samples to the WAV file
        wav_file.writeframes(left_bytes)
        wav_file.writeframes(right_bytes)
