# Bài 02 — Cộng dồn gyro thành góc (integration)

**Loại bài:** implement-from-tests (phần chính) + predict, then measure (stretch).
**Scaffold level:** S3 Guided — có các bước `TODO` và pseudocode. Nói "more help" nếu bạn bị kẹt lâu.
**Thời gian:** khoảng 30 phút.

## Vì sao cần cho bản demo

Trong bản demo, `onImuSample` chỉ nhận **tốc độ xoay** từ gyro, không nhận góc. Muốn con trỏ chỉ đúng một ô menu,
code phải biết remote đang ở góc nào. Bài này viết đúng phần đó: cộng dồn gyro thành yaw và pitch.
Ngày 3, bạn sẽ đổi yaw và pitch thành vị trí con trỏ.

---

## Nhiệm vụ

Bạn chỉ sửa `student.cpp`. Không sửa `student.h`, `tests/` và `CMakeLists.txt`.

**TODO 1:** Build và chạy tests trên code chưa sửa (các lệnh ở mục "Cách kiểm tra"). Cả 6 test đều fail. Đây là
điều bình thường.

**TODO 2 → 4:** làm theo các ghi chú `// TODO` trong `student.cpp`, theo đúng thứ tự.

**Xong khi:** cả 6 test pass, và sai số lớn nhất của `yawSweep` và `pitchSweep` dưới 1°.

### Những thứ bạn dùng trong code

| Viết | Đọc là | Kiểu | Ví dụ |
|---|---|---|---|
| `a` | góc hiện tại (tham số của `integrateGyro`) | `Angles&` | `{ yaw: 10, pitch: 0 }` |
| `a.yaw` | góc yaw trong `a` | `float`, ° | `10` |
| `s.dt` | thời gian của sample | `float`, giây | `0.01` |
| `s.gyro.z` | tốc độ xoay quanh trục z | `float`, °/s | `15` |
| `x += y;` | lấy x cũ cộng thêm y, lưu lại vào x | | x = 10, y = 0.15 → x = 10.15 |
| `reportAngles(yaw, pitch, roll)` | gửi góc của bạn cho simulator để vẽ biểu đồ | hàm có sẵn | `reportAngles(10.15f, 0, 0)` |

Ví dụ một sample: `a.yaw = 10`, `s.gyro.z = 15`, `s.dt = 0.01` → sau khi gọi hàm, `a.yaw = 10.15`.

### YOU DECIDE (TODO 4): biến góc sống ở đâu?

Biến bình thường khai báo **bên trong** một hàm sẽ bị xoá khi hàm kết thúc. Lần gọi sau, biến được tạo lại từ đầu.
Góc của bạn phải còn nguyên giữa hai lần gọi `onImuSample`. Có hai cách:

1. **Biến `static` bên trong hàm.** Từ khoá `static` đặt trước một biến trong hàm nghĩa là: tạo biến **một lần**
   ở lần gọi đầu tiên, rồi giữ giá trị qua mọi lần gọi sau. Chỉ hàm đó nhìn thấy biến này.
2. **Biến toàn cục (global variable).** Đây là biến khai báo **bên ngoài** mọi hàm, ở đầu hoặc giữa file. Biến này
   sống suốt chương trình. Mọi hàm viết bên dưới nó trong file đều đọc và sửa được biến này.

Ví dụ nhỏ (đếm số lần gọi, không phải bài của bạn):

```cpp
int countA()  { int n = 0;        n += 1; return n; } // luôn trả về 1: n được tạo lại mỗi lần gọi
int countB()  { static int n = 0; n += 1; return n; } // 1, 2, 3, ...: n được giữ lại
int total = 0;                                         // biến toàn cục
int countC()  { total += 1; return total; }            // 1, 2, 3, ...: hàm khác cũng sửa được total
```

Chọn (1) hoặc (2) cho biến `Angles` của bạn. Viết lý do **có so sánh**: lựa chọn của bạn làm được gì mà cách kia
không làm được, hoặc làm kém hơn. Gợi ý: nghĩ xem **ai** có thể sửa biến đó, và Ngày 3 hoặc Ngày 4 có hàm nào khác
cần đọc góc không.

---

## Câu trả lời của bạn

### YOU DECIDE (TODO 4)
- Lựa chọn: _global_
- Lý do (1–2 câu, có so sánh): I choose global, because this variable may be used in other functions, if I choose static varialbe in onImuSample(), this variable is only modified by calling onImuSample() 

### Xem bằng mắt (sau khi 6 test pass)
Làm theo mục "Xem trong imu-sim". Ghi lại:
- Sai số yaw lớn nhất mà biểu đồ cho thấy trong `yaw-sweep`: 0°
- Khi bấm **R** giữa chừng, nét mảnh có quay về 0 không? khong, do bi cong don goc truoc do.

---

## Stretch goal (không bắt buộc, scaffold S2): đoán rồi đo drift

Ngày 4 sẽ học về **bias**: gyro báo một tốc độ nhỏ dù remote nằm yên. Thử đoán trước bằng công thức của hôm nay.

1. Remote nằm yên 30 giây (scenario `still`). Gyro có bias 0.5 °/s trên cả 3 trục. **Đoán** yaw của bạn sau 30 s.
   Ghi số đoán vào bảng **trước khi chạy**.
2. Đo bằng simulator (lệnh ở mục "Cách kiểm tra", bước 5), rồi điền kết quả.

| Bias | Thời gian | Đoán yaw (°) | Đo được `angle err` (°) |
|---|---|---|---|
| 0.5 °/s | 30 s | 15|15 |
| 0.2 °/s | 30 s | 6| 6|

---

## Cách kiểm tra

Chạy từ thư mục gốc `motion-pointer/`:

1. `cmake -S exercises/02-gyro-integration -B build/02-gyro-integration`
2. `cmake --build build/02-gyro-integration --config Debug`
3. `ctest --test-dir build/02-gyro-integration -C Debug --output-on-failure`

Bước 1 chỉ cần chạy một lần. Sau mỗi lần sửa code: **lưu file**, rồi chạy lại bước 2 và bước 3.
Chỉ chạy một test, ví dụ `yawSweep`: thêm `-R yawSweep` vào cuối lệnh 3.

Test `yawSweep` và `pitchSweep` in một bảng: mỗi giây một dòng, có góc thật, góc của bạn và sai số. Hãy đọc bảng này.

### Xem trong imu-sim (sau khi 6 test pass)

Đóng cửa sổ `imu-sim` đang mở (nếu có). Sau đó:

4. Build simulator với file của bạn:
   ```
   cmake -S tools/imu-sim -B build/imu-sim "-DIMUSIM_PLUGIN=../../exercises/02-gyro-integration/student.cpp"
   cmake --build build/imu-sim --config Release
   ```
5. Đo bằng số (không mở cửa sổ): `build\imu-sim\Release\imu-sim-headless.exe --scenario yaw-sweep`
   Đọc cột cuối `angle err`: sai số lúc cuối / sai số lớn nhất, đơn vị °.
   Stretch: thêm `--scenario still --bias 0.5`.
6. Xem bằng mắt: `build\imu-sim\Release\imu-sim.exe --scenario yaw-sweep`
   Biểu đồ ở góc dưới bên trái (phím **C** bật/tắt). **Dải mờ** = góc thật. **Nét mảnh** = góc code của bạn.
   Code đúng thì nét mảnh nằm giữa dải mờ. Không kéo chuột khi scenario đang chạy.

Lưu ý: phím R đặt lại remote, nhưng **không** xoá biến `static` hoặc biến toàn cục trong code của bạn.

## Giới hạn
- Chỉ sửa `student.cpp`.
- `integrateGyro` cần 2 dòng code. `onImuSample` cần khoảng 3 dòng. Nếu bạn viết nhiều hơn gấp đôi, hãy dừng lại và hỏi.
- Không sửa `p` (con trỏ). Con trỏ là bài Ngày 3.
- Bài này tắt noise và bias. Tests luôn chạy với noise = 0.
