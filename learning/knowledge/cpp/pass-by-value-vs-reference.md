# Pass by value vs pass by reference

## Pitfall — 2026-10-21 (warm-up)
**Câu hỏi:** `void addOne(Vec3 v) { v.x = v.x + 1; }` — gọi `addOne(a)` với `a.x = 2`. Sau đó `a.x` bằng bao nhiêu?

**Đáp án đúng:** `2`.

**Hiểu sai đã gặp:** "a bị sửa thành 3 trong hàm, rồi reset lại 2 khi thoát hàm."

**Đúng là:** `a` **không bao giờ** bị sửa.
1. Khi gọi `addOne(a)`, C++ tạo biến mới `v` là **bản sao** của `a`.
2. Hàm sửa `v.x` thành 3. `a.x` vẫn là 2.
3. Khi hàm kết thúc, `v` bị xoá. `a` không cần "reset" vì chưa từng đổi.

```cpp
void addOne(Vec3 v)  { v.x += 1; }  // pass by value: sửa bản sao, a không đổi
void addOne(Vec3& v) { v.x += 1; }  // pass by reference: v là chính a, a.x thành 3
```

**Vì sao quan trọng cho bản demo:** `onImuSample(const ImuSample& s, PointerState& p)` nhận `p` bằng `&`.
Nếu bỏ `&`, code sửa bản sao của con trỏ → con trỏ trên TV ảo không bao giờ di chuyển.
