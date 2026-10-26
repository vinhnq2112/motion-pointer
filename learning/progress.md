# Progress

- **Started:** 2026-10-21
- **Course mode:** sprint
- **Current phase:** Day 2 — Gyro: tốc độ xoay → góc
- **Current topic:** Day 1 xong 2026-10-27 (exercise 01 pass, cả stretch). Tiếp: Day 2
- **Next step:** T4 28/10: `/quiz` trục xoay → `/lesson` gyro, `angle += rate * dt`
- **Week of:** 2026-10-19
- **Hours this week:** 1 / ~9 (cả khoá, sprint; buổi 22/10 chưa ghi giờ) · Day 0: 0.5 h, không tính

## This week
### Course plan (sprint, lập ngày 2026-10-21, lập lại 2026-10-27)
Lập lại 2026-10-27: Day 1 kéo dài sang 27/10, nên chậm 3 buổi. Learner thêm thứ 7 31/10 và chủ nhật 1/11. Không bỏ phần nào.
Học thứ 5 22/10 → chủ nhật 1/11 (7 ngày học). Nghỉ 23/10 → 26/10. Tổng khoảng 9 giờ (thêm 1 giờ vì Day 5 + Day 6 gộp vào một ngày 2 giờ).
Mỗi buổi 1 giờ: khởi động 5' · `/lesson` 20' · `/exercise` 30' · kết thúc 5' (một entry trong `learning/showcase.md`).

| Buổi | Ngày | Thời lượng | Hoạt động | Xong khi |
|---|---|---|---|---|
| Day 1 | T5 22/10 + T3 27/10 | 1 h + 1 h | Khởi động: hỏi lại `&` (weak spot) · `/lesson` trục xoay vs hướng chuyển động, body frame, yaw/pitch/roll, quy tắc bàn tay phải (xem trong `imu-sim`) · `/exercise` sửa lỗi `&` + chỉ trục cho 3 chuyển động (phím A ẩn đáp án) | Chỉ đúng trục + tên cho 3 chuyển động; test `&` pass; explain-back "trục xoay vs hướng chuyển động" ≥ 4 |
| Day 2 | T4 28/10 | 1 h | `/quiz` trục xoay · `/lesson` gyro đo °/s, `angle += rate * dt` · `/exercise` cộng dồn yaw/pitch (mentor thêm biểu đồ góc thật vs góc tính vào `imu-sim`) | Tests pass; sai số yaw/pitch < 1° sau 10 s, noise tắt |
| — | T6 23/10 → T2 26/10 | 0 | Nghỉ (không học) | – |
| Day 3 | T5 29/10 | 1 h | `/quiz` tích phân · `/lesson` góc → pixel, gain, chiều trục màn hình, screen bound (clamp) · `/exercise` `onImuSample` lần đầu điều khiển con trỏ | Tests pass; con trỏ đi đúng chiều; 0 sample ra ngoài màn hình trong scenario `wild` |
| Day 4 | T6 30/10 | 1 h | `/quiz` · `/lesson` noise vs bias, vì sao bias gây drift · `/exercise` bật `gyroBias`, đoán rồi đo drift 60 s, viết bias calibration | Tests pass; drift sau 60 s giảm ≥ 10 lần, có số trước/sau |
| Day 5 | T7 31/10 (giờ đầu) | 1 h | `/quiz` drift · `/lesson` accelerometer đo trọng lực → pitch/roll; vì sao không có yaw; complementary filter · `/exercise` pitch từ accel + filter | Tests pass; pitch không trôi sau 120 s có bias; explain-back "vì sao accel không sửa được yaw" ≥ 4 |
| Day 6 | T7 31/10 (giờ thứ hai) | 1 h | `/quiz` · `/lesson` screen bound sửa drift yaw, deadzone · ghép bản demo trong `project/`, chọn ô menu | Demo: chọn được ô; nằm yên 30 s → con trỏ đi < 5 px; luôn trong màn hình |
| Day 7 | CN 1/11 | 2 h | `/checkpoint` (30') · `/present` dựng slide tiếng Anh (30') · tập nói toàn bài với mentor (60') | Checkpoint pass; có file `.pptx`; đã tập nói một lần |

Nếu một buổi chưa đạt "Xong khi", buổi sau bắt đầu bằng phần còn thiếu. Nếu chậm hơn 1 buổi, mentor lập lại kế hoạch (gộp hoặc bỏ bớt, ghi rõ phần nào).

## Environment
- Windows 11 Home — OK
- Visual Studio Community 2022 17.14 (C++ x64 toolset) — OK
- CMake 4.0.1 — OK
- Git — OK
- Node.js v22.16.0, npm 10.9.2 — OK (cho `/present`)
- Python — chưa cài (không cần)
- `tools/imu-sim` — đã dựng 2026-10-21 (raylib 5.5); selftest 18/18 OK, ctest 2/2 pass; ảnh chụp cửa sổ đã kiểm tra

## Completed
- 01-axes-and-reference (S3) — passed 2026-10-27 (Phần A 4/4 test, Phần B 3/3 trục đúng; stretch bỏ qua)

## Scaffold levels
Starter-material level per topic (`/exercise` → "Scaffold levels"). New topics start at S3.
| Topic | Level | Last exercise | Last change |
|---|---|---|---|
| rotation: trục xoay + dấu | S2 | 01-axes-and-reference — passed, explain-back 4, không cần L4 | ↓ 2026-10-27 |

## Milestone status
| Phase | Status | Exit criteria met |
|---|---|---|
| Day 0 Chuẩn bị (imu-sim) | done 2026-10-21 | build + tests OK; learner chạy được và xoay được bằng chuột |
| Day 1 Trục xoay: yaw, pitch, roll | done 2026-10-27 | 3/3 trục + tên đúng; test `&` pass; explain-back 4 |
| Day 2 Gyro: tốc độ xoay → góc | not started | – |
| Day 3 Góc → con trỏ + screen bound | not started | – |
| Day 4 Noise, bias, drift | not started | – |
| Day 5 Accelerometer + complementary filter | not started | – |
| Day 6 Xử lý drift + ghép demo | not started | – |
| Day 7 Checkpoint + thuyết trình | not started | – |
