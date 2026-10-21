# Review checklist — Gyro + accelerometer cho remote điều khiển con trỏ

Used by `/review` and the `reviewer` agent.

## Severity levels
- 🔴 **Wrong** — sai kết quả, crash, undefined behavior, con trỏ ra ngoài màn hình
- 🟠 **Structure** — chạy được nhưng khó đọc / khó sửa, trộn nhiều việc trong một hàm
- 🟡 **Quality** — độ chính xác, drift còn lớn, con trỏ rung, chọn tham số không có lý do
- 🔵 **Style** — đặt tên, quy ước, gọn gàng

## What to check, in priority order
1. **Đơn vị và dấu:** °/s vs rad/s, giây vs mili-giây, có nhân `dt` không, chiều dương của trục đúng
   axis convention trong `subject/subject.md`, chiều trục màn hình (y pixel tăng xuống dưới).
2. **Pass by value vs reference:** `PointerState& p` và các trạng thái khác được sửa đúng biến; không sửa nhầm bản sao.
3. **Trạng thái giữa các lần gọi:** góc, bias, bộ lọc được giữ đúng giữa các lần gọi `onImuSample`; khởi tạo đúng.
4. **Drift:** bias calibration, complementary filter, screen bound được dùng đúng chỗ; có số đo trước/sau.
5. **Screen bound:** con trỏ luôn trong `[0, width) × [0, height)`.
6. **Tham số:** gain, ngưỡng deadzone, hệ số filter có tên rõ và có lý do (`YOU DECIDE`).
7. **Kiểu số:** `float` vs `int` (mất phần thập phân khi cộng dồn), chia cho 0 khi `dt = 0`.

## Automatic checks to run first
1. `cmake --build <build-dir> --config Debug` — đọc warnings (MSVC `/W4` bật trong CMakeLists của bài)
2. `ctest --test-dir <build-dir> -C Debug --output-on-failure`
