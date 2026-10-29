# Gyro: từ tốc độ xoay đến góc (integration)

## Lesson summary — 2026-10-28 (Day 2)
- Gyro đo **tốc độ xoay** (°/s), **không** đo góc. Remote giữ yên ở yaw 20° → gyro z = 0.
- Mỗi sample cho một mẩu góc nhỏ: `rate × dt`. Ví dụ: 15 °/s × 0.01 s = 0.15°.
- Cộng dồn các mẩu đó (integration) thì được góc: `yaw += s.gyro.z * s.dt;`
- Góc = tổng diện tích dưới đường gyro theo thời gian.
- Biến góc phải giữ giá trị giữa các lần gọi `onImuSample` (mỗi lần chỉ cộng thêm một mẩu).
- `dt = 1 ÷ sampleRate`: 100 Hz → 0.01 s · 200 Hz → 0.005 s · 50 Hz → 0.02 s.

Số đo trong `imu-sim`, scenario `yaw-sweep` (headless, CSV):

| t | yaw thật | gyro z |
|---|---|---|
| 2.0 s (đang quay) | 10° | 15 °/s (đỉnh) |
| 3.5 s (đứng yên bên trái) | 20° | 0 °/s |

## Pitfalls
- **Lấy tốc độ làm góc** (quiz 2026-10-28): gyro z = 20 °/s trong 0.5 s → trả lời "20°". Đúng: 20 × 0.5 = 10°.
- **Quên `dt`**: `yaw += s.gyro.z;` → góc to gấp 1 ÷ dt = 100 lần. `yaw-sweep` 1 s→3 s: báo 2000° thay vì 20°.
- **Lấy giá trị đỉnh làm trung bình**: gyro tăng 0 → 15 → 0 trong 2 s. Trung bình ≈ 10 °/s, không phải 15.
- **Viết thẳng `0.01f` thay cho `s.dt`**: với `--rate 200`, góc to gấp 2 lần (0.01 ÷ 0.005).
- **Nhầm dt khi đổi sampleRate**: 200 Hz → dt = 1 ÷ 200 = 0.005 s (không phải 0.02).
- **Hai lý do khác nhau, đừng trộn:** nhân `dt` vì °/s × s = ° (đổi đơn vị). Dùng `s.dt` (không phải số cố định)
  vì sampleRate có thể đổi.

## Q&A — 2026-10-28
**Hỏi (mentor):** Remote xoay trái 20° rồi giữ yên. Gyro z đọc bao nhiêu?
**Learner:** "0, vì trục z đứng yên." → số đúng, lý do sai. Trục xoay luôn đứng yên, kể cả lúc đang quay.
Lý do đúng: remote không còn quay → tốc độ xoay = 0.

**Hỏi (learner):** `s` trong `s.gyro.z * s.dt` là gì? → xem `cpp/struct-member-access.md`.

## Q&A — 2026-10-29 (exercise 02, xem bằng mắt)
**Hỏi (learner):** Mỗi lần nhấn R, sai số cộng thêm đúng bằng góc thật trước đó. Vì sao dải mờ (góc thật) và
nét mảnh (code của tôi) không trùng nhau nữa?

**Trả lời:** phím R **dịch chuyển** remote về 0 ngay lập tức, remote **không xoay**. Gyro không đo được cú nhảy
đó (gyro = 0), nên góc của bạn giữ nguyên. Integration chỉ biết góc **thay đổi** bao nhiêu, không bao giờ biết
góc **thật** là bao nhiêu. Một lần lệch thì lệch mãi; các lần lệch cộng dồn.

| Bước | Góc thật | Góc của bạn | Lệch |
|---|---|---|---|
| R lúc thật = 20° | 20 → 0 (nhảy) | 20 | +20 |
| xoay tới −10° | −10 | 10 | +20 |
| R lúc thật = −10° | −10 → 0 (nhảy) | 10 | +10 |

**Hiểu sai đã gặp:**
- "Lúc nhấn R, nếu vẫn giữ phím, gyro > 0 nên góc tăng." Đúng khi giữ phím, nhưng không phải lý do chính.
- Câu kiểm tra (R ở 20°, rồi R ở −10°) → trả lời "0". Đúng: 10°. Sau khi xem bảng: R ở 15°, rồi R ở 5° → 20° (đúng).

**Liên hệ:** remote thật không có phím R, nhưng bias gyro (Day 4) gây lệch nhỏ mỗi giây theo đúng cách này.
Cần nguồn khác để kéo góc về đúng: accelerometer (Day 5), mép màn hình (Day 6).

**Vì sao quan trọng cho bản demo:** `onImuSample` nhận gyro, không nhận góc. Muốn con trỏ chỉ đúng ô menu,
code phải cộng dồn `s.gyro.z * s.dt` (yaw) và `s.gyro.y * s.dt` (pitch) ở mỗi sample.

Tham khảo: `tools/imu-sim/README.md` mục 6 (quy ước dấu) và mục 8 (`--rate`).
