#pragma once
#include "midi.hpp"
#include <iostream>

namespace SSynthesis
{
    namespace MIDI
    {
        class Reader
        {
        public:
            //Reads header from input
            Reader(std::istream& input);
           
        private:
            HeaderData m_header;
        };
       
    }
}
