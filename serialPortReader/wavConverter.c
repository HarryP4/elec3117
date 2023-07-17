#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct {
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
    char subchunk1_id[4];
    uint32_t subchunk1_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char subchunk2_id[4];
    uint32_t subchunk2_size;
} WavHeader;
#pragma pack(pop)

int main() {
    const char* input_filename = "data.txt";
    const char* output_filename = "audio.wav";
    const uint16_t num_channels = 2; // Two channels for stereo
    const uint32_t sample_rate = 44100; // Number of samples per second (e.g., 44100 for CD-quality audio)
    const uint16_t bits_per_sample = 32; // Number of bits per sample (e.g., 16 for 16-bit audio, 32 for 32-bit audio)

    FILE* input_file = fopen(input_filename, "r");
    if (!input_file) {
        printf("Error opening the input file.\n");
        return 1;
    }

    FILE* output_file = fopen(output_filename, "wb");
    if (!output_file) {
        printf("Error creating the output file.\n");
        fclose(input_file);
        return 1;
    }

    // Calculate the required sizes
    fseek(input_file, 0L, SEEK_END);
    size_t num_samples_per_channel = ftell(input_file) / (num_channels * sizeof(int));
    uint32_t data_size = num_samples_per_channel * num_channels * (bits_per_sample / 8);
    uint32_t total_size = 36 + data_size;

    // Fill the WAV header
    WavHeader wav_header;
    memcpy(wav_header.chunk_id, "RIFF", 4);
    wav_header.chunk_size = total_size - 8;
    memcpy(wav_header.format, "WAVE", 4);
    memcpy(wav_header.subchunk1_id, "fmt ", 4);
    wav_header.subchunk1_size = 16; // PCM format chunk size
    wav_header.audio_format = 1; // PCM format code
    wav_header.num_channels = num_channels;
    wav_header.sample_rate = sample_rate;
    wav_header.byte_rate = sample_rate * num_channels * (bits_per_sample / 8);
    wav_header.block_align = num_channels * (bits_per_sample / 8);
    wav_header.bits_per_sample = bits_per_sample;
    memcpy(wav_header.subchunk2_id, "data", 4);
    wav_header.subchunk2_size = data_size;

    // Write the WAV header to the output file
    fwrite(&wav_header, sizeof(wav_header), 1, output_file);

    // Read the audio samples from the input file and write them to the output file
    rewind(input_file);
    int sample;
    while (fscanf(input_file, "%d", &sample) == 1) {
        for (uint16_t channel = 0; channel < num_channels; channel++) {
            fwrite(&sample, bits_per_sample / 8, 1, output_file);
        }
    }

    // Close the files
    fclose(input_file);
    fclose(output_file);

    printf("WAV file created successfully: %s\n", output_filename);

    return 0;
}
