#pragma once
#include <variant>
#include <vector>
#include <cstdint>

namespace SSynthesis
{
    namespace MIDI
    {
        namespace Events
        {
            class MIDI
            {
               
            };

            class SysEx
            {

            };

            class Meta
            {

            };
        }

        using Event = std::variant<Events::MIDI, Events::SysEx, Events::Meta>;

        class Track
        {
        public:
            struct Entry
            {
                uint32_t delta;
                Event event;
            };

        private:
            std::vector<Entry> m_events;
        };
    }
}
