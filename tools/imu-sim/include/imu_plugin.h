#pragma once
// imu-sim plug-in interface. Keep it stable: exercises and tests are written against it.
// Axis convention (remote body frame): x = along the remote, toward the TV · y = left · z = up.
// Right-hand rule: yaw = rotation about z, pitch = about y, roll = about x.

struct Vec3 { float x, y, z; };
struct ImuSample { float dt; Vec3 gyro; Vec3 accel; }; // dt: s · gyro: °/s · accel: m/s²
struct PointerState { float x, y; };                   // px on the virtual TV, (0,0) = top-left

// Called once per sample (100 times per second by default). Your code updates p.
void onImuSample(const ImuSample& s, PointerState& p);
