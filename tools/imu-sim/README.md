# imu-sim: simulator remote con trỏ (air mouse)

`imu-sim` là một **learning tool** (công cụ học). Mentor viết và sửa công cụ này. Bạn **không** cần đọc code bên
trong. Bạn chỉ viết một file `.cpp` nhỏ rồi cắm vào simulator (xem mục 5).

Simulator thay cho thiết bị thật. Nó cho bạn:
- xem remote xoay trong không gian 3D, kèm trục xoay vẽ bằng mũi tên màu;
- nhận dữ liệu gyro và accelerometer giả, giống chip IMU thật;
- so con trỏ do code của bạn tính với con trỏ đúng (ground truth).

## 1. Build (lần đầu cần internet)

Chạy 2 lệnh này từ thư mục gốc `motion-pointer/`:

```
cmake -S tools/imu-sim -B build/imu-sim
cmake --build build/imu-sim --config Release
```

Lần build đầu, CMake tự tải thư viện đồ hoạ raylib 5.5. Bước này mất khoảng 1–2 phút.

Kiểm tra simulator (tự test phần vật lý):

```
ctest --test-dir build/imu-sim -C Release --output-on-failure
```

Kết quả đúng: `100% tests passed`.

## 2. Chạy

| Lệnh | Làm gì |
|---|---|
| `build\imu-sim\Release\imu-sim.exe` | Mở cửa sổ 3D, xoay remote bằng chuột |
| `build\imu-sim\Release\imu-sim.exe --scenario yaw-sweep` | Mở cửa sổ và chạy ngay một scenario |
| `build\imu-sim\Release\imu-sim.exe --mystery` | Chế độ "bí ẩn": phím 1–3 chạy 3 chuyển động không có tên, đáp án bị ẩn (mục 7) |
| `build\imu-sim\Release\imu-sim-headless.exe` | Chạy mọi scenario, không mở cửa sổ, in bảng số |
| `build\imu-sim\Release\imu-sim-headless.exe --list` | Liệt kê các scenario |

## 3. Trên màn hình có gì

**Bên trái: remote 3D**

| Thứ bạn thấy | Ý nghĩa |
|---|---|
| Hộp xám dài | Remote. Mặt có nút trắng (nút OK) là mặt trên. Đầu có nút đỏ là mũi remote, hướng về TV. |
| Mũi tên **đỏ x** | Trục x của remote: dọc thân remote, hướng về TV |
| Mũi tên **xanh lá y** | Trục y: sang trái |
| Mũi tên **xanh dương z** | Trục z: lên trên |
| Đường **vàng** + mũi tên cong vàng | **Trục xoay**. Remote đang quay quanh đường này. Mũi tên cong cho biết chiều quay. |
| Mũi tên **trắng** | **Hướng chuyển động** của mũi remote. Khi chỉ có roll, mũi tên trắng nằm ở đầu trục y. |

Ba mũi tên x, y, z gắn chặt vào remote và xoay cùng remote. Người ta gọi hệ trục này là **body frame**.

Hai thứ khác nhau cần nhớ:
- **Trục xoay** (vàng) đứng yên, còn remote quay quanh trục đó.
- **Hướng chuyển động** (trắng) là hướng mà một điểm trên remote đang đi tới.

Hai hướng này luôn vuông góc với nhau.

**Góc dưới bên trái: biểu đồ góc** (phím C bật/tắt)

| Thứ bạn thấy | Ý nghĩa |
|---|---|
| Dải mờ xanh dương / xanh lá | yaw / pitch **thật** trong 10 giây gần nhất |
| Nét mảnh xanh dương / xanh lá | yaw / pitch do **code của bạn** tính, gửi qua `reportAngles()` (mục 5) |
| "sai số bây giờ" | Khoảng cách giữa nét mảnh và dải mờ, đơn vị ° |

Code đúng thì nét mảnh nằm giữa dải mờ. Nếu code chưa gọi `reportAngles()`, biểu đồ chỉ có dải mờ.

**Bên phải: TV ảo**

| Thứ bạn thấy | Ý nghĩa |
|---|---|
| Lưới 12 ô | Menu TV. Ô có con trỏ ở trên được tô xanh. |
| Chấm **trắng** | Con trỏ do code của bạn tính (`PointerState p`) |
| Vòng **vàng** | Con trỏ đúng (ground truth): nơi người dùng thật sự muốn chỉ |
| Chấm **đỏ** + chữ "ra ngoài màn hình" | Con trỏ của bạn đã ra ngoài TV. Đây là lỗi. |
| "Góc thật" | yaw, pitch, roll thật của remote, đơn vị độ (°) |
| "gyro", "accel" | Đúng dữ liệu mà code của bạn nhận trong `ImuSample` |
| "sai số" | Khoảng cách giữa chấm trắng và vòng vàng, đơn vị pixel (px) |

## 4. Điều khiển

| Thao tác | Kết quả |
|---|---|
| Kéo chuột trái sang ngang | yaw (mũi remote quay trái/phải) |
| Kéo chuột trái lên/xuống | pitch (mũi remote ngẩng lên/chúc xuống) |
| Kéo chuột phải sang ngang | roll (vặn cổ tay) |
| Phím ← → / ↑ ↓ / Q E | Xoay đúng một trục: yaw / pitch / roll |
| Phím 1–6 | Chạy scenario số 1–6 |
| R | Đặt lại: remote thẳng, con trỏ về giữa |
| Space hoặc Enter | Nút OK: chọn ô dưới con trỏ |
| A | Ẩn/hiện trục xoay, hướng chuyển động, góc thật, số gyro và accel (dùng cho bài "tự chỉ trục") |
| G | Ẩn/hiện con trỏ đúng (vòng vàng) |
| C | Ẩn/hiện biểu đồ góc |
| P | Tạm dừng |
| N / B / T | Đổi knob: noise / gyroBias / handTremor (mỗi lần bấm chuyển sang mức tiếp theo) |

Lưu ý: phím R không xoá biến `static` trong code của bạn. Nếu code của bạn lưu góc, hãy tắt rồi mở lại simulator.

## 5. Cắm code của bạn vào (plug-in interface)

Bạn viết một file `.cpp` có đúng hàm sau (khai báo trong `tools/imu-sim/include/imu_plugin.h`):

```cpp
struct Vec3 { float x, y, z; };
struct ImuSample { float dt; Vec3 gyro; Vec3 accel; }; // dt: giây · gyro: °/s · accel: m/s²
struct PointerState { float x, y; };                   // pixel trên TV ảo, (0,0) góc trên-trái
void onImuSample(const ImuSample& s, PointerState& p); // gọi 100 lần/giây
```

- Simulator gọi `onImuSample` một lần cho mỗi mẫu (sample) dữ liệu.
- Lúc bắt đầu, `p` nằm ở giữa màn hình: (960, 540).
- Code của bạn sửa `p`. Vì `p` được truyền bằng `&` (pass by reference), simulator thấy thay đổi.

**Không bắt buộc: cho simulator xem góc của bạn.** Simulator có sẵn hàm này (bạn chỉ gọi, không viết):

```cpp
void reportAngles(float yaw, float pitch, float roll); // góc code của bạn tính, đơn vị °
```

Gọi hàm này trong `onImuSample`, sau khi tính góc. Cửa sổ vẽ góc của bạn lên biểu đồ (mục 3). Headless mode in
sai số góc ở cột `angle err` (mục 9). Nếu bạn chưa tính roll, truyền `0`.

File `.cpp` của bạn có thể `#include` file `.h` nằm cùng thư mục (ví dụ `student.h` của bài tập).

Chọn file của bạn khi build (đường dẫn tính từ `tools/imu-sim/`):

```
cmake -S tools/imu-sim -B build/imu-sim -DIMUSIM_PLUGIN=../../project/pointer.cpp
cmake --build build/imu-sim --config Release
```

Muốn quay lại file mặc định: `-DIMUSIM_PLUGIN=plugin/placeholder.cpp`.

File mặc định `plugin/placeholder.cpp` **không làm gì**, nên con trỏ đứng yên ở giữa. File này chỉ giúp simulator
chạy được khi bạn chưa viết code. Đây không phải lời giải.

## 6. Quy ước trục và dấu (dùng giống nhau ở mọi nơi)

- **x** = dọc thân remote, hướng về TV · **y** = sang trái · **z** = lên trên.
- Quy tắc bàn tay phải: ngón cái chỉ theo trục, các ngón còn lại cong theo chiều quay dương.
- **yaw** = xoay quanh z. yaw > 0: mũi remote quay sang **trái**.
- **pitch** = xoay quanh y. pitch > 0: mũi remote chúc **xuống**.
- **roll** = xoay quanh x. roll > 0: cạnh **phải** của remote đi xuống.
- Khi remote nằm yên và thẳng, accelerometer đọc (0, 0, +9.81) m/s².

**Con trỏ đúng (ground truth)** được tính như sau:
`x = 960 − 40 × yaw`, `y = 540 + 40 × pitch`, rồi giữ trong màn hình.
Số 40 px/° là gain (hệ số khuếch đại), đổi bằng `--gain`. Roll không làm con trỏ di chuyển.
Đây là cách tính đơn giản; bài Ngày 3 sẽ nói rõ hơn.

## 7. Scenario (chuyển động ghi sẵn)

| # | Tên | Thời gian | Chuyển động |
|---|---|---|---|
| 1 | `still` | 30 s | Remote nằm yên, chĩa thẳng vào TV |
| 2 | `yaw-sweep` | 10 s | Chỉ yaw: trái 20°, phải 20°, về giữa |
| 3 | `pitch-sweep` | 10 s | Chỉ pitch: xuống 12°, lên 12°, về giữa |
| 4 | `roll-twist` | 8 s | Chỉ roll: phải 45°, trái 45°, về giữa |
| 5 | `figure` | 20 s | Kết hợp cả 3 trục |
| 6 | `wild` | 10 s | Vung tay mạnh, góc rất lớn (để thử screen bound) |

**Chế độ bí ẩn** (`--mystery`): phím 1–3 chạy `bi-an-1`, `bi-an-2`, `bi-an-3`. Mỗi chuyển động chỉ xoay quanh **một** trục,
mỗi lần 8 s. Lúc đầu mọi đáp án bị ẩn, kể cả vòng vàng. Nhấn A để hiện đáp án, nhấn G để hiện con trỏ đúng.

## 8. Knob (núm chỉnh) và tuỳ chọn dòng lệnh

Ngày đầu mọi knob tắt (bằng 0). Bài học sẽ bật dần từng knob.

| Knob | Tuỳ chọn | Mặc định | Ý nghĩa |
|---|---|---|---|
| `noise` | `--noise X` | 0 | Nhiễu ngẫu nhiên của gyro, độ lệch chuẩn X °/s. Accelerometer có nhiễu 0.1 × X m/s². |
| `gyroBias` | `--bias X` | 0 | Lỗi lệch cố định của gyro, X °/s, cộng vào cả x, y, z |
| `sampleRate` | `--rate X` | 100 | Số mẫu mỗi giây (Hz). `dt = 1 / sampleRate` |
| `handTremor` | `--tremor X` | 0 | Tay rung nhẹ 8–11 Hz, biên độ X°. Con trỏ đúng không tính phần rung này. |
| `screenSize` | `--screen WxH` | 1920x1080 | Kích thước TV ảo |
| | `--gain X` | 40 | Gain của con trỏ đúng, px/° |
| | `--seed N` | 1 | Hạt giống (seed) của nhiễu: cùng seed thì cùng nhiễu |
| | `--scenario NAME` | all | Scenario cần chạy |
| | `--csv PATH` | | Headless: ghi từng mẫu ra file CSV |
| | `--screenshot PATH --at S` | | Cửa sổ: chạy đến giây S rồi lưu ảnh PNG và thoát |

## 9. Bảng số của headless mode

`imu-sim-headless.exe` in một dòng cho mỗi scenario:

| Cột | Ý nghĩa |
|---|---|
| `true yaw/pitch/roll (end)` | Góc thật lúc cuối scenario (°) |
| `pointer (end)` / `truth (end)` | Con trỏ của bạn / con trỏ đúng lúc cuối (px) |
| `err end` / `err max` | Sai số con trỏ lúc cuối / lớn nhất trong cả scenario (px) |
| `off-screen` | Số mẫu mà con trỏ của bạn nằm ngoài màn hình. Phải bằng 0. |
| `moved from start` | Con trỏ của bạn đi xa điểm bắt đầu nhất bao nhiêu (px). Dùng cho `still`: đo drift. |
| `angle err` | Sai số góc lúc cuối / lớn nhất (°): lấy số lớn hơn giữa sai số yaw và sai số pitch. Dấu `-` nghĩa là code chưa gọi `reportAngles()`. |

## 10. Giới hạn (đơn giản hoá có chủ ý)

- Accelerometer chỉ đo trọng lực, không đo gia tốc do tay di chuyển. Bài Ngày 5 có thể bật thêm phần này.
- Góc Euler: không xử lý trường hợp pitch gần ±90°. Chuột bị giới hạn pitch ±80°.
- Không có magnetometer, không có quaternion, không có Bluetooth (ngoài phạm vi khoá học).

## Cho mentor / tests

- `core/` (`imusim_core`): mô hình chuyển động, cảm biến, scenario. Không cần đồ hoạ. Tests của bài tập link
  vào thư viện này: `add_subdirectory(<path>/tools/imu-sim/core imusim_core)`.
- `src/runner.*`: nối core với `onImuSample`. `src/gui_main.cpp`: cửa sổ raylib. `src/headless_main.cpp`.
- `tests/selftest.cpp`: test vật lý (trọng lực, quy ước dấu, tích phân gyro, bias, nhiễu, con trỏ đúng).
- Dữ liệu gyro tính tại giữa mỗi mẫu, góc thật tính tại cuối mẫu. Vì vậy `angle += gyro * dt` cho sai số
  < 0.01° khi không có nhiễu.
- `reportAngles()` lưu góc vào `runner.cpp` (`reportedAngles()`); mỗi `Session` mới đặt lại `valid = false`.
  Tests của bài tập link thêm `src/runner.cpp` để chạy `onImuSample` qua `Session`.

## Changes

- 2026-10-28: `reportAngles(yaw, pitch, roll)` trong interface (không bắt buộc). Cửa sổ: biểu đồ góc thật vs góc
  learner, 10 s (phím C). Headless: cột `angle err`. Thư mục của plug-in được thêm vào include path.

- 2026-10-22: chế độ `--mystery` (3 chuyển động một trục, đáp án ẩn) cho bài 01. Phím A ẩn thêm góc thật và accel.
  Self-test thêm 4 kiểm tra (22/22).
- 2026-10-21: bản đầu tiên. Remote 3D + trục + trục xoay + hướng chuyển động, TV ảo + menu, 6 scenario,
  headless mode, knob noise/gyroBias/handTremor, self-test vật lý (18 kiểm tra).
