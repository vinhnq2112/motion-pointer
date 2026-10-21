// imu-sim self-test: checks that the simulator's physics is right, using rules with a known answer.
// A simulator that is wrong teaches wrong ideas, so this test must pass before any lesson uses imu-sim.
#include <cmath>
#include <cstdio>

#include "imusim_core.h"

using namespace imusim;

namespace {

int failures = 0;

void check(bool ok, const char* what, double got, double want)
{
    std::printf("[%s] %-58s got %10.4f  want %10.4f\n", ok ? " OK " : "FAIL", what, got, want);
    if (!ok) ++failures;
}

void near(const char* what, double got, double want, double tol) { check(std::fabs(got - want) <= tol, what, got, want); }

// Rotation by a small body-frame rotation vector w (rad), Rodrigues formula.
M3 expSO3(const V3& w)
{
    const double th = std::sqrt(w.x * w.x + w.y * w.y + w.z * w.z);
    M3 K;
    K.m[0][0] = 0;    K.m[0][1] = -w.z; K.m[0][2] = w.y;
    K.m[1][0] = w.z;  K.m[1][1] = 0;    K.m[1][2] = -w.x;
    K.m[2][0] = -w.y; K.m[2][1] = w.x;  K.m[2][2] = 0;
    const M3 K2 = K * K;
    const double a = th < 1e-12 ? 1.0 : std::sin(th) / th;
    const double b = th < 1e-12 ? 0.5 : (1 - std::cos(th)) / (th * th);
    M3 r;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) r.m[i][j] += a * K.m[i][j] + b * K2.m[i][j];
    return r;
}

double angleBetween(const M3& a, const M3& b)
{
    const M3 d = a.transposed() * b;
    const double c = (d.m[0][0] + d.m[1][1] + d.m[2][2] - 1) / 2;
    return std::acos(std::fmax(-1.0, std::fmin(1.0, c))) * 180 / kPi;
}

} // namespace

int main()
{
    std::printf("Accelerometer at rest = gravity seen from the remote\n");
    const V3 flat = gravityReading({0, 0, 0});
    near("flat: accel z = +9.81 (reads 'up')", flat.z, 9.81, 1e-9);
    near("flat: accel x = 0", flat.x, 0, 1e-9);
    const V3 down = gravityReading({0, 30, 0});
    near("pitch +30 (nose down): accel x = -9.81*sin30", down.x, -4.905, 1e-6);
    near("pitch +30: accel z = 9.81*cos30", down.z, 8.4957, 1e-3);
    const V3 rolled = gravityReading({0, 0, 30});
    near("roll +30 (right side down): accel y = +9.81*sin30", rolled.y, 4.905, 1e-6);
    const V3 yawed = gravityReading({70, 0, 0});
    near("yaw 70 only: accel z unchanged (gravity cannot see yaw)", yawed.z, 9.81, 1e-9);

    std::printf("\nSign convention (right-hand rule)\n");
    const V3 noseYaw = bodyToWorld({10, 0, 0}) * V3{1, 0, 0};
    check(noseYaw.y > 0, "yaw +10: nose turns left (world y > 0)", noseYaw.y, 0.1736);
    const V3 nosePitch = bodyToWorld({0, 10, 0}) * V3{1, 0, 0};
    check(nosePitch.z < 0, "pitch +10: nose turns down (world z < 0)", nosePitch.z, -0.1736);
    const V3 leftRoll = bodyToWorld({0, 0, 10}) * V3{0, 1, 0};
    check(leftRoll.z > 0, "roll +10: left side goes up (world z > 0)", leftRoll.z, 0.1736);

    std::printf("\nGyro: integrating the perfect gyro gives back the true angle\n");
    Knobs k;
    const double dt = 1.0 / k.sampleRate;
    const Scenario* yaw = findScenario("yaw-sweep");
    double yawSum = 0, yawErrMax = 0;
    for (int i = 0; i < (int)std::lround(yaw->duration() / dt); ++i) {
        const double tEnd = (i + 1) * dt, tMid = tEnd - dt / 2;
        yawSum += bodyRates(yaw->angles(tMid), yaw->rates(tMid)).z * dt;
        yawErrMax = std::fmax(yawErrMax, std::fabs(yawSum - yaw->angles(tEnd).yaw));
    }
    near("yaw-sweep: max |sum(gyro.z*dt) - true yaw|", yawErrMax, 0, 0.01);

    const Scenario* fig = findScenario("figure");
    M3 R = bodyToWorld(fig->angles(0));
    for (int i = 0; i < (int)std::lround(fig->duration() / dt); ++i) {
        const double tEnd = (i + 1) * dt, tMid = tEnd - dt / 2;
        const V3 w = bodyRates(fig->angles(tMid), fig->rates(tMid));
        const double s = dt * kPi / 180;
        R = R * expSO3({w.x * s, w.y * s, w.z * s});
    }
    near("figure (all 3 axes): orientation error after 20 s, deg", angleBetween(R, bodyToWorld(fig->angles(20))), 0,
         0.05);

    std::printf("\nSensor errors\n");
    k.gyroBias = 0.5;
    SensorModel biased(1);
    double still = 0;
    for (int i = 0; i < 6000; ++i) still += biased.read({0, 0, 0}, gravityReading({}), k).gyro.z * dt;
    near("bias 0.5 deg/s, still 60 s: integrated yaw = 30 deg", still, 30, 1e-6);

    k = Knobs{};
    k.noise = 1.0;
    SensorModel noisy(7);
    double sum = 0, sq = 0;
    const int n = 20000;
    for (int i = 0; i < n; ++i) {
        const double g = noisy.read({0, 0, 0}, gravityReading({}), k).gyro.x;
        sum += g;
        sq += g * g;
    }
    near("noise 1 deg/s: mean of gyro.x", sum / n, 0, 0.03);
    near("noise 1 deg/s: standard deviation of gyro.x", std::sqrt(sq / n - (sum / n) * (sum / n)), 1.0, 0.03);

    std::printf("\nGround-truth pointer\n");
    k = Knobs{};
    near("yaw 0, pitch 0: x = centre", truePointer({0, 0, 0}, k).x, 960, 1e-9);
    near("yaw +10 (nose left): x = 960 - 40*10", truePointer({10, 0, 0}, k).x, 560, 1e-9);
    near("pitch +5 (nose down): y = 540 + 40*5", truePointer({0, 5, 0}, k).y, 740, 1e-9);
    near("yaw -90: x clamped to right edge 1919", truePointer({-90, 0, 0}, k).x, 1919, 1e-9);

    std::printf("\n%s: %d failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
