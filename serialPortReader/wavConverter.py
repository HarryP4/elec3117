import wave
import struct

def convert_to_wav(input_file, output_file, num_channels, sample_width, frame_rate):
    with open(input_file, 'r') as infile:
        lines = infile.readlines()
    audio_data = [int(line.strip()) for line in lines]

    wav_file = wave.open(output_file, 'w')
    wav_file.setnchannels(num_channels)
    wav_file.setsampwidth(sample_width)
    wav_file.setframerate(frame_rate)

    # Convert the audio data to bytes in little-endian format (PCM)
    audio_bytes = struct.pack('<' + ('h' * len(audio_data)), *audio_data)
    wav_file.writeframes(audio_bytes)
    wav_file.close()

if __name__ == "__main__":
    # Replace these values with the appropriate information about your input audio file
    input_file = 'data.txt'
    output_file = 'audio.wav'
    num_channels = 2  # Change this to 1 for mono or 2 for stereo
    sample_width = 4  # Size of each sample in bytes (e.g., 2 for 16-bit audio, 4 for 32-bit audio)
    frame_rate = 44100  # Number of samples per second (e.g., 44100 for CD-quality audio)

    convert_to_wav(input_file, output_file, num_channels, sample_width, frame_rate)
