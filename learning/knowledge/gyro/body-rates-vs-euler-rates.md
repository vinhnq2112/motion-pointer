# Gyro: vì sao `yaw += gyro.z × dt` chỉ gần đúng (body rates vs Euler rates)

## Q&A — 2026-11-01 (sau exercise 03)
**Hỏi (learner):** Chấm vàng là vị trí remote, chấm trắng là con trỏ thật, đúng không? Yaw thật và yaw tính khác nhau
thế nào? Vì sao trong `wild` chấm trắng lệch khỏi vòng vàng: do code cộng dồn hay do bản chất gyro? Cách làm đúng là gì?

**Trả lời:**
- **Vòng vàng** = chỗ remote **thật sự chỉ vào** trên TV (ground truth, simulator tính từ góc thật). Vòng vàng không phải vị trí của remote.
- **Chấm trắng** = con trỏ TV hiển thị, tức là chỗ code **nghĩ** remote đang chỉ (tính từ gyro).
- **Yaw thật:** simulator biết chắc góc này. Code không đọc được nó. Yaw thật chỉ dùng để chấm điểm.
- **Yaw tính:** code cộng dồn từ gyro. Remote thật chỉ có yaw tính.
- **Lý do lệch (noise = 0, bias = 0):** gyro đo đúng. Lỗi nằm ở **công thức đơn giản hoá** trong code.
  `gyro.z` đo tốc độ xoay quanh trục z **của remote**, còn yaw là xoay quanh trục **thẳng đứng của phòng**.
  Hai trục này trùng nhau chỉ khi pitch = roll = 0. Integration cộng dồn sai số và không tự sửa.
- **Ví dụ:** roll 30°, `gyro.z = 10` °/s → tốc độ yaw thật = 10 × cos 30° = 8.66 °/s, tốc độ pitch thật = −10 × sin 30° = −5 °/s.
  Code đơn giản cộng 10 vào yaw và 0 vào pitch.

**Cách đúng hơn:** đổi body rates (tốc độ theo trục remote) thành Euler rates (tốc độ thay đổi của từng góc),
rồi mới cộng dồn:
```
tốc độ yaw   = (gyro.y × sin(roll) + gyro.z × cos(roll)) ÷ cos(pitch)
tốc độ pitch =  gyro.y × cos(roll) − gyro.z × sin(roll)
tốc độ roll  =  gyro.x + (gyro.y × sin(roll) + gyro.z × cos(roll)) × tan(pitch)
```
Công thức không dùng được khi pitch gần ±90° (chia cho cos ≈ 0). Sản phẩm thật dùng quaternion (ngoài phạm vi khoá học).

**Số đo** (`imu-sim-headless --gain 40`, noise tắt, mentor đo 2026-11-01), cột `angle err` lúc cuối:

| Scenario | `yaw += gyro.z × dt` | Đổi sang Euler rates |
|---|---|---|
| `figure` | 1.94° | 0.01° |
| `wild` | 21.75° (con trỏ cuối: 1830, 332 thay vì 960, 540) | 2.32° |

**Quyết định cho khoá học:** giữ công thức đơn giản (scope: Euler đơn giản). Bản demo kéo chuột chủ yếu yaw/pitch.
Sai số còn lại được Day 5 (accelerometer → pitch/roll) và Day 6 (screen bound → yaw) kéo về. Trên slide: ghi công thức
Euler rates và quaternion là "next step".

## Pitfalls
- Nghĩ "gyro sai" khi thấy lệch. Ở đây gyro hoàn hảo; sai là do mô hình trong code.
- Gọi vòng vàng là "vị trí remote". Vòng vàng là **chỗ remote chỉ vào** trên TV.
