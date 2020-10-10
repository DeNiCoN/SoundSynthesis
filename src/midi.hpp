#pragma once
#include <cstdint>

namespace SSynthesis
{
    namespace MIDI
    {
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
            //From there tracks go
        };

        struct MIDITrackChunk
        {
            int8_t chunkID[4]; //Should be "MTrk"
            uint32_t chunkSize; // number of bytes used for all of the events
                                // contained in the track.
            //From there goes event data
        };

        enum class EventType
        {
            NOTE_OFF = 0x8,
            NOTE_ON = 0x9,
            NOTE_AFTERTOUCH = 0xA,
            CONTROLLER = 0xB,
            PROGRAM_CHANGE = 0xC,
            CHANNEL_AFTERTOUCH = 0xD,
            PITCH_BEND = 0xE,
        };

        struct MIDIChannelEvent
        {
            //Starts vith variable-length delta time
            uint8_t eventType : 4;
            uint8_t MIDIChannel : 4;
            uint8_t firstPar;
            uint8_t secondPar;
        };
    }
}
