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
| body frame | hệ trục gắn với remote | Ba trục x, y, z gắn chặt vào remote và xoay cùng remote. | x luôn dọc thân remote, kể cả khi remote nghiêng. | 2026-10-21 |
| ground truth | giá trị đúng (đáp án thật) | Giá trị thật mà simulator biết chắc, dùng để chấm code của bạn. | Góc yaw thật = 10°; code của bạn tính 9.6° → sai 0.4°. | 2026-10-21 |
| scenario | kịch bản chuyển động | Một chuyển động ghi sẵn, chạy giống nhau mỗi lần. | `yaw-sweep`: trái 20°, phải 20°, về giữa. | 2026-10-21 |
| plug-in interface | chỗ cắm code | Hàm nhỏ, cố định, nơi code của bạn nối vào simulator. | `onImuSample(s, p)` | 2026-10-21 |
| knob | núm chỉnh | Một thiết lập của simulator, bật dần theo bài học. | `gyroBias = 0.5` °/s | 2026-10-21 |
| headless mode | chạy không cửa sổ | Chạy simulator không mở cửa sổ, chỉ in số. Dùng cho tests. | `imu-sim-headless.exe --scenario still` | 2026-10-21 |
| sample | mẫu dữ liệu | Một lần đọc cảm biến: dt, gyro, accel. | 100 sample mỗi giây → dt = 0.01 s | 2026-10-21 |
| gain | hệ số khuếch đại | Số pixel con trỏ đi được cho mỗi độ xoay. | gain 40 px/°: yaw 10° → con trỏ đi 400 px | 2026-10-21 |
| pitch | ngẩng/chúc | Xoay quanh trục y của remote (trục sang trái). | Mũi remote chúc xuống → con trỏ đi xuống; pitch > 0. | 2026-10-22 |
| roll | vặn cổ tay | Xoay quanh trục x của remote (trục dọc thân, hướng về TV). | Vặn cổ tay → mũi đứng yên, con trỏ không đi. | 2026-10-22 |
| direction of motion | hướng chuyển động | Hướng mà một điểm trên remote đang đi tới; luôn vuông góc với trục xoay. | yaw: trục dọc (z), mũi đi ngang. | 2026-10-22 |
| right-hand rule | quy tắc bàn tay phải | Cách tìm chiều quay dương: ngón cái theo trục, bốn ngón cong theo chiều dương. | Ngón cái lên (+z) → yaw dương = mũi sang trái. | 2026-10-22 |
