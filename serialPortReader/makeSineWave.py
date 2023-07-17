import numpy as np

sample_rate = 44100
duration = 1  # Duration in seconds
num_samples = sample_rate * duration

frequency = 2000.0
amplitude = 0.5

# Generate time values
times = np.linspace(0, duration, num_samples, endpoint=False)

# Generate audio samples for a sine wave
samples = amplitude * np.sin(2 * np.pi * frequency * times)

# Convert samples to binary representation
bitstream = [int(sample >= 0) for sample in samples]

# Write the bitstream to a text file
with open("output.txt", "w") as file:
    for bit in bitstream:
        file.write(str(bit))
