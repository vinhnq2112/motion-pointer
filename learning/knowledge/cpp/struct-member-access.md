# Đọc dữ liệu trong struct: `s.gyro.z`

## Q&A — 2026-10-28 (lesson Day 2)
**Câu hỏi:** Trong `s.gyro.z * s.dt`, ký hiệu `s` là gì?

**Trả lời:** `s` là **tên tham số** của hàm `onImuSample`. Simulator truyền một sample vào tham số này.

```cpp
void onImuSample(const ImuSample& s, PointerState& p);
//                              ^ tên tham số: s = "sample"
```

Dấu chấm `.` nghĩa là "lấy phần bên trong". Đọc từ trái sang phải:

| Viết | Đọc là | Kiểu | Ví dụ giá trị |
|---|---|---|---|
| `s` | cả sample | `ImuSample` | `{ dt, gyro, accel }` |
| `s.dt` | thời gian của sample | `float` (giây) | `0.01` |
| `s.gyro` | ba số gyro | `Vec3` (°/s) | `{0, 0, 15}` |
| `s.gyro.z` | tốc độ xoay quanh trục z | `float` (°/s) | `15` |

Vậy `s.gyro.z * s.dt` = 15 × 0.01 = 0.15° (mẩu yaw nhỏ của sample đó).

**Ghi chú:**
- Tên `s` do người viết hàm chọn. Đổi thành `sample` cũng chạy giống hệt.
- `const ... &`: hàm đọc được `s` nhưng không sửa được. `&` giúp không phải sao chép cả sample.
- `p` (`PointerState&`) không có `const`, nên hàm sửa được `p.x`, `p.y`.

**Vì sao quan trọng cho bản demo:** mọi dữ liệu IMU đến với code của bạn đều qua `s`.
Gyro cho yaw là `s.gyro.z`, gyro cho pitch là `s.gyro.y`.
