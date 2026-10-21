# Roadmap — Gyro + accelerometer cho remote điều khiển con trỏ

Sprint mode: 7 ngày, mỗi ngày một phase. Ngày 1–6: 1 giờ. Ngày 7: 2 giờ. Tổng 8 giờ.
Exit criteria quan trọng hơn ngày tháng. Nếu một ngày chưa đạt exit, ngày sau bắt đầu bằng phần còn thiếu.

Một ngày điển hình: khởi động 5' → `/lesson` 20' → `/exercise` 30' → "done" 5' (một entry trong
`learning/showcase.md`).

---

## Ngày 0 — Chuẩn bị (mentor, không tính giờ học)
- Mentor dựng `tools/imu-sim/` bằng `/lab`: remote 3D, mũi tên trục, TV ảo + menu, plug-in interface,
  headless mode cho tests.

**Exit:** `imu-sim` build và chạy được trên máy learner; learner xoay được remote bằng chuột.

## Ngày 1 — Trục xoay: yaw, pitch, roll (1 h)
- Foundations (10'): pass by value vs pass by reference (`&`), vì sao `PointerState& p` cần dấu `&`.
- Trục xoay khác hướng chuyển động. Body frame x/y/z của remote. yaw / pitch / roll.
- Quy tắc bàn tay phải: chiều dương của góc xoay.
- Bài tập: sửa lỗi `&` trong một hàm cập nhật; trong simulator, chỉ đúng trục cho 3 chuyển động.

**Exit:** chỉ đúng trục xoay và tên (yaw/pitch/roll) cho 3 chuyển động trong `imu-sim`; test của bài `&` pass;
explain-back "trục xoay vs hướng chuyển động" ≥ 4.

## Ngày 2 — Gyro: từ tốc độ xoay đến góc (1 h)
- Gyro đo angular velocity (tốc độ xoay, °/s) quanh mỗi trục.
- Tích phân đơn giản: `angle += rate * dt`. Vì sao cần `dt`.
- Bài tập: viết hàm cộng dồn yaw và pitch từ dữ liệu gyro (chưa có noise).

**Exit:** tests pass; sai số yaw/pitch < 1° so với ground truth sau scenario 10 giây, noise tắt.

## Ngày 3 — Từ góc đến con trỏ + screen bound (1 h)
- Đổi góc (hoặc thay đổi góc) thành pixel: gain (hệ số khuếch đại), chiều của trục màn hình.
- Screen bound: giữ con trỏ trong màn hình (clamp).
- Bài tập: `onImuSample` lần đầu điều khiển con trỏ trên TV ảo.

**Exit:** tests pass; con trỏ đi đúng chiều khi yaw/pitch; không bao giờ ra ngoài 1920×1080 trong scenario
"vung tay mạnh".

## Ngày 4 — Noise, bias và drift (1 h)
- Noise (nhiễu ngẫu nhiên) vs bias (lỗi lệch cố định). Vì sao bias gây drift khi tích phân.
- Đo drift bằng ground truth. Bias calibration: đo trung bình gyro khi remote nằm yên, rồi trừ đi.
- Bài tập: bật `gyroBias`, đo drift sau 60 s, viết bias calibration.

**Exit:** tests pass; drift sau 60 s giảm ít nhất 10 lần so với khi chưa calibration, có số trước/sau.

## Ngày 5 — Accelerometer + complementary filter (1 h)
- Accelerometer đo trọng lực + gia tốc chuyển động. Khi đứng yên: biết remote nghiêng bao nhiêu (pitch, roll).
- Vì sao accelerometer **không** cho biết yaw (trọng lực song song với trục yaw).
- Complementary filter: gyro cho phần nhanh, accelerometer kéo pitch về đúng trong thời gian dài.
- Bài tập: tính pitch từ accelerometer; ghép complementary filter.

**Exit:** tests pass; pitch không trôi sau 120 s khi có bias; explain-back "vì sao accelerometer không sửa
được yaw" ≥ 4.

## Ngày 6 — Xử lý drift cho con trỏ + ghép bản demo (1 h)
- Dùng screen bound để sửa drift yaw: khi con trỏ chạm mép, phần chuyển động thừa bị bỏ đi, nên con trỏ
  và hướng tay khớp lại.
- Deadzone: bỏ qua chuyển động rất nhỏ (rung tay) để con trỏ đứng yên.
- Ghép tất cả vào `project/`: menu TV, chọn ô.

**Exit:** bản demo chạy trong `imu-sim`: chọn được ô menu; remote nằm yên 30 s thì con trỏ di chuyển < 5 px;
con trỏ luôn trong màn hình.

## Ngày 7 — Checkpoint + bài thuyết trình (2 h)
- `/checkpoint`: kiểm tra mọi exit criteria ở trên bằng bằng chứng.
- `/present`: dựng PowerPoint tiếng Anh từ `learning/showcase.md` (khoảng 25–35 slide cho 60 phút),
  có speaker notes và ảnh/số đo từ `imu-sim`.
- Tập nói một lần với mentor.

**Exit:** checkpoint pass; có file `.pptx`; đã tập toàn bài một lần.

---

## Final goal
Bản demo trong `imu-sim`: code C++ của learner đổi dữ liệu gyro + accelerometer có noise và bias thành con trỏ
trên menu TV. Tiêu chí:
- chọn được ô menu bằng cách xoay remote ảo;
- remote nằm yên 30 s → con trỏ di chuyển < 5 px;
- con trỏ không bao giờ ra ngoài màn hình;
- learner giải thích được (bằng lời của mình, tiếng Anh, trong bài nói 60 phút) drift từ đâu ra và 3 cách xử lý:
  bias calibration, complementary filter, screen bound.
