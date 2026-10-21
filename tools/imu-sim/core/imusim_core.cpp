#include "imusim_core.h"

#include <algorithm>
#include <cmath>

namespace imusim {

namespace {
double rad(double deg) { return deg * kPi / 180.0; }
double smooth(double u) { return u * u * (3.0 - 2.0 * u); }
double smoothSlope(double u) { return 6.0 * u * (1.0 - u); }
} // namespace

Euler operator+(const Euler& a, const Euler& b) { return {a.yaw + b.yaw, a.pitch + b.pitch, a.roll + b.roll}; }
Euler operator-(const Euler& a, const Euler& b) { return {a.yaw - b.yaw, a.pitch - b.pitch, a.roll - b.roll}; }
Euler operator*(const Euler& a, double s) { return {a.yaw * s, a.pitch * s, a.roll * s}; }

V3 M3::operator*(const V3& v) const
{
    return {m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z};
}

M3 M3::operator*(const M3& o) const
{
    M3 r;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            r.m[i][j] = 0;
            for (int k = 0; k < 3; ++k) r.m[i][j] += m[i][k] * o.m[k][j];
        }
    return r;
}

M3 M3::transposed() const
{
    M3 r;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) r.m[i][j] = m[j][i];
    return r;
}

M3 bodyToWorld(const Euler& e)
{
    const double cy = std::cos(rad(e.yaw)), sy = std::sin(rad(e.yaw));
    const double cp = std::cos(rad(e.pitch)), sp = std::sin(rad(e.pitch));
    const double cr = std::cos(rad(e.roll)), sr = std::sin(rad(e.roll));
    M3 r;
    r.m[0][0] = cy * cp; r.m[0][1] = cy * sp * sr - sy * cr; r.m[0][2] = cy * sp * cr + sy * sr;
    r.m[1][0] = sy * cp; r.m[1][1] = sy * sp * sr + cy * cr; r.m[1][2] = sy * sp * cr - cy * sr;
    r.m[2][0] = -sp;     r.m[2][1] = cp * sr;                r.m[2][2] = cp * cr;
    return r;
}

V3 bodyRates(const Euler& a, const Euler& r)
{
    const double cp = std::cos(rad(a.pitch)), sp = std::sin(rad(a.pitch));
    const double cr = std::cos(rad(a.roll)), sr = std::sin(rad(a.roll));
    return {r.roll - r.yaw * sp,
            r.pitch * cr + r.yaw * cp * sr,
            -r.pitch * sr + r.yaw * cp * cr};
}

V3 gravityReading(const Euler& e)
{
    // R^T * (0, 0, g): the "up" direction seen from the remote.
    const double cp = std::cos(rad(e.pitch)), sp = std::sin(rad(e.pitch));
    const double cr = std::cos(rad(e.roll)), sr = std::sin(rad(e.roll));
    return {-kGravity * sp, kGravity * cp * sr, kGravity * cp * cr};
}

Euler tremor(double t, double a)
{
    const double w = 2.0 * kPi;
    return {a * (0.6 * std::sin(w * 9.1 * t) + 0.4 * std::sin(w * 11.3 * t + 1.0)),
            a * (0.6 * std::sin(w * 8.3 * t + 2.0) + 0.4 * std::sin(w * 10.7 * t)),
            a * 0.5 * std::sin(w * 9.7 * t + 0.5)};
}

Euler tremorRate(double t, double a)
{
    const double w = 2.0 * kPi;
    return {a * (0.6 * w * 9.1 * std::cos(w * 9.1 * t) + 0.4 * w * 11.3 * std::cos(w * 11.3 * t + 1.0)),
            a * (0.6 * w * 8.3 * std::cos(w * 8.3 * t + 2.0) + 0.4 * w * 10.7 * std::cos(w * 10.7 * t)),
            a * 0.5 * w * 9.7 * std::cos(w * 9.7 * t + 0.5)};
}

Pointer2 truePointer(const Euler& e, const Knobs& k)
{
    // Nose left (yaw > 0) moves the pointer left; nose down (pitch > 0) moves it down (screen y grows down).
    const double x = k.screenW / 2.0 - k.pointerGain * e.yaw;
    const double y = k.screenH / 2.0 + k.pointerGain * e.pitch;
    return {std::clamp(x, 0.0, k.screenW - 1.0), std::clamp(y, 0.0, k.screenH - 1.0)};
}

SensorModel::SensorModel(unsigned seed) : rng_(seed) {}

Reading SensorModel::read(const V3& g, const V3& a, const Knobs& k)
{
    Reading r;
    r.gyro = {g.x + k.gyroBias + k.noise * normal_(rng_),
              g.y + k.gyroBias + k.noise * normal_(rng_),
              g.z + k.gyroBias + k.noise * normal_(rng_)};
    const double an = 0.1 * k.noise;
    r.accel = {a.x + an * normal_(rng_), a.y + an * normal_(rng_), a.z + an * normal_(rng_)};
    return r;
}

double Scenario::duration() const { return keys.empty() ? 0.0 : keys.back().t; }

Euler Scenario::angles(double t) const
{
    if (keys.empty()) return {};
    if (t <= keys.front().t) return keys.front().e;
    for (size_t i = 0; i + 1 < keys.size(); ++i) {
        const Keyframe& k0 = keys[i];
        const Keyframe& k1 = keys[i + 1];
        if (t <= k1.t) return k0.e + (k1.e - k0.e) * smooth((t - k0.t) / (k1.t - k0.t));
    }
    return keys.back().e;
}

Euler Scenario::rates(double t) const
{
    for (size_t i = 0; i + 1 < keys.size(); ++i) {
        const Keyframe& k0 = keys[i];
        const Keyframe& k1 = keys[i + 1];
        if (t >= k0.t && t <= k1.t) {
            const double span = k1.t - k0.t;
            return (k1.e - k0.e) * (smoothSlope((t - k0.t) / span) / span);
        }
    }
    return {};
}

const std::vector<Scenario>& scenarios()
{
    static const std::vector<Scenario> list = {
        {"still", "Remote nằm yên 30 s, chĩa thẳng vào TV",
         {{0, {0, 0, 0}}, {30, {0, 0, 0}}}},
        {"yaw-sweep", "Chỉ yaw: quay mũi remote sang trái 20°, sang phải 20°, rồi về giữa",
         {{0, {0, 0, 0}}, {1, {0, 0, 0}}, {3, {20, 0, 0}}, {4, {20, 0, 0}},
          {7, {-20, 0, 0}}, {8, {-20, 0, 0}}, {10, {0, 0, 0}}}},
        {"pitch-sweep", "Chỉ pitch: chúc mũi remote xuống 12°, ngẩng lên 12°, rồi về giữa",
         {{0, {0, 0, 0}}, {1, {0, 0, 0}}, {3, {0, 12, 0}}, {4, {0, 12, 0}},
          {7, {0, -12, 0}}, {8, {0, -12, 0}}, {10, {0, 0, 0}}}},
        {"roll-twist", "Chỉ roll: vặn cổ tay sang phải 45°, sang trái 45°, rồi về giữa",
         {{0, {0, 0, 0}}, {1, {0, 0, 0}}, {2.5, {0, 0, 45}}, {3.5, {0, 0, 45}},
          {6, {0, 0, -45}}, {7, {0, 0, -45}}, {8, {0, 0, 0}}}},
        {"figure", "Kết hợp yaw + pitch + một chút roll trong 20 s",
         {{0, {0, 0, 0}}, {2, {15, -8, 5}}, {5, {-15, -8, -5}}, {8, {-15, 8, 10}},
          {11, {15, 8, -10}}, {14, {0, 0, 0}}, {16, {0, 0, 0}}, {18, {10, 5, 20}}, {20, {0, 0, 0}}}},
        {"wild", "Vung tay mạnh và nhanh, góc rất lớn (thử screen bound)",
         {{0, {0, 0, 0}}, {0.8, {70, -40, 20}}, {1.6, {-80, 50, -30}}, {2.4, {60, 40, 10}},
          {3.2, {-70, -45, 0}}, {4, {0, 0, 0}}, {4.5, {0, 0, 0}}, {5.3, {85, 0, 0}},
          {6.1, {-85, 0, 0}}, {6.9, {0, 60, 0}}, {7.7, {0, -60, 0}}, {8.5, {0, 0, 0}}, {10, {0, 0, 0}}}},
    };
    return list;
}

const Scenario* findScenario(const std::string& name)
{
    for (const Scenario& s : scenarios())
        if (s.name == name) return &s;
    return nullptr;
}

} // namespace imusim
