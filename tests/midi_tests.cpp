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
