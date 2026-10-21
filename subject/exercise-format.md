# Exercise format — Gyro + accelerometer cho remote điều khiển con trỏ

Used by `/exercise` and `/review`. Style A (Programming, tests as checker) + một phần "xem bằng mắt" trong
`imu-sim`.

## Folder layout
`exercises/NN-<topic-slug>/` containing:
- `README.md` — nhiệm vụ, yêu cầu, giới hạn, stretch goal, liên hệ với bản demo con trỏ (mentor viết)
- `CMakeLists.txt` — mentor viết (learner chưa cần học CMake trong khoá này)
- `tests/` — tests và scenario ghi sẵn từ `imu-sim` (mentor viết)
- `student.cpp` (hoặc tên ghi trong README) — learner viết phần cài đặt

## How to check
1. `cmake -S exercises/NN-<slug> -B build/NN-<slug>`
2. `cmake --build build/NN-<slug> --config Debug`
3. `ctest --test-dir build/NN-<slug> -C Debug --output-on-failure`
4. Xem bằng mắt: chạy `imu-sim` với code của bài (lệnh ghi trong README), so con trỏ / góc với ground truth.

Tests in ra số đo (sai số °, sai số px, drift sau N giây), không chỉ pass/fail.

## Exercise types to rotate
- implement-from-tests — viết hàm cho tests pass
- debug lab — code có một lỗi cài sẵn (ví dụ thiếu `&`, quên nhân `dt`, sai dấu trục)
- predict, then measure — đoán con số (ví dụ drift sau 60 s) trước, rồi chạy simulator để đo
- spatial check — chỉ trục xoay / chiều con trỏ cho một chuyển động trong `imu-sim`, trả lời trong README

## Scaffold levels for this subject
- **S3 Guided:** `student.cpp` có sẵn chữ ký hàm, thân hàm rỗng, các bước `// TODO 1..n` (làm gì + làm thế nào)
  bằng tiếng Việt, pseudocode cho bước khó, ít nhất một `// YOU DECIDE:` (ví dụ chọn gain, ngưỡng deadzone,
  thời gian calibration).
- **S2 Outline:** chữ ký hàm + `// TODO` chỉ ghi mục tiêu (làm gì), không có cách làm.
- **S1 Spec:** README ghi yêu cầu + tests; learner tự tạo `student.cpp` theo interface trong README.
- **S0 Blank:** chỉ README; learner tự viết thêm ít nhất một test (ví dụ trường hợp remote nằm yên).
