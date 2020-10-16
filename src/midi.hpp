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

        class ChunkHeader
        {
        public:
            static ChunkHeader readFromStream(std::istream& in);

            std::string getChunkID() const
            { return std::string(m_chunkID, sizeof(m_chunkID)); };

            uint32_t getChunkSize() const { return m_chunkSize; }

            static constexpr const char* headerID = "MThd";
            static constexpr const char* trackID = "MTrk";


        private:
            char m_chunkID[4];
            uint32_t m_chunkSize;

            //Only reading is implemented
            ChunkHeader() = default;
        };

        enum class FormatType : uint16_t
        {
            ONE_TRACK = 0,
            PARALLEL_TRACKS = 1,
            SEQUENTIAL_TRAKS = 2
        };

        class HeaderData
        {
        public:
            static HeaderData readFromStream(std::istream& in);
            bool isFramesPerSecond() const { return m_isFramesPerSecond; }
            uint16_t getTicksPerByte() const { return m_rawTimeInfo; }
            uint16_t getFrames() const { return m_rawTimeInfo & 0x7F00; }
            uint16_t getTicks() const { return m_rawTimeInfo & 0x00FF; }
            FormatType getFormatType() const { return m_formatType; }
            uint16_t getNumOfTracks() const { return m_numberOfTracks; }

        private:
            // "MThd" and 6
            FormatType m_formatType; // 0-2
            uint16_t m_numberOfTracks; // 1-65,535

            bool m_isFramesPerSecond;
            uint16_t m_rawTimeInfo;
          
            //Only reading is implemented
            HeaderData() = default;
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
