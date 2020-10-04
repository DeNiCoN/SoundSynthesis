#include <iostream>
#include "pcm.hpp"
#include "wav_header.hpp"
#include <fstream>
#include <cmath>

using namespace std;
using namespace SSynthesis;

const string OUTPUT_FILE = "out.wav";
const unsigned SAMPLE_RATE = 44000;

int main(int argc, char *argv[]) {
    auto sine500hz = [](double t) { return sin(500*t*M_PI); };

    ofstream fs(OUTPUT_FILE, std::ios::binary);

    LinearPCM pcm(sine500hz, 0.0, 10.0, SAMPLE_RATE);
    PCMWAVHeader header(pcm.numSamples(), SAMPLE_RATE, sizeof(SampleType)*8);

    fs.write(reinterpret_cast<const char*>(&header.data()), sizeof(WAVHeader));
    fs << pcm;
    return 0;
}
