// imu-sim-headless: runs scripted scenarios without a window and prints numbers.
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

#include "runner.h"

namespace {

struct Result {
    double errEnd = 0, errMax = 0, stillMove = 0;
    bool angles = false;                 // learner called reportAngles()
    double angErrEnd = 0, angErrMax = 0; // max(|yaw error|, |pitch error|), deg
    int offScreen = 0, samples = 0;
    imusim::Euler endAngles;
    PointerState endPointer{};
    imusim::Pointer2 endTruth;
};

Result run(const imusim::Scenario& sc, const imusim::Knobs& k, std::FILE* csv)
{
    Session s(k);
    const PointerState start = s.pointer;
    const double dt = 1.0 / k.sampleRate;
    const int n = (int)std::lround(sc.duration() * k.sampleRate);
    Result r;
    for (int i = 0; i < n; ++i) {
        const double tEnd = (i + 1) * dt;
        const double tMid = tEnd - dt / 2.0;
        s.step(sc.angles(tMid), sc.rates(tMid), sc.angles(tEnd));

        const ReportedAngles& ra = reportedAngles();
        if (ra.valid) {
            r.angles = true;
            r.angErrEnd = std::max(std::fabs(ra.yaw - s.actual.yaw), std::fabs(ra.pitch - s.actual.pitch));
            r.angErrMax = std::max(r.angErrMax, r.angErrEnd);
        }
        const double err = std::hypot(s.pointer.x - s.truth.x, s.pointer.y - s.truth.y);
        r.errMax = std::max(r.errMax, err);
        r.stillMove = std::max(r.stillMove, (double)std::hypot(s.pointer.x - start.x, s.pointer.y - start.y));
        if (s.pointer.x < 0 || s.pointer.y < 0 || s.pointer.x > k.screenW - 1 || s.pointer.y > k.screenH - 1)
            ++r.offScreen;
        if (csv)
            std::fprintf(csv, "%s,%.4f,%.4f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f\n",
                         sc.name.c_str(), s.time, s.sample.dt, s.sample.gyro.x, s.sample.gyro.y, s.sample.gyro.z,
                         s.sample.accel.x, s.sample.accel.y, s.sample.accel.z, s.actual.yaw, s.actual.pitch,
                         s.actual.roll, s.pointer.x, s.pointer.y, s.truth.x, s.truth.y);
    }
    r.samples = n;
    r.endAngles = s.actual;
    r.endPointer = s.pointer;
    r.endTruth = s.truth;
    r.errEnd = std::hypot(s.pointer.x - s.truth.x, s.pointer.y - s.truth.y);
    return r;
}

} // namespace

int main(int argc, char** argv)
{
    Options o;
    std::string error;
    if (!parseOptions(argc, argv, o, error)) {
        std::fprintf(stderr, "error: %s\n%s", error.c_str(), optionsHelp());
        return 2;
    }
    if (o.help) {
        std::printf("imu-sim-headless: run scenarios without a window.\n%s", optionsHelp());
        return 0;
    }
    if (o.list) {
        for (const auto& s : imusim::scenarios())
            std::printf("%-12s %5.1f s  %s\n", s.name.c_str(), s.duration(), s.description.c_str());
        return 0;
    }

    std::FILE* csv = nullptr;
    if (!o.csvPath.empty()) {
        csv = std::fopen(o.csvPath.c_str(), "w");
        if (!csv) {
            std::fprintf(stderr, "error: cannot write %s\n", o.csvPath.c_str());
            return 2;
        }
        std::fprintf(csv, "scenario,t,dt,gyro_x,gyro_y,gyro_z,accel_x,accel_y,accel_z,"
                          "true_yaw,true_pitch,true_roll,pointer_x,pointer_y,true_pointer_x,true_pointer_y\n");
    }

    const imusim::Knobs& k = o.knobs;
    std::printf("imu-sim headless | plugin: %s | noise %.2f deg/s | gyroBias %.2f deg/s | %.0f Hz | "
                "tremor %.2f deg | screen %dx%d\n\n",
                pluginName(), k.noise, k.gyroBias, k.sampleRate, k.handTremor, k.screenW, k.screenH);
    std::printf("%-12s %6s  %-26s %-14s %-14s %9s %9s %10s %11s %15s\n", "scenario", "time",
                "true yaw/pitch/roll (end)", "pointer (end)", "truth (end)", "err end", "err max", "off-screen",
                "moved from", "angle err");
    std::printf("%-12s %6s  %-26s %-14s %-14s %9s %9s %10s %11s %15s\n", "", "s", "deg", "px", "px", "px", "px",
                "samples", "start, px", "end / max, deg");

    std::vector<const imusim::Scenario*> list;
    if (o.scenario == "all")
        for (const auto& s : imusim::scenarios()) list.push_back(&s);
    else
        list.push_back(imusim::findScenario(o.scenario));

    for (const imusim::Scenario* sc : list) {
        const Result r = run(*sc, k, csv);
        char angles[64], ptr[32], tru[32];
        std::snprintf(angles, sizeof angles, "%7.2f %7.2f %7.2f", r.endAngles.yaw, r.endAngles.pitch,
                      r.endAngles.roll);
        std::snprintf(ptr, sizeof ptr, "%6.0f,%6.0f", r.endPointer.x, r.endPointer.y);
        std::snprintf(tru, sizeof tru, "%6.0f,%6.0f", r.endTruth.x, r.endTruth.y);
        char angErr[32] = "-";
        if (r.angles) std::snprintf(angErr, sizeof angErr, "%.2f / %.2f", r.angErrEnd, r.angErrMax);
        std::printf("%-12s %6.1f  %-26s %-14s %-14s %9.1f %9.1f %10d %11.1f %15s\n", sc->name.c_str(),
                    sc->duration(), angles, ptr, tru, r.errEnd, r.errMax, r.offScreen, r.stillMove, angErr);
    }
    std::printf("\nangle err: yaw/pitch from reportAngles() vs the true angles. \"-\" = reportAngles() not called.\n");
    if (csv) {
        std::fclose(csv);
        std::printf("\nCSV written: %s\n", o.csvPath.c_str());
    }
    return 0;
}
