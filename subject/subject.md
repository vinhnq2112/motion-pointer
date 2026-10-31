# Subject: Gyro + accelerometer cho remote điều khiển con trỏ (air mouse)

## Learner
- **Background:** viết C++ ở mức cơ bản. Chưa biết gì về IMU, gyro, accelerometer. Không có thiết bị thật.
- **Current level (verified):** kiểm tra ngày 2026-10-21.
  - C++: tự chấm 3/5. Viết được hàm, `struct`, vòng lặp. **Còn nhầm** truyền bản sao (pass by value) và
    truyền tham chiếu (pass by reference, `&`): đoán `addOne(Vec3 v)` có sửa biến gốc.
  - Hình học không gian: **nhầm hướng chuyển động với trục xoay** (nói yaw là xoay quanh trục ngang).
  - Toán: tốc độ × thời gian đúng. Tự hiểu được drift bằng trực giác (0.5 °/s × 60 s = 30°).
- **Preferences:** lý thuyết trước, code sau (sau khi đã xem hình / simulator).
- **Learning needs:** yếu hình học không gian → **visuals first** (`CLAUDE.md` rule 11). Mỗi ý mới: cho xem
  hình hoặc chuyển động trong simulator `imu-sim` trước, có vẽ trục xoay bằng mũi tên màu, rồi mới nói quy tắc.
  Luôn tách rõ "hướng chuyển động" và "trục xoay".
- **Công thức và code (learner yêu cầu 2026-10-28):** mỗi khi đưa ra công thức hoặc một dòng code mới, giải
  thích **từng ký hiệu** ngay bên dưới, trước khi hỏi: biến/tham số đến từ đâu (ví dụ `s` là tham số của
  `onImuSample`), dấu `.` / `&` / `+=` nghĩa là gì, đơn vị của mỗi số. Kèm **một ví dụ có số thật**
  (ví dụ `s.gyro.z = 15`, `s.dt = 0.01` → `0.15°`). Dùng bảng "Viết | Đọc là | Kiểu | Ví dụ".
  Lý do: ngày 2026-10-28, mentor viết `yaw += s.gyro.z * s.dt` mà không nói `s` là gì → learner không hiểu.
- **Language:** tiếng Việt, câu ngắn, từ phổ thông.
  - **Term language:** thuật ngữ kỹ thuật giữ tiếng Anh, lần đầu có giải thích tiếng Việt, rồi thêm vào
    `learning/glossary.md` (`CLAUDE.md` writing rule 11).
  - **Translate later to:** English (`/translate en`).

## Goals
- **Learning goal:** hiểu gyro và accelerometer đủ để tự viết code đổi dữ liệu IMU thành con trỏ trên TV:
  tích phân góc, noise, bias, drift, bias calibration, tilt từ trọng lực, complementary filter, screen bound.
- **Final project / proof of skill:** bản demo trong `imu-sim`: xoay remote ảo bằng chuột → code C++ của
  learner đổi dữ liệu gyro/accelerometer giả (có noise và drift) thành vị trí con trỏ trên TV ảo → con trỏ
  chọn được ô menu → remote nằm yên thì con trỏ không trôi → con trỏ không bao giờ ra ngoài màn hình.
- **Final deliverable:** PowerPoint tiếng Anh, 60 phút, cho đồng nghiệp mới học (beginner). Có phần riêng về
  drift và cách xử lý, gồm cả screen bound (`/present`).
  - **Audience quiz (learner yêu cầu 2026-10-30):** sau mỗi `/quiz`, lưu các câu hay vào `learning/showcase.md`
    (dòng "Audience quiz" của topic đó, tiếng Anh, kèm đáp án). `/present` đưa chúng vào slide thành quiz nhỏ
    cho khán giả.
- **Deadline:** hết 7 ngày học.

## Time budget
- **Course mode:** sprint (mỗi ngày một phase).
- 7 ngày × 1 giờ. Ngày 7: 2 giờ (1 giờ làm slide + 1 giờ tập nói). Tổng 8 giờ.

## Teaching specifics
- **Scope / standard:** góc Euler (yaw, pitch, roll) ở mức đơn giản; không có quaternion, magnetometer,
  Kalman filter, thiết bị thật, Bluetooth (chỉ nhắc tên như "bước tiếp theo" trong slide).
- **Axis convention (dùng thống nhất mọi nơi):** trục của remote (body frame): **x** = dọc thân remote, hướng
  về TV · **y** = sang trái · **z** = lên trên. Quy tắc bàn tay phải. **yaw** = xoay quanh z (con trỏ trái/phải),
  **pitch** = xoay quanh y (con trỏ lên/xuống), **roll** = xoay quanh x (vặn cổ tay).
  Đơn vị: gyro °/s, accelerometer m/s², thời gian giây, con trỏ pixel.
- **Always connect concepts to:** con trỏ trên menu TV trong bản demo cuối khoá.
- **Preferred references:** simulator `imu-sim` là chính. Tài liệu ngoài chỉ khi cần, ưu tiên bài viết có hình.
- **Quiz question types:** chỉ trục xoay trên hình · đoán con trỏ sẽ di chuyển thế nào · tính góc / drift ·
  tìm lỗi trong đoạn code ngắn · giải thích vì sao (ví dụ: vì sao accelerometer không sửa được yaw).
- **Extra exceptions to rule 1 ("I do the work"):** mentor viết tests, file CMake của bài tập, toàn bộ
  `imu-sim` và code nối (glue) giữa simulator và code learner.
- **Scaffolding:** bài tập bắt đầu ở S3 và giảm dần (`CLAUDE.md` rule 9). Project work (code cho bản demo):
  được bắt đầu ở S3, giảm dần như bài tập; "more help" quay lại S3 bất cứ lúc nào.

## Workspace
- **Practice folder:** `exercises/`
- **Project folder:** `project/` (code của learner cho bản demo cuối khoá, cắm vào `imu-sim`)
- **Tools / environment:**
  - Windows 11, Visual Studio Community 2022 17.14 (có bộ C++ x64)
  - CMake 4.0.1, Git, Node.js v22.16.0 (cho `/present`)
  - Không có Python, g++, clang.

## Learning tools
- **imu-sim** — folder `tools/imu-sim/`
  - **Shows:** remote 3D xoay bằng chuột; trục x/y/z của remote và trục xoay hiện tại vẽ bằng mũi tên màu;
    TV ảo có menu dạng lưới và con trỏ; biểu đồ góc thật vs góc learner tính theo thời gian.
  - **Plug-in interface:** learner viết trong một file `.cpp`:
    ```cpp
    struct Vec3 { float x, y, z; };
    struct ImuSample { float dt; Vec3 gyro; Vec3 accel; }; // dt: giây · gyro: °/s · accel: m/s²
    struct PointerState { float x, y; };                   // pixel trên TV ảo, (0,0) góc trên-trái
    void onImuSample(const ImuSample& s, PointerState& p); // gọi 100 lần/giây
    ```
    Mỗi bài tập có thể thêm hàm nhỏ hơn (ví dụ `float integrateYaw(...)`), ghi trong README của bài.
  - **Ground truth:** góc thật yaw/pitch/roll, vị trí con trỏ đúng. In ra sai số góc (°), sai số con trỏ (px),
    drift sau N giây khi remote nằm yên.
  - **Knobs:** `noise` (mặc định 0), `gyroBias` (mặc định 0 °/s), `sampleRate` (mặc định 100 Hz),
    `handTremor` (mặc định 0), `screenSize` (mặc định 1920×1080). Ngày đầu tắt hết, bật dần theo bài.
  - **Headless mode:** chạy dữ liệu ghi sẵn (scenario) không cần cửa sổ, dùng cho tests.
  - **Build / run:** `cmake -S tools/imu-sim -B build/imu-sim` → `cmake --build build/imu-sim --config Release`
    (raylib được CMake tự tải lần đầu, cần internet). Chi tiết trong `tools/imu-sim/README.md`.

## Knowledge categories
`rotation`, `gyro`, `accelerometer`, `drift-and-filters`, `pointer`, `cpp`

## Packs & agents
- **Packs:** none
- **Subject agents:** none
