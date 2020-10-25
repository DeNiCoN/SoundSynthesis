#include "gtest/gtest.h"
#include "midi_reader.hpp"
#include "midi_track.hpp"
#include <sstream>

using namespace std;

TEST(MIDI, BigEndianRead)
{
    using namespace SSynthesis::MIDI;
    const char data[] {
        0x00,
        0x00, 0x00,
        0x00, 0x00, 0x00,
        0x01,
        0x00, 0x01,
        0x01, 0x00,
        0x01, 0x00, 0x01, 0x00,
        0x11, 0x00, 0x01, 0x00
    };

    istringstream ss(string(data, sizeof(data)));

    ASSERT_EQ(readBigEndian<int32_t>(ss, 1), 0);
    ASSERT_EQ(readBigEndian<int16_t>(ss), 0);
    ASSERT_EQ(readBigEndian<int32_t>(ss, 3), 0);

    ASSERT_EQ(readBigEndian<int8_t>(ss), 0x01);
    ASSERT_EQ(readBigEndian<int16_t>(ss), 0x0001);
    ASSERT_EQ(readBigEndian<int16_t>(ss), 0x0100);
    ASSERT_EQ(readBigEndian<int32_t>(ss), 0x01000100);
    ASSERT_EQ(readBigEndian<uint32_t>(ss), 0x11000100);
}

TEST(MIDI, ReadChunkHeader)
{
    using namespace SSynthesis::MIDI;
    const char headerData[] {
        'M', 'T', 'h', 'd',
        0x00, 0x00, 0x00, 0x06
    };

    istringstream ss(string(headerData, sizeof(headerData)));

    auto midiHeader = ChunkHeader::readFromStream(ss);
    ASSERT_EQ(midiHeader.getChunkID(), ChunkHeader::headerID);
    ASSERT_EQ(midiHeader.getChunkSize(), 6);
}

TEST(MIDI, ReadHeaderData)
{
    using namespace SSynthesis::MIDI;
    const char headerData[] {
        0x00, 0x01, //PARALLEL_TRACKS
        0x00, 0x11, //17 tracks
        0x00, 0x60  //96 ppqn
    };

    istringstream ss(string(headerData, sizeof(headerData)));

    auto midiHeader = HeaderData::readFromStream(ss);
    ASSERT_EQ(midiHeader.getFormatType(), FormatType::PARALLEL_TRACKS);
    ASSERT_EQ(midiHeader.getNumOfTracks(), 17);
    ASSERT_FALSE(midiHeader.isFramesPerSecond());
    ASSERT_EQ(midiHeader.getTicks(), 96);
}

TEST(MIDI, ReadVariableLength)
{
    using namespace SSynthesis::MIDI;
    const unsigned char data[]
    {
        0x00,                   // 00000000
        0x40,                   // 00000040
        0x7F,                   // 0000007F
        0x81, 0x00,             // 00000080
        0xC0, 0x00,             // 00002000
        0xFF, 0x7F,             // 00003FFF
        0x81, 0x80, 0x00,       // 00004000
        0xC0, 0x80, 0x00,       // 00100000
        0xFF, 0xFF, 0x7F,       // 001FFFFF
        0x81, 0x80, 0x80, 0x00, // 00200000
        0xC0, 0x80, 0x80, 0x00, // 08000000
        0xFF, 0xFF, 0xFF, 0x7F  // 0FFFFFFF
    };
  
    istringstream ss(string(reinterpret_cast<const char*>(data), sizeof(data)));
    ASSERT_EQ(readVariableLength(ss), 0x00000000);
    ASSERT_EQ(readVariableLength(ss), 0x00000040);
    ASSERT_EQ(readVariableLength(ss), 0x0000007F);
    ASSERT_EQ(readVariableLength(ss), 0x00000080);
    ASSERT_EQ(readVariableLength(ss), 0x00002000);
    ASSERT_EQ(readVariableLength(ss), 0x00003FFF);
    ASSERT_EQ(readVariableLength(ss), 0x00004000);
    ASSERT_EQ(readVariableLength(ss), 0x00100000);
    ASSERT_EQ(readVariableLength(ss), 0x001FFFFF);
    ASSERT_EQ(readVariableLength(ss), 0x00200000);
    ASSERT_EQ(readVariableLength(ss), 0x08000000);
    ASSERT_EQ(readVariableLength(ss), 0x0FFFFFFF);
}
