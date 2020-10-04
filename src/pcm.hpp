#pragma once
#include <concepts>
#include <cstdint>
#include <iostream>
#include <limits>

namespace SSynthesis
{
    template <typename T>
    concept AnalogFunction = requires(T a, double t)
    {
        { a(t) } -> std::convertible_to<double>;
    };

    using SampleType = int8_t;
    template <AnalogFunction Func>
    class LinearPCM
    {
    public:
        LinearPCM(Func func, double from, double to, unsigned sampleRate)
            : func(func), from(from), to(to), sampleRate(sampleRate)
        {

        }

        SampleType at(double t) const
        { return SampleType(func(t) * std::numeric_limits<SampleType>::max())
                ^ std::numeric_limits<SampleType>::min(); }

        SampleType operator()(double t) const { return at(t); }
        static unsigned calculateNumSamples(double duration, unsigned sampleRate)
        {
            return duration * sampleRate;
        }
        unsigned numSamples() const { return calculateNumSamples(to - from, sampleRate); }
        const Func func;
        const double from;
        const double to;
        const unsigned sampleRate;
    };

    template <AnalogFunction Func>
    std::ostream& operator<<(std::ostream& out, LinearPCM<Func> pcm)
    {
        double step = 1.0 / pcm.sampleRate;
        for (unsigned i = 0; i < pcm.numSamples(); i++)
        {
            out << pcm.at(pcm.from + i / static_cast<double>(pcm.sampleRate));
        }
        return out;
    }
}
