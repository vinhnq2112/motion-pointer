#pragma once
// Bài 03 — mentor viết file này. Bạn KHÔNG cần sửa file này.
// Vec3, ImuSample, PointerState, onImuSample, reportAngles: xem tools/imu-sim/include/imu_plugin.h.

#include "imu_plugin.h"

// Kích thước TV ảo mặc định, đơn vị pixel. onImuSample không nhận kích thước màn hình, nên dùng hai số này.
constexpr float kScreenW = 1920;
constexpr float kScreenH = 1080;

// Gain bạn chọn cho con trỏ, đơn vị px/°. Bạn định nghĩa giá trị trong student.cpp (YOU DECIDE).
// Tests đọc số này để chấm con trỏ của bạn với con trỏ đúng cùng gain.
extern const float kGain;

// Góc do code của bạn tính, đơn vị độ (°). Giống bài 02.
struct Angles {
    float yaw = 0;   // xoay quanh z
    float pitch = 0; // xoay quanh y
};

// Cộng thêm phần góc của MỘT sample vào a. Giống bài 02.
void integrateGyro(Angles& a, const ImuSample& s);

// Kẹp v vào khoảng [lo, hi]: v < lo → lo · v > hi → hi · còn lại → v.
float clampf(float v, float lo, float hi);

// Đổi góc thành vị trí con trỏ trên màn hình screenW × screenH, rồi kẹp vào trong màn hình.
PointerState anglesToPointer(const Angles& a, float gain, float screenW, float screenH);

// Stretch goal (không bắt buộc): con trỏ đi theo PHẦN THAY ĐỔI của yaw.
// x: vị trí hiện tại (px) · dYaw: yaw thay đổi bao nhiêu trong sample này (°) → x mới, đã kẹp.
float moveXByChange(float x, float dYaw, float gain, float screenW);
