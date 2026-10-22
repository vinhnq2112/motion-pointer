#pragma once
// Bài 01 — mentor viết file này. Bạn KHÔNG cần sửa file này.
// Quy ước trục và dấu: tools/imu-sim/README.md, mục 6.

// Ba góc của remote, đơn vị độ (°).
struct Angles {
    float yaw = 0;   // xoay quanh z
    float pitch = 0; // xoay quanh y
    float roll = 0;  // xoay quanh x
};

void turnLeft(Angles& a, float deg);   // mũi remote quay sang TRÁI deg độ
void tiltDown(Angles& a, float deg);   // mũi remote chúc XUỐNG deg độ
void twistRight(Angles& a, float deg); // vặn cổ tay: cạnh PHẢI của remote đi XUỐNG deg độ
void lookUp(Angles& a, float deg);     // mũi remote ngẩng LÊN deg độ

// Stretch goal: xoay deg độ theo chiều dương quanh trục 'x', 'y' hoặc 'z'.
void rotateAbout(Angles& a, char axis, float deg);
