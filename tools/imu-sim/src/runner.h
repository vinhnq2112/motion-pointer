#pragma once
// Glue between the core (true motion + sensors) and the learner's plug-in (onImuSample).

#include <string>

#include "imu_plugin.h"
#include "imusim_core.h"

struct Options {
    imusim::Knobs knobs;
    std::string scenario = "all";
    std::string csvPath;
    std::string screenshotPath; // GUI only
    double shotAt = 3.0;        // GUI only: simulated seconds before the screenshot
    bool list = false;
    bool help = false;
};

bool parseOptions(int argc, char** argv, Options& out, std::string& error);
const char* optionsHelp();
const char* pluginName();

class Session {
public:
    explicit Session(const imusim::Knobs& k);
    void setKnobs(const imusim::Knobs& k);
    const imusim::Knobs& knobs() const { return knobs_; }

    // One sample. mid / midRate: intended angles and their rates in the middle of the sample,
    // end: intended angles at the end. Adds tremor, makes the IMU sample, calls onImuSample.
    void step(const imusim::Euler& mid, const imusim::Euler& midRate, const imusim::Euler& end);

    double time = 0;
    ImuSample sample{};
    PointerState pointer{};
    imusim::Euler intended;  // where the user wants to point (no tremor)
    imusim::Euler actual;    // real orientation of the remote (with tremor)
    imusim::V3 omegaBody;    // true angular velocity, body frame, °/s (no noise, no bias)
    imusim::Pointer2 truth;  // ground-truth pointer

private:
    imusim::Knobs knobs_;
    imusim::SensorModel sensor_;
};
