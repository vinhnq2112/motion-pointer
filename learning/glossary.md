# Glossary

One row per technical term, added the first time the term is used (`CLAUDE.md` writing rule 11).
`/translate` uses this file to translate terms the same way everywhere.

| Term | Tiếng Việt | Meaning (one plain sentence) | Example | First seen |
|---|---|---|---|---|
| IMU (Inertial Measurement Unit) | bộ đo quán tính | Con chip chứa gyro và accelerometer. | Remote TV có một IMU bên trong. | 2026-10-21 |
| gyro (gyroscope) | con quay hồi chuyển | Cảm biến đo tốc độ xoay quanh mỗi trục, đơn vị °/s. | Xoay đều 10 °/s trong 3 s → 30°. | 2026-10-21 |
| accelerometer | cảm biến gia tốc | Cảm biến đo gia tốc, gồm cả trọng lực, đơn vị m/s². | Nằm yên trên bàn: đo được khoảng 9.8 m/s² hướng lên. | 2026-10-21 |
| axis of rotation | trục xoay | Đường thẳng đứng yên trong khi vật quay quanh nó. | Cánh cửa quay quanh bản lề thẳng đứng. | 2026-10-21 |
| yaw | xoay trái/phải | Xoay quanh trục thẳng đứng (trục z của remote). | Lắc đầu "không" → con trỏ đi trái/phải. | 2026-10-21 |
| drift | trôi | Lỗi nhỏ bị cộng dồn theo thời gian, làm góc tính được lệch dần khỏi góc thật. | Gyro báo 0.5 °/s khi nằm yên → sau 60 s lệch 30°. | 2026-10-21 |
| pass by value | truyền bản sao | Hàm nhận một bản sao; sửa bản sao không đổi biến gốc. | `void addOne(Vec3 v)` | 2026-10-21 |
| pass by reference | truyền tham chiếu | Hàm nhận chính biến gốc (dấu `&`); sửa là đổi biến gốc. | `void addTwo(Vec3& v)` | 2026-10-21 |
| screen bound | giới hạn màn hình | Quy tắc giữ con trỏ luôn nằm trong màn hình. | Con trỏ chạm mép phải thì dừng ở x = 1919. | 2026-10-21 |
| starter code | code mẫu ban đầu | Code có sẵn trong bài tập để learner điền tiếp. | File có chữ ký hàm và các `TODO`. | 2026-10-21 |
| pseudocode | code giả | Các bước của thuật toán viết bằng lời, chưa phải code thật. | "cộng tốc độ × dt vào góc" | 2026-10-21 |
