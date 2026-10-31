# Bài 03 — Từ góc đến con trỏ + screen bound (clamp)

**Loại bài:** implement-from-tests (phần chính) + spatial check trong `imu-sim` + predict, then measure (stretch).
**Scaffold level:** S3 Guided — có các bước `TODO` và pseudocode. Nói "more help" nếu bạn bị kẹt lâu.
**Thời gian:** khoảng 30–40 phút.

## Vì sao cần cho bản demo

Đây là lần đầu code của bạn **điều khiển con trỏ** trên TV ảo. Sau bài này, bạn xoay remote thì con trỏ đi đúng
chiều tay. Bạn vung tay mạnh thì con trỏ vẫn dính ở mép TV, không bao giờ biến mất. Bản demo cuối khoá cần cả hai điều này
để chọn ô menu.

---

## Nhiệm vụ

Bạn chỉ sửa `student.cpp` và phần "Câu trả lời của bạn" trong README này. Không sửa `student.h`, `tests/`,
`CMakeLists.txt`.

**TODO 1:** Build và chạy tests trên code chưa sửa (lệnh ở mục "Cách kiểm tra"). Cả 7 test chính đều fail. Đây là
điều bình thường.

**TODO 2 → 6:** làm theo các ghi chú `// TODO` trong `student.cpp`, theo đúng thứ tự.

**Xong khi:**
- cả 7 test chính pass (`integrate`, `clamp`, `center`, `direction`, `bound`, `plugin-sweep`, `plugin-wild`);
- `plugin-wild` in "sample ngoài màn hình: 0";
- bạn đã điền mục "Câu trả lời của bạn".

### Những thứ bạn dùng trong code

| Viết | Đọc là | Kiểu | Ví dụ |
|---|---|---|---|
| `a` | góc đầu vào (tham số của `anglesToPointer`) | `const Angles&`: đọc được, không sửa được | `{ yaw: 10, pitch: 3 }` |
| `gain` | số pixel cho mỗi độ (tham số) | `float`, px/° | `40` |
| `screenW`, `screenH` | chiều rộng, chiều cao màn hình (tham số) | `float`, pixel | `1920`, `1080` |
| `kGain` | gain **bạn chọn** cho bản demo (TODO 2) | `const float`, px/° | `40` |
| `kScreenW`, `kScreenH` | kích thước TV ảo mặc định (có sẵn trong `student.h`) | `float`, pixel | `1920`, `1080` |
| `if (v < lo) return lo;` | nếu v nhỏ hơn lo thì trả về lo và **dừng hàm ngay** | | v = −240, lo = 0 → trả về 0 |
| `return {x, y};` | tạo một struct mới từ hai số rồi trả về | `PointerState` | `return {560, 540};` |
| `p = q;` | copy toàn bộ struct `q` vào `p` (cả `x` và `y`) | | `p` thành `{560, 540}` |

Ví dụ nhỏ về cách trả về một struct (không phải bài của bạn):

```cpp
Vec3 up() { return {0, 0, 1}; }   // tạo Vec3 có x = 0, y = 0, z = 1 rồi trả về
Vec3 v = up();                     // v.z == 1
```

Ví dụ số cho `anglesToPointer` (giống bài học): `yaw = 10`, `pitch = 0`, gain 40, TV 1920 × 1080 → con trỏ `(560, 540)`.

### YOU DECIDE (TODO 2): chọn gain cho bản demo

Gain lớn hay nhỏ đều có cái giá. Hãy tính trước khi chọn:

| Gain (px/°) | Xoay bao nhiêu độ thì con trỏ đi từ giữa đến mép trái (960 px)? | Tay rung 0.5° → con trỏ rung bao nhiêu px? |
|---|---|---|
| 20 | ? | ? |
| 40 | 960 ÷ 40 = 24° | 0.5 × 40 = 20 px |
| 80 | ? | ? |

- Gain **nhỏ**: phải xoay cổ tay nhiều mới tới mép màn hình.
- Gain **lớn**: chỉ cần xoay ít, nhưng mỗi rung nhẹ của tay cũng làm con trỏ rung nhiều. Day 4 sẽ bật knob `handTremor`.

Chọn một số trong khoảng 10 → 200 và viết vào `kGain`. Viết lý do **có số**: điền 4 ô `?` trong bảng, rồi nói vì sao
bạn chọn số đó.

---

## Câu trả lời của bạn

### YOU DECIDE (TODO 2)
- Bảng gain đã điền (4 ô `?`): ...
- Gain của tôi: ___ px/°
- Lý do (1–2 câu, có số): ...

### Spatial check: xem bằng mắt (sau khi 7 test chính pass)
Làm theo mục "Xem trong imu-sim". Trước mỗi lần thử, **đoán** chấm trắng sẽ đi hướng nào, rồi mới thử.

| Bạn làm | Đoán: chấm trắng đi | Thấy: chấm trắng đi |
|---|---|---|
| Phím ← (yaw, mũi remote quay trái) | | |
| Mũi remote chúc xuống (phím ↑ hoặc ↓, bạn tìm phím nào) | | |
| Phím Q / E (roll) | | |

- Scenario `wild`: có lúc nào thấy chấm đỏ "ra ngoài màn hình" không? ___
- Ở mép màn hình, chấm trắng và vòng vàng có trùng nhau không? ___

---

## Stretch goal (không bắt buộc, scaffold S2): cách "cộng phần thay đổi"

Trong bài học, bạn đã thấy cách tính con trỏ **lại từ góc** ở mỗi sample. Có một cách khác: giữ vị trí con trỏ, mỗi sample
**cộng thêm phần thay đổi** của góc, rồi kẹp. Day 6 sẽ dùng cách này.

Ví dụ: TV rộng 1920, gain 40, con trỏ bắt đầu ở `x = 960`. Remote quay trái từ yaw 0° đến +30°, rồi quay về 0°.

1. **TODO S1 — đoán trước khi code:** với cách "cộng phần thay đổi rồi kẹp", `x` lúc cuối bằng bao nhiêu?
   Gợi ý: con trỏ đi được bao xa sang trái trước khi bị kẹp? Lúc quay về, con trỏ đi bao xa sang phải?
   - Số đoán của tôi: ___ px. Vì sao: ...
2. **TODO S2:** viết `moveXByChange` trong `student.cpp`, rồi chạy test `stretch-relative` (lệnh ở bước 3b).
   - Số test in ra: ___ px. Khớp với số đoán không? ___
3. Một câu: so với cách của phần chính, cách này để lại gì sau khi con trỏ bị kẹp ở mép?
   ...

---

## Cách kiểm tra

Chạy từ thư mục gốc `motion-pointer/`:

1. `cmake -S exercises/03-angles-to-pointer -B build/03-angles-to-pointer`
2. `cmake --build build/03-angles-to-pointer --config Debug`
3. `ctest --test-dir build/03-angles-to-pointer -C Debug -L core --output-on-failure`
   - 3b (stretch): `ctest --test-dir build/03-angles-to-pointer -C Debug -R stretch --output-on-failure`

Bước 1 chỉ cần chạy một lần. Sau mỗi lần sửa code: **lưu file**, rồi chạy lại bước 2 và bước 3.
`-L core` chỉ chạy 7 test chính. Chỉ chạy một test, ví dụ `clamp`: dùng `-R clamp` thay cho `-L core`.

Test `plugin-sweep` in một bảng: mỗi giây một dòng, có con trỏ đúng, con trỏ của bạn và sai số (px). Hãy đọc bảng này.

### Xem trong imu-sim (sau khi 7 test chính pass)

Đóng cửa sổ `imu-sim` đang mở (nếu có). Sau đó:

4. Build simulator với file của bạn:
   ```
   cmake -S tools/imu-sim -B build/imu-sim "-DIMUSIM_PLUGIN=../../exercises/03-angles-to-pointer/student.cpp"
   cmake --build build/imu-sim --config Release
   ```
5. Đo bằng số (không mở cửa sổ). Thay `40` bằng gain bạn chọn:
   `build\imu-sim\Release\imu-sim-headless.exe --gain 40`
   Đọc cột `off-screen` (phải bằng 0 ở mọi dòng) và `err max` (px).
6. Xem bằng mắt (thay `40` bằng gain của bạn):
   - `build\imu-sim\Release\imu-sim.exe --gain 40` → dùng phím, điền bảng "Spatial check".
   - `build\imu-sim\Release\imu-sim.exe --gain 40 --scenario wild` → xem con trỏ ở mép TV.
   **Chấm trắng** = con trỏ của bạn. **Vòng vàng** = con trỏ đúng.

Lưu ý: phím R đặt lại remote, nhưng **không** xoá biến toàn cục `angle` trong code của bạn. Sau khi nhấn R, chấm trắng
sẽ lệch khỏi vòng vàng (bạn đã thấy điều này ở bài 02). Muốn bắt đầu lại: tắt rồi mở lại simulator.

Lưu ý 2: scenario `wild` và `figure` xoay nhiều trục cùng lúc. Khi đó, `yaw += gyro.z × dt` chỉ là gần đúng, nên chấm
trắng có thể lệch khỏi vòng vàng một đoạn. Đây không phải lỗi của bạn (đơn giản hoá có chủ ý; chi tiết nằm ngoài khoá
học). Tests chỉ đo sai số ở `yaw-sweep` và `pitch-sweep`.

## Giới hạn
- Chỉ sửa `student.cpp` và README này.
- `clampf` cần khoảng 3 dòng. `anglesToPointer` cần khoảng 3–5 dòng. `onImuSample` cần khoảng 3 dòng.
  Nếu bạn viết nhiều hơn gấp đôi, hãy dừng lại và hỏi.
- Không viết thẳng `960` hoặc `540` trong code. Test `center` dùng cả TV 1280 × 720.
- Bài này tắt noise và bias. Tests luôn chạy với noise = 0.
