# Pointer: từ góc đến pixel + screen bound (clamp)

## Lesson summary — 2026-10-31 (Day 3)
- Toạ độ TV ảo: (0,0) ở **góc trên bên trái**. x tăng khi đi sang phải, y tăng khi đi **xuống**. Giữa màn hình 1920×1080 là (960, 540).
- **gain** = số pixel con trỏ đi cho mỗi độ xoay. Trong `imu-sim`, gain mặc định là 40 px/°. Đơn vị: px/° × ° = px.
- `x = 960 − gain × yaw`: dùng dấu `−` vì yaw > 0 nghĩa là mũi remote quay sang **trái**, còn x của màn hình tăng sang **phải**.
- `y = 540 + gain × pitch`: dùng dấu `+` vì pitch > 0 nghĩa là mũi remote chúc **xuống**, và y của màn hình cũng tăng khi đi xuống.
- **clamp** = kẹp một số vào khoảng [thấp nhất, cao nhất]. Số nằm ngoài khoảng thì đặt bằng mép gần nhất. Vùng hợp lệ: x trong [0, 1919], y trong [0, 1079].
- Ba bước cho mỗi sample: cộng dồn góc → đổi góc thành pixel → clamp con trỏ (không clamp góc).
- Code tính lại con trỏ từ góc ở mỗi sample, nên clamp không để lại lỗi về sau. Ví dụ: yaw +30° (con trỏ bị kẹp ở x = 0), rồi yaw về 0° → con trỏ về x = 960.
- Đơn giản hoá có chủ ý: theo hình học, chấm laser trên tường đi một đoạn tính bằng `tan(góc)`. Air mouse dùng quy tắc tuyến tính (mỗi độ = gain pixel). Với góc dưới khoảng 20°, hai cách cho kết quả gần nhau.

| yaw | x = 960 − 40 × yaw | Sau clamp [0, 1919] |
|---|---|---|
| +5° (trái) | 760 | 760 |
| +10° (trái) | 560 | 560 |
| −5° (phải) | 1160 | 1160 |
| +30° (`wild`) | −240 | 0 |

## Pitfalls
- **Lẫn số của hai trục** (lesson 2026-10-31): learner tính `1080 − 1200` thay vì `960 − 1200`. 1080 là chiều cao màn hình. Điểm giữa theo chiều ngang là 960. Khi công thức có nhiều số, hãy hỏi: "số này là gì, của trục nào?"
- **Viết `gain.yaw` thay cho `gain * yaw`.** Trong C++, dấu `.` lấy một phần bên trong struct (xem `cpp/struct-member-access.md`). Phép nhân là `*`.
- **Sai dấu** → con trỏ đi ngược tay. Hãy kiểm tra từng trục: quay trái thì con trỏ phải sang trái, chúc xuống thì con trỏ phải đi xuống.
- **Clamp góc thay vì clamp con trỏ.** Góc vẫn phải là góc thật. Chỉ con trỏ bị kẹp.
- **Clamp bằng 1920 / 1080.** Pixel cuối cùng là 1919 / 1079, vì pixel đầu tiên mang số 0. `imu-sim` đếm off-screen khi `x > 1919` hoặc `y > 1079`.

## Q&A — 2026-10-31 (lesson)
**Hỏi (mentor):** yaw = 10° (quay trái). Tính `p.x` thế nào?
**Learner:** "p.x = 960 * sin/cos/tan của góc." → có một phần đúng: hình học thật có dùng tan. Nhưng air mouse dùng gain tuyến tính. Ngoài ra, 960 là điểm bắt đầu, không phải hệ số nhân: con trỏ lệch khỏi 960 một đoạn bằng gain × góc.

**Hỏi (mentor):** pitch = +3°, gain 40. → learner: "xuống, 540 + 120 = 660, dấu +" (đúng).

**Hỏi (mentor):** wild, yaw = +30°. → learner: "1080 − 1200 = −120, không" (kết luận đúng, số sai; đúng: −240).

**Hỏi (mentor):** y = 1300, clamp? → learner: "[0, 1079], 1079" (đúng).

**Hỏi (mentor):** yaw +30° (bị kẹp), rồi về 0°. p.x cuối? → learner: "960, vì p.x tính lại từ yaw mỗi sample" (đúng).

**Explain-back (4/5):** công thức, dấu, gain và clamp đều đúng. Còn hai điểm cần sửa: viết `gain.yaw` thay cho `gain * yaw`; và nói lý do của dấu `−` là "vì (0,0) ở góc trên trái". Lý do chính xác: x tăng sang phải, còn yaw dương là quay trái.

**Vì sao quan trọng cho bản demo:** con trỏ phải đi đúng chiều tay để chọn được ô menu, và phải luôn nằm trong TV.

**Sau này (Day 6):** có một cách khác là cộng phần thay đổi vào con trỏ rồi mới clamp. Với cách đó, mép màn hình giữ lại một độ lệch, và độ lệch này dùng được để sửa drift yaw.

Tham khảo: `tools/imu-sim/README.md` mục 6 (con trỏ đúng, gain) và mục 9 (cột `off-screen`).
