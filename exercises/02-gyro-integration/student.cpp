// Bài 02 — cộng dồn gyro thành góc (integration).
// Làm theo TODO 1 → 4 theo thứ tự. Đọc README.md trước.
// Ghi chú của bạn: learning/knowledge/gyro/rate-to-angle-integration.md
//                  learning/knowledge/cpp/struct-member-access.md

#include "student.h"
Angles angle; // TODO 4: biến toàn cục (global) để giữ góc giữa các lần gọi onImuSample. Xem README.md, mục "YOU DECIDE".

// Hàm chính của bài. Simulator (và tests) gọi hàm này một lần cho mỗi sample.
//   a : góc hiện tại. Có dấu & → hàm sửa chính biến của người gọi.
//   s : một sample. s.dt (giây), s.gyro.x / .y / .z (°/s), s.accel (bài này chưa dùng).
//
// TODO 2: Cập nhật a.yaw.
//   Pseudocode:
//     - yaw xoay quanh trục nào? → chọn đúng thành phần của s.gyro (.x, .y hay .z)
//     - mẩu góc nhỏ của sample này = tốc độ xoay × thời gian của sample
//     - cộng mẩu góc đó vào góc cũ (dùng +=)
//   Chạy lại tests: test oneSample-yaw và yawSweep phải pass.
//
// TODO 3: Cập nhật a.pitch, giống TODO 2 nhưng cho pitch.
//   Pseudocode:
//     - pitch xoay quanh trục nào? → chọn thành phần của s.gyro
//     - các bước còn lại giống TODO 2
//   Chạy lại tests: oneSample-pitch, pitchSweep, sampleRate phải pass.
//   Nếu sampleRate fail mà yawSweep pass: bạn có viết thẳng số 0.01 không?
void integrateGyro(Angles& a, const ImuSample& s)
{
    a.yaw += s.gyro.z * s.dt;   // TODO 2: sửa trục gyro đúng cho yaw
    a.pitch += s.gyro.y * s.dt; // TODO 3: sửa trục gyro đúng cho pitch
}

// TODO 4: Nối hàm của bạn vào simulator.
//   Simulator gọi onImuSample 100 lần mỗi giây. Mỗi lần, hàm này phải:
//     - lấy góc đã tính ở lần gọi TRƯỚC (góc phải còn giữ nguyên giữa hai lần gọi)
//     - gọi integrateGyro để cộng thêm sample mới
//     - gọi reportAngles(yaw, pitch, 0) để simulator vẽ góc của bạn lên biểu đồ (roll = 0 vì bài này chưa tính)
//   KHÔNG sửa p trong bài này. Con trỏ là bài Ngày 3.
//
//   YOU DECIDE: biến Angles cần sống qua nhiều lần gọi onImuSample. Bạn để biến đó ở đâu?
//     (a) biến `static` bên trong onImuSample, hay (b) biến toàn cục (global) bên ngoài hàm, ngay trên onImuSample.
//     Cả hai đều chạy được. Đọc README, mục "YOU DECIDE", rồi viết lựa chọn + lý do có so sánh vào README.
//
//   Câu hỏi để tự kiểm tra: nếu bạn khai báo `Angles a;` bình thường bên trong hàm (không static),
//   thì ở mỗi lần gọi, a.yaw bắt đầu bằng bao nhiêu?
//   Test kiểm tra phần này: plugin.
void onImuSample(const ImuSample& s, PointerState& p)
{
    integrateGyro(angle, s); // TODO 4: cộng sample mới vào góc đã tính ở lần gọi trước
    reportAngles(angle.yaw, angle.pitch, 0); // TODO 4: báo simulator vẽ góc của bạn (roll = 0 vì bài này chưa tính)
}
