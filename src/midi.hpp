#pragma once
#include <cstdint>


struct MIDIHeader
{
    int8_t chunkID[4]; // "MThd" by default
    uint32_t chunkSize; // should be 6
    uint16_t formatType; // 0-2
    uint16_t numberOfTracks; // 1-65,535

    uint16_t isFramesPerSecond : 1;
    uint16_t _raw : 15;
    uint16_t ticksPerByte() const { return _raw; }
    uint16_t frames() const { return _raw & 0x7F00; }
    uint16_t ticks() const { return _raw & 0x00FF; }
};
