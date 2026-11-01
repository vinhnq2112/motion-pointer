# Progress

- **Started:** 2026-10-21
- **Course mode:** sprint
- **Current phase:** Day 4 — Noise, bias, drift
- **Current topic:** Day 3 xong 2026-11-01 (exercise 03 pass 7/7 + stretch, 0 sample ngoài màn hình). Tiếp: Day 4
- **Next step:** Day 4 (2/11): khởi động (con trỏ có cả x và y) → `/quiz` (độ lệch không tự sửa) → `/lesson` noise vs bias → `/exercise` 04
- **Week of:** 2026-11-02
- **Hours this week:** 4.0 / ~19 (cả khoá, sprint; buổi 22/10 chưa ghi giờ) · Day 0: 0.5 h, không tính

## This week
### Course plan (sprint, lập ngày 2026-10-21, lập lại 2026-10-27 và 2026-11-01)
Day 1 → Day 3 đã xong (22/10 → 1/11). Lịch cũ ở commit `8505ab5`.
Lập lại 2026-11-01 (lần 2, cùng ngày): learner muốn khoá học giúp giải quyết 3 triệu chứng của remote thật
(không mượt; quay mạnh rồi dừng thì bật ngược; quay chậm thì không đi). Thêm 4 buổi mới (Day 6, 7, 8, 9) và 1 buổi điều tra (Day 11).
Không cắt nội dung cũ. Chỉ học bằng lý thuyết + `imu-sim` (learner không chia sẻ được log thật). Chi tiết: `subject/roadmap.md`.
Mỗi buổi 1.5 h: khởi động + `/quiz` 10' · `/lesson` 30' · `/exercise` 45' · kết thúc 5' (một entry trong `learning/showcase.md`).
Ngày học: mỗi ngày từ T2 2/11 → T4 11/11 (10 buổi, 15 h). Learner xác nhận ngày 2026-11-01.
Mentor thêm knob và số đo vào `imu-sim` (`/lab`) trước buổi cần dùng, ngoài giờ học.

| Buổi | Ngày | Thời lượng | Hoạt động | Xong khi |
|---|---|---|---|---|
| Day 4 | T2 2/11 | 1.5 h | `/quiz` (con trỏ có cả x và y; độ lệch không tự sửa) · `/lesson` noise vs bias, drift, bẫy calibration khi đang quay · `/exercise` 04 | Drift 60 s giảm ≥ 10 lần, có số trước/sau; bật ngược < 5 px |
| Day 5 | T3 3/11 | 1.5 h | `/quiz` drift · `/lesson` accelerometer → pitch/roll, không có yaw, complementary filter, accel khi dừng gấp · `/exercise` 05 | Pitch không trôi sau 120 s; sai số pitch lúc dừng gấp có số; explain-back ≥ 4 |
| Day 6 | T4 4/11 | 1.5 h | `/lesson` bước di chuyển, phần lẻ pixel, tràn `int8_t` · `/exercise` 06 | Quay 2 °/s trong 10 s → sai số < 1 px; `wild` 0 lần nhảy ngược |
| Day 7 | T5 5/11 | 1.5 h | `/lesson` `dt` thật, gói đến không đều, gyro bão hoà · `/exercise` 07 | `packetJitter` → sai số yaw < 1° sau 10 s; đoán + đo góc thiếu khi bão hoà |
| Day 8 | T6 6/11 | 1.5 h | `/lesson` low-pass filter, độ mượt vs độ trễ · `/exercise` 08 + bảng 3 hệ số | Độ rung giảm ≥ 3 lần; độ trễ < 50 ms |
| Day 9 | T7 7/11 | 1.5 h | `/lesson` pointer acceleration, deadzone cứng vs mềm · `/exercise` 09 | Nằm yên 30 s → < 5 px; quay 2 °/s vẫn đi; hết màn hình với ≤ 45° |
| Day 10 | CN 8/11 | 1.5 h | `/lesson` screen bound sửa drift yaw · ghép bản demo trong `project/`, chọn ô menu | Demo chọn được ô; nằm yên 30 s → < 5 px; luôn trong màn hình |
| Day 11 | T2 9/11 | 1.5 h | Điều tra "remote bí ẩn" (3 lỗi giấu) bằng số đo · viết checklist cho remote thật | 3/3 nguyên nhân đúng, có số đo; có file checklist |
| Day 12 | T3 10/11 | 1.5 h | `/checkpoint` 30' · `/present` dựng slide tiếng Anh 55' | Checkpoint pass; có file `.pptx` |
| Day 13 | T4 11/11 | 1.5 h | Tập nói toàn bài với mentor 60' · sửa slide 25' | Đã tập nói toàn bài một lần |

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
- 02-gyro-integration (S3) — passed 2026-10-29 (6/6 test, sai số yaw/pitch 0.000°; YOU DECIDE: global, lý do có so sánh)
- 03-angles-to-pointer (S3) — passed 2026-11-01 (7/7 test, sai số 0.01 px, `wild` 0 sample ngoài màn hình; YOU DECIDE: gain 40, lý do có số; spatial check 3/3; stretch S2 pass: 1200 px, lệch 240 px)

## Scaffold levels
Starter-material level per topic (`/exercise` → "Scaffold levels"). New topics start at S3.
| Topic | Level | Last exercise | Last change |
|---|---|---|---|
| rotation: trục xoay + dấu | S2 | 01-axes-and-reference — passed, explain-back 4, không cần L4 | ↓ 2026-10-27 |
| gyro: tích phân góc | S2 | 02-gyro-integration — passed, explain-back 4, không cần L4 | ↓ 2026-10-29 |
| pointer: góc → pixel + clamp | S2 | 03-angles-to-pointer — passed, explain-back 4, không cần L4 | ↓ 2026-11-01 |

## Milestone status
| Phase | Status | Exit criteria met |
|---|---|---|
| Day 0 Chuẩn bị (imu-sim) | done 2026-10-21 | build + tests OK; learner chạy được và xoay được bằng chuột |
| Day 1 Trục xoay: yaw, pitch, roll | done 2026-10-27 | 3/3 trục + tên đúng; test `&` pass; explain-back 4 |
| Day 2 Gyro: tốc độ xoay → góc | done 2026-10-29 | 6/6 test; sai số yaw/pitch 0.000° < 1° sau 10 s, noise tắt |
| Day 3 Góc → con trỏ + screen bound | done 2026-11-01 | 7/7 test; con trỏ đúng chiều (sai số 0.01 px, spatial check 3/3); `wild` 0 sample ngoài màn hình |
| Day 4 Noise, bias, drift (+ bẫy calibration khi đang quay) | not started | – |
| Day 5 Accelerometer + complementary filter (+ dừng gấp) | not started | – |
| Day 6 Bước di chuyển: phần lẻ pixel, tràn số | not started | – |
| Day 7 Thời gian thật + gyro bão hoà | not started | – |
| Day 8 Low-pass filter + độ trễ | not started | – |
| Day 9 Pointer acceleration + deadzone | not started | – |
| Day 10 Xử lý drift + ghép demo | not started | – |
| Day 11 Điều tra remote bí ẩn | not started | – |
| Day 12 Checkpoint + slide | not started | – |
| Day 13 Tập nói | not started | – |
