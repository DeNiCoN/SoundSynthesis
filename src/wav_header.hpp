#pragma once
#include <cstdint>

namespace SSynthesis
{

    struct WAVHeader
    {
        uint8_t chunkId[4];
        uint32_t chunkSize;

        uint8_t format[4];
        uint8_t subchunk1Id[4];
        uint32_t subchunk1Size;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;

        uint8_t subchunk2Id[4];
        uint32_t subchunk2Size;
    };

    class PCMWAVHeader
    {
    public:
        constexpr PCMWAVHeader(uint32_t numSamples,
                               uint32_t sampleRate,
                               uint16_t bitsPerSample,
                               uint16_t numChannels = 1)
            : m_header({
                    .chunkId {'R', 'I', 'F', 'F'},
                    // 36 is header size - 8
                    .chunkSize = 36 + numSamples * numChannels * bitsPerSample / 8,
                    .format {'W', 'A', 'V', 'E'},
                    .subchunk1Id {'f', 'm', 't', ' '},
                    .subchunk1Size = 16,
                    .audioFormat = 1,
                    .numChannels = numChannels,
                    .sampleRate = sampleRate,
                    .byteRate = numChannels * sampleRate * bitsPerSample / 8, //8 is byte size
                    .blockAlign = static_cast<unsigned short>(numChannels * bitsPerSample / 8),
                    .bitsPerSample = bitsPerSample,
                    .subchunk2Id {'d', 'a', 't', 'a'},
                    .subchunk2Size = numSamples * numChannels * bitsPerSample / 8
                })
        {}

        const WAVHeader& data() { return m_header; }
    private:
        WAVHeader m_header;
    };

}
