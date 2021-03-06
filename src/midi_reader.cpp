#include "midi_reader.hpp"
#include <exception>


using namespace std;

namespace SSynthesis
{
    namespace MIDI
    {
        Reader::Reader(istream& in)
        {
            auto header = ChunkHeader::readFromStream(in);
            if (header.getChunkID() != ChunkHeader::headerID)
                throw invalid_argument("First chunk of midi file is not header");
            if (header.getChunkSize() != 6)
                throw invalid_argument("Invalid header chunk size");

            m_header = HeaderData::readFromStream(in);
           
        }

        ChunkHeader ChunkHeader::readFromStream(istream &in)
        {
            ChunkHeader result;
            in.read(result.m_chunkID, sizeof(result.m_chunkID));
            result.m_chunkSize = readBigEndian<typeof(result.m_chunkSize)>(in);
            return result;
        }

        HeaderData HeaderData::readFromStream(istream &in)
        {
            HeaderData result;
            result.m_formatType =
                static_cast<FormatType>(readBigEndian<uint16_t>(in));
            result.m_numberOfTracks =
                readBigEndian<typeof(result.m_numberOfTracks)>(in);

            auto time = readBigEndian<uint16_t>(in);
            result.m_isFramesPerSecond = time & 0x8000;
            result.m_rawTimeInfo = time & 0x7FFF;
            return result;
        }

        uint32_t readVariableLength(std::istream& in)
        {
            uint32_t result = 0;
            char next;
            in.read(&next, 1);
            result |= next & 0x7F;
            while (next & 0x80)
            {
                result = result << 7;
                in.read(&next, 1);
                result |= next & 0x7F;
            }
            return result;
        };
    }
}
