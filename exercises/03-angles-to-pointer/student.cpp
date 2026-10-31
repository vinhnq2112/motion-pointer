// Bài 03 — từ góc đến con trỏ + screen bound (clamp).
// Làm theo TODO 1 → 6 theo thứ tự. Đọc README.md trước.
// Ghi chú của bạn: learning/knowledge/pointer/angle-to-pixel-and-clamp.md
//                  learning/knowledge/gyro/rate-to-angle-integration.md

#include "student.h"

// TODO 2 — YOU DECIDE: chọn gain (px/°) cho con trỏ. Đọc README.md, mục "YOU DECIDE", rồi viết lý do vào README.
//   Số 0 dưới đây chỉ để code build được. Gain = 0 thì con trỏ không bao giờ di chuyển.
const float kGain = 0;

Angles angle; // góc giữ giữa các lần gọi onImuSample (biến toàn cục, bạn đã chọn ở bài 02)

// TODO 3: Viết lại thân hàm integrateGyro.
//   Copy 2 dòng bạn đã viết trong exercises/02-gyro-integration/student.cpp. Đây là code của chính bạn.
//   Test: integrate.
void integrateGyro(Angles& a, const ImuSample& s)
{
}

// TODO 4: Viết clampf: kẹp v vào khoảng [lo, hi].
//   Pseudocode:
//     - nếu v nhỏ hơn mức thấp nhất → trả về mức thấp nhất
//     - nếu v lớn hơn mức cao nhất → trả về mức cao nhất
//     - các trường hợp còn lại → trả về v (không đổi)
//   Viết bằng if. Không dùng std::clamp, std::min, std::max: bài này muốn bạn tự viết clamp.
//   Test: clamp.
float clampf(float v, float lo, float hi)
{
    return v; // TODO 4: dòng này chưa kẹp gì cả
}

// TODO 5: Viết anglesToPointer: đổi a.yaw, a.pitch thành vị trí con trỏ, rồi kẹp vào trong màn hình.
//   Pseudocode:
//     - tính điểm giữa màn hình từ screenW và screenH (KHÔNG viết thẳng 960 / 540: test dùng cả TV 1280 × 720)
//     - x: bắt đầu ở điểm giữa ngang, đi lệch một đoạn gain × yaw.
//          Dấu + hay −? Nhớ lại: yaw > 0 = mũi remote quay trái; x màn hình tăng sang phải.
//     - y: bắt đầu ở điểm giữa dọc, đi lệch một đoạn gain × pitch.
//          Dấu + hay −? Nhớ lại: pitch > 0 = mũi remote chúc xuống; y màn hình tăng khi đi xuống.
//     - kẹp x và y bằng clampf. Mép cao nhất là kích thước − 1 (pixel đầu tiên mang số 0).
//     - trả về một PointerState chứa x và y (cách viết: README, bảng "Những thứ bạn dùng").
//   Tests: center, direction, bound.
PointerState anglesToPointer(const Angles& a, float gain, float screenW, float screenH)
{
    return {0, 0}; // TODO 5: dòng này luôn trả về góc trên bên trái
}

// TODO 6: Nối mọi thứ vào simulator. Mỗi lần simulator gọi onImuSample, hàm này phải:
//     - cộng sample mới vào angle (integrateGyro)
//     - đổi angle thành con trỏ bằng anglesToPointer, với kGain, kScreenW, kScreenH
//     - gán kết quả vào p (p có dấu & → simulator thấy thay đổi)
//     - gọi reportAngles(yaw, pitch, 0) như bài 02 (để biểu đồ góc vẫn chạy)
//   Tests: plugin-sweep, plugin-wild.
void onImuSample(const ImuSample& s, PointerState& p)
{
}

// Stretch goal (không bắt buộc, scaffold S2). Đọc README, mục "Stretch goal".
//   TODO S1: Ghi số đoán của bạn vào README trước khi viết code.
//   TODO S2: moveXByChange trả về x mới: con trỏ đi lệch khỏi x hiện tại theo phần thay đổi dYaw,
//            rồi được kẹp vào trong màn hình.
//   Test: stretch-relative.
float moveXByChange(float x, float dYaw, float gain, float screenW)
{
    return x; // TODO S2
}
