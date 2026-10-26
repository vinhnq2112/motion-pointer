# Build trên Windows và lựa chọn thiết kế nhỏ

## Know-how — 2026-10-27 (exercise 01)
**MSBuild không in dòng `student.cpp` = file chưa được lưu.**
- Khi build, MSBuild in tên file `.cpp` mà nó biên dịch lại.
- Nếu không thấy `student.cpp`, MSBuild thấy file không đổi. Thường là do quên Ctrl+S.
- Cách sửa: lưu file, rồi build lại.

## Know-how — 2026-10-27
**Lệnh `!` trong Claude Code chạy bằng bash.** Trong bash, `\` có nghĩa đặc biệt và bị xoá.
- Sai: `! build\imu-sim\Release\imu-sim.exe` → bash tìm `buildimu-simReleaseimu-sim.exe`.
- Đúng: `! build/imu-sim/Release/imu-sim.exe` (dùng `/`).

## Pitfall — 2026-10-27 (exercise 01, TODO 5, YOU DECIDE)
**Câu hỏi:** `lookUp` nên gọi `addTo(a.pitch, -deg)` hay `tiltDown(a, -deg)`?

**Cả hai đều đúng.** Mỗi cách có một điểm mạnh:

| Tình huống | (1) `addTo` trực tiếp | (2) gọi `tiltDown` |
|---|---|---|
| Quy ước dấu của pitch đổi | sửa 2 chỗ | sửa 1 chỗ |
| `tiltDown` có lỗi riêng | `lookUp` vẫn đúng | `lookUp` sai theo |

- (1): hai hàm **độc lập**, lỗi không lan.
- (2): quy tắc dấu chỉ nằm ở **một nơi**.

**Hiểu sai đã gặp:** lúc đầu đếm ngược số chỗ cần sửa cho tình huống "quy ước dấu đổi".
Lý do "vì code giống `tiltDown`" chưa đủ: một lý do phải so sánh hai lựa chọn.
