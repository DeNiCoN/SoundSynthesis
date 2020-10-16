#include "gtest/gtest.h"
#include "midi.hpp"
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
