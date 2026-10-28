#include "runner.h"

#include <cstdio>
#include <cstdlib>

#ifndef IMUSIM_PLUGIN_NAME
#define IMUSIM_PLUGIN_NAME "unknown"
#endif

using imusim::Euler;

const char* pluginName() { return IMUSIM_PLUGIN_NAME; }

namespace {
ReportedAngles gReported;
}

void reportAngles(float yaw, float pitch, float roll) { gReported = {true, yaw, pitch, roll}; }
const ReportedAngles& reportedAngles() { return gReported; }

const char* optionsHelp()
{
    return "Options:\n"
           "  --scenario NAME   still | yaw-sweep | pitch-sweep | roll-twist | figure | wild | all\n"
           "  --noise X         gyro noise, deg/s (default 0)\n"
           "  --bias X          gyro bias, deg/s (default 0)\n"
           "  --rate X          sample rate, Hz (default 100)\n"
           "  --tremor X        hand tremor amplitude, deg (default 0)\n"
           "  --screen WxH      TV size in px (default 1920x1080)\n"
           "  --gain X          ground-truth pointer gain, px/deg (default 40)\n"
           "  --seed N          random seed (default 1)\n"
           "  --csv PATH        write every sample to a CSV file (headless)\n"
           "  --screenshot PATH save a PNG and quit (window app)\n"
           "  --at SECONDS      simulated time of the screenshot (default 3)\n"
           "  --mystery         window app: keys 1-3 play mystery motions, answers hidden (A shows them)\n"
           "  --list            list scenarios\n";
}

bool parseOptions(int argc, char** argv, Options& o, std::string& error)
{
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        auto next = [&](const char*& v) {
            if (i + 1 >= argc) { error = "missing value after " + a; return false; }
            v = argv[++i];
            return true;
        };
        const char* v = nullptr;
        if (a == "--help" || a == "-h") o.help = true;
        else if (a == "--list") o.list = true;
        else if (a == "--mystery") o.mystery = true;
        else if (a == "--scenario") { if (!next(v)) return false; o.scenario = v; }
        else if (a == "--noise") { if (!next(v)) return false; o.knobs.noise = std::atof(v); }
        else if (a == "--bias") { if (!next(v)) return false; o.knobs.gyroBias = std::atof(v); }
        else if (a == "--rate") { if (!next(v)) return false; o.knobs.sampleRate = std::atof(v); }
        else if (a == "--tremor") { if (!next(v)) return false; o.knobs.handTremor = std::atof(v); }
        else if (a == "--gain") { if (!next(v)) return false; o.knobs.pointerGain = std::atof(v); }
        else if (a == "--seed") { if (!next(v)) return false; o.knobs.seed = (unsigned)std::atoi(v); }
        else if (a == "--csv") { if (!next(v)) return false; o.csvPath = v; }
        else if (a == "--screenshot") { if (!next(v)) return false; o.screenshotPath = v; }
        else if (a == "--at") { if (!next(v)) return false; o.shotAt = std::atof(v); }
        else if (a == "--screen") {
            if (!next(v)) return false;
            if (std::sscanf(v, "%dx%d", &o.knobs.screenW, &o.knobs.screenH) != 2) {
                error = "--screen needs WxH, e.g. 1920x1080";
                return false;
            }
        } else {
            error = "unknown option: " + a;
            return false;
        }
    }
    if (o.knobs.sampleRate <= 0) { error = "--rate must be > 0"; return false; }
    if (o.scenario != "all" && !imusim::findScenario(o.scenario)) {
        error = "unknown scenario: " + o.scenario + " (use --list)";
        return false;
    }
    return true;
}

Session::Session(const imusim::Knobs& k) : knobs_(k), sensor_(k.seed)
{
    gReported = {};
    pointer = {k.screenW / 2.0f, k.screenH / 2.0f};
    truth = imusim::truePointer(intended, knobs_);
    sample.accel = {0, 0, (float)imusim::kGravity};
}

void Session::setKnobs(const imusim::Knobs& k) { knobs_ = k; }

void Session::step(const Euler& mid, const Euler& midRate, const Euler& end)
{
    const double dt = 1.0 / knobs_.sampleRate;
    const double amp = knobs_.handTremor;
    const double tMid = time + dt / 2.0;
    time += dt;

    intended = end;
    actual = end + imusim::tremor(time, amp);
    omegaBody = imusim::bodyRates(mid + imusim::tremor(tMid, amp), midRate + imusim::tremorRate(tMid, amp));
    const imusim::Reading r = sensor_.read(omegaBody, imusim::gravityReading(actual), knobs_);

    sample.dt = (float)dt;
    sample.gyro = {(float)r.gyro.x, (float)r.gyro.y, (float)r.gyro.z};
    sample.accel = {(float)r.accel.x, (float)r.accel.y, (float)r.accel.z};
    onImuSample(sample, pointer);
    truth = imusim::truePointer(intended, knobs_);
}
