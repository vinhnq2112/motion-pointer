# Roadmap — Gyro + accelerometer cho remote điều khiển con trỏ

Sprint mode: 13 ngày, mỗi ngày một phase. Ngày 1–3: 1 giờ (thực tế 1.5 h). Ngày 4–13: 1.5 giờ. Tổng khoảng 19 giờ.
(Ban đầu 7 ngày, 8 giờ; mở rộng 2026-11-01, xem ghi chú ở Ngày 4.)
Exit criteria quan trọng hơn ngày tháng. Nếu một ngày chưa đạt exit, ngày sau bắt đầu bằng phần còn thiếu.

Một ngày điển hình (1.5 h): khởi động + `/quiz` 10' → `/lesson` 30' → `/exercise` 45' → "done" 5' (một entry trong
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

> **Mở rộng 2026-11-01.** Từ Day 4, mỗi buổi 1.5 h. Learner thêm 4 buổi để giải quyết 3 triệu chứng của remote
> thật: con trỏ không mượt; quay mạnh rồi dừng thì con trỏ bật ngược; quay chậm thì con trỏ không đi.
> Chỉ học bằng lý thuyết + `imu-sim` (learner không chia sẻ được log thật). Giả thuyết: `learning/knowledge/pointer/real-remote-symptoms.md`.
> Mentor thêm vào `imu-sim` (`/lab`, ngoài giờ học) các knob và số đo cần cho từng buổi, ghi ở dòng "Lab" của buổi đó.

## Ngày 4 — Noise, bias và drift (1.5 h)
- Noise (nhiễu ngẫu nhiên) vs bias (lỗi lệch cố định). Vì sao bias gây drift khi tích phân.
- Đo drift bằng ground truth. Bias calibration: đo trung bình gyro khi remote nằm yên, rồi trừ đi.
- **Thêm:** bẫy của bias calibration tự động: nếu code đo "bias" cả khi remote đang quay, con trỏ bật ngược khi dừng.
- Bài tập: bật `gyroBias`, đo drift sau 60 s, viết bias calibration chỉ chạy khi remote nằm yên.
- Lab: số đo "bật ngược" (px con trỏ đi lùi sau khi dừng gấp).

**Exit:** tests pass; drift sau 60 s giảm ít nhất 10 lần so với khi chưa calibration, có số trước/sau;
scenario "quay mạnh rồi dừng": bật ngược < 5 px.

## Ngày 5 — Accelerometer + complementary filter (1.5 h)
- Accelerometer đo trọng lực + gia tốc chuyển động. Khi đứng yên: biết remote nghiêng bao nhiêu (pitch, roll).
- Vì sao accelerometer **không** cho biết yaw (trọng lực song song với trục yaw).
- Complementary filter: gyro cho phần nhanh, accelerometer kéo pitch về đúng trong thời gian dài.
- **Thêm:** khi dừng gấp, accelerometer đo cả lực dừng → pitch tính từ accel sai trong thời gian ngắn.
- Bài tập: tính pitch từ accelerometer; ghép complementary filter.
- Lab: scenario dừng gấp có gia tốc tuyến tính trong dữ liệu accel.

**Exit:** tests pass; pitch không trôi sau 120 s khi có bias; đo được sai số pitch lúc dừng gấp (có số);
explain-back "vì sao accelerometer không sửa được yaw" ≥ 4.

## Ngày 6 — Bước di chuyển: phần lẻ pixel và tràn số (1.5 h) — mới
- Remote thật thường gửi **bước di chuyển** (`dx`, `dy`) mỗi lần, không gửi vị trí. Đổi góc → bước.
- Mất phần lẻ: 0.4 px → `int` → 0. Cách giữ phần lẻ cho lần sau.
- Tràn số `int8_t` (−128 … 127) trong báo cáo chuột HID: vì sao quay mạnh làm con trỏ nhảy ngược.
- Bài tập: `onImuSample` gửi bước kiểu `int8_t`, giữ phần lẻ, không tràn.
- Lab: chế độ đầu ra "bước `int8_t`"; scenario quay rất chậm.

**Exit:** tests pass; quay chậm 2 °/s trong 10 s → sai số con trỏ < 1 px; scenario `wild` → 0 lần nhảy ngược.

## Ngày 7 — Thời gian thật và gyro bão hoà (1.5 h) — mới
- Gói Bluetooth đến không đều: `dt` thật khác `dt` cố định. Lấy `dt` từ thời gian thật.
- Gyro bão hoà (saturation): quay nhanh hơn giới hạn đo (ví dụ ±250 °/s) thì số bị cắt → con trỏ đi thiếu.
- Bài tập: đoán rồi đo góc bị thiếu khi bão hoà; sửa code dùng `dt` thật.
- Lab: knob `packetJitter`, `gyroRange`.

**Exit:** tests pass; `packetJitter` bật → sai số yaw < 1° sau 10 s; đoán và đo góc thiếu khi bão hoà (lệch < 20%).

## Ngày 8 — Làm mượt: low-pass filter và độ trễ (1.5 h) — mới
- Rung tay + noise làm con trỏ không mượt. Low-pass filter giữ thay đổi chậm, bỏ rung nhanh.
- Lọc mạnh hơn → mượt hơn nhưng trễ hơn. Đo cả hai bằng số.
- Bài tập: viết low-pass filter; bảng so sánh 3 hệ số (độ rung px, độ trễ ms); YOU DECIDE hệ số.
- Lab: số đo "độ rung" (px) và "độ trễ" (ms); knob `handTremor` + scenario tay bật lại khi dừng.

**Exit:** tests pass; giữ yên với `handTremor` bật → độ rung giảm ≥ 3 lần; độ trễ < 50 ms; có bảng 3 hệ số.

## Ngày 9 — Pointer acceleration + deadzone (1.5 h) — mới
- Gain cố định không thể vừa chính xác khi quay chậm vừa đi xa khi quay nhanh. Pointer acceleration: gain thay
  đổi theo tốc độ quay.
- Deadzone: bỏ qua chuyển động rất nhỏ. Deadzone "cứng" làm quay chậm không đi; deadzone "mềm" thì không.
- Bài tập: đường cong gain theo tốc độ + deadzone mềm.
- Lab: scenario "chọn ô nhỏ" (cần chính xác) + "đi qua cả màn hình" (cần nhanh).

**Exit:** tests pass; nằm yên với rung tay 30 s → con trỏ đi < 5 px; quay chậm 2 °/s vẫn đi; đi hết màn hình
với ≤ 45° quay.

## Ngày 10 — Xử lý drift cho con trỏ + ghép bản demo (1.5 h)
- Dùng screen bound để sửa drift yaw: khi con trỏ chạm mép, phần chuyển động thừa bị bỏ đi, nên con trỏ
  và hướng tay khớp lại.
- **Thêm:** triệu chứng thật "quay mạnh sang trái → con trỏ chạm mép trái → bật ngược sang phải". Ở mép, clamp
  giữ con trỏ đứng yên, nên mọi chuyển động nhỏ sau đó đều lộ ra. So sánh vị trí tuyệt đối vs bước di chuyển ở mép.
- Ghép tất cả vào `project/`: menu TV, chọn ô.
- Lab: scenario `edge-slam` (quay mạnh sang trái chạm mép rồi dừng gấp, có tay bật lại) + số đo bật ngược ở mép.

**Exit:** bản demo chạy trong `imu-sim`: chọn được ô menu; remote nằm yên 30 s thì con trỏ di chuyển < 5 px;
con trỏ luôn trong màn hình; `edge-slam` → bật ngược < 5 px.

## Ngày 11 — Điều tra: "remote bí ẩn" (1.5 h) — mới
- Mentor giấu 3 lỗi trong một plug-in "remote bí ẩn" để tạo lại 3 triệu chứng. Learner không đọc code đó.
- Learner chỉ dùng số đo của `imu-sim` để tìm nguyên nhân từng lỗi.
- Kết quả: một **checklist** cho remote thật: cần log cột nào, nhìn số nào, mỗi triệu chứng thử gì trước.

**Exit:** tìm đúng nguyên nhân 3/3 lỗi, mỗi lỗi có số đo làm bằng chứng; có file checklist.

## Ngày 12 — Checkpoint + slide (1.5 h)
- `/checkpoint`: kiểm tra mọi exit criteria ở trên bằng bằng chứng.
- `/present`: dựng PowerPoint tiếng Anh từ `learning/showcase.md` (khoảng 25–35 slide cho 60 phút),
  có speaker notes và ảnh/số đo từ `imu-sim`.

**Exit:** checkpoint pass; có file `.pptx`.

## Ngày 13 — Tập nói (1.5 h)
- Tập nói toàn bài một lần với mentor (60'), sửa slide theo góp ý.

**Exit:** đã tập toàn bài một lần; slide đã sửa.

---

## Final goal
Bản demo trong `imu-sim`: code C++ của learner đổi dữ liệu gyro + accelerometer có noise và bias thành con trỏ
trên menu TV. Tiêu chí:
- chọn được ô menu bằng cách xoay remote ảo;
- remote nằm yên 30 s → con trỏ di chuyển < 5 px;
- con trỏ không bao giờ ra ngoài màn hình;
- quay mạnh rồi dừng → con trỏ bật ngược < 5 px; quay chậm 2 °/s → con trỏ vẫn đi;
- learner giải thích được (bằng lời của mình, tiếng Anh, trong bài nói 60 phút) drift từ đâu ra và 3 cách xử lý:
  bias calibration, complementary filter, screen bound; và 3 triệu chứng của remote thật cùng cách tìm nguyên nhân.
