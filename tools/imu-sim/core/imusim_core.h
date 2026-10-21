#pragma once
// imu-sim core: true motion of the remote, what an ideal IMU would measure, and sensor errors.
// Units: angles in degrees, angular rates in °/s, acceleration in m/s², time in s, pointer in px.
// Frames: body frame x = toward the TV, y = left, z = up. World frame = body frame when all angles are 0.
// Euler order: yaw (about z), then pitch (about y), then roll (about x). R = Rz(yaw) * Ry(pitch) * Rx(roll).
// Sign check: pitch > 0 turns the nose DOWN, yaw > 0 turns the nose LEFT, roll > 0 lowers the right side.

#include <random>
#include <string>
#include <vector>

namespace imusim {

constexpr double kGravity = 9.81;
constexpr double kPi = 3.14159265358979323846;

struct V3 { double x = 0, y = 0, z = 0; };
struct Euler { double yaw = 0, pitch = 0, roll = 0; };

Euler operator+(const Euler& a, const Euler& b);
Euler operator-(const Euler& a, const Euler& b);
Euler operator*(const Euler& a, double s);

struct M3 {
    double m[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    V3 operator*(const V3& v) const;
    M3 operator*(const M3& o) const;
    M3 transposed() const;
};

struct Knobs {
    double noise = 0;        // gyro noise, standard deviation in °/s (accel noise = 0.1 × this, in m/s²)
    double gyroBias = 0;     // constant gyro error in °/s, added to x, y and z
    double sampleRate = 100; // samples per second
    double handTremor = 0;   // hand shake amplitude in degrees (8–11 Hz)
    int screenW = 1920;
    int screenH = 1080;
    double pointerGain = 40; // px per degree, used only by the ground-truth pointer
    unsigned seed = 1;       // random seed for noise
};

// Rotation matrix that turns a body-frame vector into a world-frame vector.
M3 bodyToWorld(const Euler& e);
// True angular velocity in the body frame (what a perfect gyro reads), °/s.
V3 bodyRates(const Euler& angles, const Euler& eulerRates);
// What a perfect accelerometer reads when the remote does not accelerate: gravity, seen in the body frame.
V3 gravityReading(const Euler& e);

// Hand tremor added on top of the intended motion.
Euler tremor(double t, double amplitude);
Euler tremorRate(double t, double amplitude);

// Ground-truth pointer: where the user wants to point. Centre of the screen when yaw = pitch = 0.
struct Pointer2 { double x = 0, y = 0; };
Pointer2 truePointer(const Euler& intended, const Knobs& k);

// Adds bias and noise to perfect readings.
struct Reading { V3 gyro, accel; };
class SensorModel {
public:
    explicit SensorModel(unsigned seed);
    Reading read(const V3& trueGyro, const V3& trueAccel, const Knobs& k);
private:
    std::mt19937 rng_;
    std::normal_distribution<double> normal_{0.0, 1.0};
};

// Scripted motion: smooth moves between keyframes (speed is 0 at every keyframe).
struct Keyframe { double t; Euler e; };
struct Scenario {
    std::string name;
    std::string description;
    std::vector<Keyframe> keys;
    double duration() const;
    Euler angles(double t) const;
    Euler rates(double t) const;
};
const std::vector<Scenario>& scenarios();
const Scenario* findScenario(const std::string& name);

} // namespace imusim
