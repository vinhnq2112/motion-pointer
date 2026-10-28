// Tests for exercise 02. Mentor-owned.
// The samples come from the imu-sim physics core: perfect gyro (no noise, no bias), read in the middle of each
// sample, true angles at the end of the sample — the same as the simulator does.
// Usage: test_gyro <oneSample-yaw|oneSample-pitch|yawSweep|pitchSweep|sampleRate|plugin>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#include "imusim_core.h"
#include "runner.h"
#include "student.h"

static int failures = 0;

static void check(bool ok, const char* what)
{
    std::printf("  [%s] %s\n", ok ? " OK " : "FAIL", what);
    if (!ok) ++failures;
}

static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

static ImuSample sample(float dt, Vec3 gyro) { return {dt, gyro, {0, 0, 9.81f}}; }

static void testOneSampleYaw()
{
    std::printf("integrateGyro: một sample, chỉ có yaw\n");
    Angles a;
    integrateGyro(a, sample(0.01f, {0, 0, 15}));
    std::printf("  sample 1: gyro z = 15 °/s, dt = 0.01 s      → yaw %+.4f  pitch %+.4f\n", a.yaw, a.pitch);
    check(a.yaw != 0 || a.pitch != 0, "a đã thay đổi");
    check(near(a.yaw, 0.15f), "yaw = 15 × 0.01 = 0.15°");
    check(near(a.pitch, 0), "pitch không đổi (= 0)");
    integrateGyro(a, sample(0.02f, {30, 0, -5}));
    std::printf("  sample 2: gyro x = 30, z = -5 °/s, dt = 0.02 → yaw %+.4f  pitch %+.4f\n", a.yaw, a.pitch);
    check(near(a.yaw, 0.05f), "yaw = 0.15 + (-5 × 0.02) = 0.05° (cộng dồn, không ghi đè)");
    check(near(a.pitch, 0), "gyro x (roll) không làm đổi yaw hay pitch");
}

static void testOneSamplePitch()
{
    std::printf("integrateGyro: một sample, chỉ có pitch\n");
    Angles a;
    integrateGyro(a, sample(0.02f, {30, 10, 0}));
    std::printf("  gyro x = 30, y = 10 °/s, dt = 0.02 s → yaw %+.4f  pitch %+.4f\n", a.yaw, a.pitch);
    check(near(a.pitch, 0.2f), "pitch = 10 × 0.02 = 0.2°");
    check(near(a.yaw, 0), "yaw không đổi (= 0)");
}

// Runs one scenario through integrateGyro and compares with the true angles. Returns the max error (°).
static double runScenario(const char* name, double rate, bool table)
{
    const imusim::Scenario* sc = imusim::findScenario(name);
    const double dt = 1.0 / rate;
    const int n = (int)std::lround(sc->duration() * rate);
    const int every = (int)std::lround(rate); // one table row per second
    Angles a;
    double maxErr = 0;
    // Header written by hand: printf widths count bytes, and Vietnamese letters take 2–3 bytes.
    if (table) std::printf("   t (s)    yaw thật    yaw bạn  pitch thật  pitch bạn    sai số\n");
    for (int i = 0; i < n; ++i) {
        const double tEnd = (i + 1) * dt, tMid = tEnd - dt / 2;
        const imusim::V3 g = imusim::bodyRates(sc->angles(tMid), sc->rates(tMid));
        integrateGyro(a, sample((float)dt, {(float)g.x, (float)g.y, (float)g.z}));
        const imusim::Euler truth = sc->angles(tEnd);
        const double err = std::max(std::fabs(a.yaw - truth.yaw), std::fabs(a.pitch - truth.pitch));
        maxErr = std::max(maxErr, err);
        if (table && (i + 1) % every == 0)
            std::printf("  %6.1f  %10.2f %10.2f  %10.2f %10.2f  %8.3f\n", tEnd, truth.yaw, a.yaw, truth.pitch,
                        a.pitch, err);
    }
    return maxErr;
}

static void testSweep(const char* name)
{
    std::printf("integrateGyro: scenario %s, 100 Hz, noise tắt\n", name);
    const double maxErr = runScenario(name, 100, true);
    std::printf("  sai số lớn nhất trong cả scenario: %.3f°\n", maxErr);
    check(maxErr < 1.0, "sai số yaw và pitch luôn < 1°");
}

static void testSampleRate()
{
    std::printf("integrateGyro: scenario yaw-sweep và pitch-sweep ở 50 Hz và 200 Hz\n");
    for (double rate : {50.0, 200.0}) {
        for (const char* name : {"yaw-sweep", "pitch-sweep"}) {
            const double maxErr = runScenario(name, rate, false);
            char what[96];
            std::snprintf(what, sizeof what, "%s ở %.0f Hz (dt = %.3f s): sai số lớn nhất %.3f° < 1°", name, rate,
                          1.0 / rate, maxErr);
            check(maxErr < 1.0, what);
        }
    }
}

static void testPlugin()
{
    std::printf("onImuSample: chạy scenario yaw-sweep rồi pitch-sweep qua simulator (Session)\n");
    imusim::Knobs k;
    Session s(k);
    const PointerState start = s.pointer;
    double maxErr = 0;
    bool everValid = true, pointerStill = true;
    imusim::Euler offset; // pitch-sweep starts where yaw-sweep ended (both end at 0, so offset stays 0)
    for (const char* name : {"yaw-sweep", "pitch-sweep"}) {
        const imusim::Scenario* sc = imusim::findScenario(name);
        const double dt = 1.0 / k.sampleRate;
        const int n = (int)std::lround(sc->duration() * k.sampleRate);
        for (int i = 0; i < n; ++i) {
            const double tEnd = (i + 1) * dt, tMid = tEnd - dt / 2;
            s.step(sc->angles(tMid) + offset, sc->rates(tMid), sc->angles(tEnd) + offset);
            const ReportedAngles& r = reportedAngles();
            if (!r.valid) { everValid = false; continue; }
            maxErr = std::max({maxErr, std::fabs(r.yaw - s.actual.yaw), std::fabs(r.pitch - s.actual.pitch)});
            if (s.pointer.x != start.x || s.pointer.y != start.y) pointerStill = false;
            if (i == n / 4)
                std::printf("  %s t = %.2f s: yaw thật %+.2f, bạn báo %+.2f · pitch thật %+.2f, bạn báo %+.2f\n",
                            name, tEnd, s.actual.yaw, r.yaw, s.actual.pitch, r.pitch);
        }
    }
    check(everValid, "onImuSample gọi reportAngles ở mọi sample");
    char what[96];
    std::snprintf(what, sizeof what, "góc báo qua reportAngles khớp góc thật: sai số lớn nhất %.3f° < 1°", maxErr);
    check(everValid && maxErr < 1.0, what);
    check(pointerStill, "con trỏ p không bị sửa (bài Ngày 3 mới làm)");
}

int main(int argc, char** argv)
{
    const char* name = argc > 1 ? argv[1] : "";
    if (!std::strcmp(name, "oneSample-yaw")) testOneSampleYaw();
    else if (!std::strcmp(name, "oneSample-pitch")) testOneSamplePitch();
    else if (!std::strcmp(name, "yawSweep")) testSweep("yaw-sweep");
    else if (!std::strcmp(name, "pitchSweep")) testSweep("pitch-sweep");
    else if (!std::strcmp(name, "sampleRate")) testSampleRate();
    else if (!std::strcmp(name, "plugin")) testPlugin();
    else {
        std::printf("usage: test_gyro <oneSample-yaw|oneSample-pitch|yawSweep|pitchSweep|sampleRate|plugin>\n");
        return 2;
    }
    std::printf("%s\n", failures ? "FAILED" : "PASSED");
    return failures ? 1 : 0;
}
