#pragma once
// Bài 02 — mentor viết file này. Bạn KHÔNG cần sửa file này.
// Vec3, ImuSample, PointerState, onImuSample, reportAngles: xem tools/imu-sim/include/imu_plugin.h.

#include "imu_plugin.h"

// Góc do code của bạn tính, đơn vị độ (°). Bài này chưa cần roll.
struct Angles {
    float yaw = 0;   // xoay quanh z
    float pitch = 0; // xoay quanh y
};

// Cộng thêm phần góc của MỘT sample vào a.
void integrateGyro(Angles& a, const ImuSample& s);
