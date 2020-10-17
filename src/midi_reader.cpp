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
    }
}
