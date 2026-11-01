# Triệu chứng của remote thật (dự án của learner)

## Summary
- Remote thật của learner có 3 triệu chứng: con trỏ không mượt; quay mạnh rồi dừng thì con trỏ bật ngược lại
  (quay càng mạnh, bật càng xa); chuyển động chậm thì con trỏ không đi.
- Chưa có số đo. Danh sách dưới đây là **giả thuyết**. Mỗi giả thuyết cần được kiểm tra bằng log dữ liệu thật
  hoặc bằng `imu-sim`.

## Q&A
### Con trỏ không mượt, bật ngược khi dừng gấp, cần quay mạnh mới đi — khoá học có giúp không? — 2026-11-01
Khoá học hiện tại chỉ dạy drift. Ba triệu chứng này đến từ các chỗ khác. Giả thuyết theo từng triệu chứng:

**Bật ngược sau khi quay mạnh (càng mạnh càng xa):**
1. Bias calibration tự động chạy cả khi remote đang chuyển động: phần chuyển động bị coi là bias. Khi remote
   dừng, code trừ "bias" sai đó, nên con trỏ đi lùi. Bộ lọc high-pass cũng gây ra hiện tượng giống vậy.
2. Tràn số (integer overflow) khi gửi bước di chuyển: báo cáo chuột HID thường chứa dx kiểu `int8_t`
   (−128 … 127). Ví dụ dx = 150 → thành −106 → con trỏ nhảy ngược.
3. Accelerometer đo cả gia tốc lúc dừng gấp, không chỉ trọng lực. Complementary filter tưởng remote bị nghiêng.
4. Tay thật bật lại vài độ khi dừng gấp. Trường hợp này gyro đo đúng chuyển động thật.

**Cần quay mạnh mới đi:**
1. Deadzone quá lớn.
2. Mất phần lẻ pixel: 0.4 px mỗi sample, đổi sang `int` thành 0. Chuyển động chậm thì con trỏ không bao giờ đi.
3. Bias calibration tự động "ăn" mất chuyển động chậm (giống giả thuyết 1 ở trên).

**Không mượt, không theo tay:**
1. `dt` cố định nhưng gói Bluetooth đến không đều.
2. Gyro bị bão hoà (saturation): quay nhanh hơn giới hạn đo (ví dụ ±250 °/s) thì số bị cắt, nên con trỏ đi thiếu.
3. Rung tay + noise, nhưng bộ lọc mạnh quá lại gây trễ (lag).
4. Gain cố định: không chính xác được khi quay chậm mà vẫn đi xa được khi quay nhanh (cần pointer acceleration).
- **Applies to:** remote thật của learner; bản demo trong `imu-sim`.
- **See also:** [[angle-to-pixel-and-clamp]] · [[rate-to-angle-integration]]

## Pitfalls

## Know-how
- Muốn tìm nguyên nhân: ghi log dữ liệu thô (thời gian, gyro x/y/z, accel x/y/z, dx/dy gửi đi), rồi so từng cột
  ngay lúc con trỏ bật ngược.
