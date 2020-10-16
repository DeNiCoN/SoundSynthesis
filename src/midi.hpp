#pragma once
#include <cstdint>
#include <iostream>
#include <cassert>

namespace SSynthesis
{
    namespace MIDI
    {

        template <typename Integer>
        Integer readBigEndian(std::istream& in, size_t bytes = sizeof(Integer))
        {
            assert(bytes > 0 && bytes <= sizeof(Integer));
            uint8_t raw[sizeof(Integer)];
            in.read(reinterpret_cast<char*>(raw), bytes);
            Integer result = raw[0];
            for (size_t i = 1; i < bytes; i++)
            {
                result = (result << 8) | raw[i];
            }
            return result;
        }

        class Header
        {
        public:
            uint16_t ticksPerByte() const { return _raw; }
            uint16_t frames() const { return _raw & 0x7F00; }
            uint16_t ticks() const { return _raw & 0x00FF; }
        private:
            int8_t chunkID[4]; // "MThd" by default
            uint32_t chunkSize; // should be 6
            uint16_t formatType; // 0-2
            uint16_t numberOfTracks; // 1-65,535

            uint16_t isFramesPerSecond : 1;
            uint16_t _raw : 15;
            //From there tracks go

        };

        struct TrackChunk
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

        struct ChannelEvent
        {
            //Starts vith variable-length delta time
            uint8_t eventType : 4;
            uint8_t MIDIChannel : 4;
            uint8_t firstPar;
            uint8_t secondPar;
        };
    }
}
