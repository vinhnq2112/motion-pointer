# Bài 01 — Trục xoay, dấu của góc, và dấu `&`

**Loại bài:** debug lab (Phần A) + spatial check (Phần B).
**Scaffold level:** S3 Guided — có các bước `TODO` và pseudocode. Nói "more help" nếu bạn bị kẹt lâu.
**Thời gian:** khoảng 30 phút. Phần A khoảng 15 phút, Phần B khoảng 10 phút.

## Vì sao cần cho bản demo

Trong bản demo, code của bạn nhận dữ liệu gyro rồi cộng vào yaw và pitch. Nếu thiếu `&`, góc sẽ không bao giờ đổi.
Nếu sai dấu, con trỏ đi ngược. Nếu sai trục, vặn cổ tay cũng làm con trỏ chạy. Bài này luyện cả 3 lỗi đó
trên code nhỏ, trước khi bạn gặp chúng trong `onImuSample`.

---

## Phần A — debug lab (code)

`student.cpp` có **3 lỗi cài sẵn** và **1 hàm trống**. Bạn chỉ sửa `student.cpp`.
Không sửa `student.h`, `tests/` và `CMakeLists.txt`.

**TODO 1:** Build và chạy tests trên code chưa sửa (các lệnh ở mục "Cách kiểm tra"). Đọc dòng `[FAIL]` của
từng test.

**TODO 2 → 5:** làm theo các ghi chú `// TODO` trong `student.cpp`, theo đúng thứ tự.
Sau mỗi bước, chạy lại tests. Mỗi lần sửa đúng, sẽ có thêm test chuyển sang pass.

**Xong Phần A khi:** 4 test `turnLeft`, `tiltDown`, `twistRight`, `lookUp` đều pass.

Tests dùng phần vật lý của `imu-sim`. Mỗi test in ra mũi remote thật sự chỉ tới đâu sau khi hàm của bạn chạy.
Hãy đọc các con số này, không chỉ đọc chữ OK/FAIL.

## Phần B — spatial check (trong simulator)

Bạn tự chỉ trục cho 3 chuyển động mà không có đáp án.

1. Chạy: `build\imu-sim\Release\imu-sim.exe --mystery`
2. Nhấn **1** để chạy chuyển động bí ẩn 1. Mỗi chuyển động dài 8 giây. Nhấn lại 1 để xem lần nữa.
3. **Chưa nhấn A.** Nhìn remote, rồi điền một dòng vào bảng bên dưới.
4. Làm giống vậy với phím **2** và **3**.
5. Điền xong cả 3 dòng thì mới nhấn **A** (hiện trục xoay) và **G** (hiện con trỏ đúng), rồi chạy lại để tự kiểm tra.
   **Không sửa câu trả lời** sau khi nhấn A. Nếu sai, ghi lại điều bạn thấy vào cột cuối.

**Xong Phần B khi:** cả 3 dòng có trục và tên đúng.

---

## Câu trả lời của bạn

### Phần A — YOU DECIDE (TODO 5)
- `lookUp` gọi: _(addTo trực tiếp / hàm có sẵn nào?)_
- Lý do (1 câu): I call addTo, I want 2 fucntions titlDown() and lookUp() are independent.
- lookUp() can call titlDown() with purpose: quy tắc dấu nằm ở một nơi

### Phần B — bảng trục (điền TRƯỚC khi nhấn A)

| Bí ẩn | Trục xoay (x / y / z + màu) | Tên (yaw / pitch / roll) | Mũi remote đi thế nào | Con trỏ trên TV đi thế nào | Sau khi nhấn A: đúng? |
|---|---|---|---|---|---|
| 1 |x + red |roll | đứng yên | đứng yên | yes|
| 2 | z + blue | yaw | left, right | left, right | yes|
| 3 | y + green| pitch | up, down | up, down| yes| 

---

## Stretch goal (không bắt buộc, scaffold S2)

**TODO 6:** viết `rotateAbout(a, axis, deg)` trong `student.cpp`. Hàm xoay `deg` độ theo chiều **dương** quanh
trục `'x'`, `'y'` hoặc `'z'`. Ký tự khác thì không đổi góc nào. Test: `stretch-rotateAbout`.

Thêm cho Phần B: với mỗi chuyển động bí ẩn, đoán lần xoay **đầu tiên** là góc dương hay âm. Dùng quy tắc
"nhìn từ đầu mũi tên", rồi kiểm tra bằng phím A. Ghi vào bảng.

Test `stretch-rotateAbout` sẽ **fail** cho đến khi bạn làm stretch goal. Điều này bình thường.

---

## Cách kiểm tra

Chạy từ thư mục gốc `motion-pointer/`:

1. `cmake -S exercises/01-axes-and-reference -B build/01-axes-and-reference`
2. `cmake --build build/01-axes-and-reference --config Debug`
3. `ctest --test-dir build/01-axes-and-reference -C Debug --output-on-failure`

Chỉ chạy một test, ví dụ test `turnLeft`: thêm `-R turnLeft` vào cuối lệnh 3.

Bước 1 chỉ cần chạy một lần. Sau mỗi lần sửa code, chạy lại bước 2 và bước 3.

## Giới hạn
- Chỉ sửa `student.cpp`.
- Mỗi hàm trong Phần A cần 1 dòng code. Nếu bạn viết nhiều hơn 3 dòng, hãy dừng lại và hỏi.
- Phần B không dùng code. Phần B chỉ dùng mắt và quy tắc trục.
