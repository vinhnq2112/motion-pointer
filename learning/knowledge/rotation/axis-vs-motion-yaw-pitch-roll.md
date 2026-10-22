# Trục xoay vs hướng chuyển động · yaw, pitch, roll

## Lesson summary — 2026-10-22 (Day 1)
- **Trục xoay** (axis of rotation) là đường thẳng **đứng yên** khi remote quay quanh nó.
- **Hướng chuyển động** (direction of motion) là hướng mà một điểm trên remote (ví dụ mũi remote) đang đi tới.
- Hai hướng này luôn **vuông góc** nhau. Mũi đi ngang **không** có nghĩa là trục nằm ngang.
- Một điểm nằm **trên** trục xoay thì đứng yên. Mũi remote nằm trên trục x, nên roll không làm mũi di chuyển.

| Tên | Trục xoay | Mũi remote | Con trỏ trên TV | Giá trị dương |
|---|---|---|---|---|
| yaw | z (xanh dương, lên trên) | trái / phải | trái / phải | mũi sang **trái** |
| pitch | y (xanh lá, sang trái) | lên / xuống | lên / xuống | mũi chúc **xuống** |
| roll | x (đỏ, về phía TV) | đứng yên | đứng yên | cạnh **phải** đi **xuống** |

**Vì sao quan trọng cho bản demo:** gyro báo tốc độ xoay **quanh từng trục**, không báo hướng con trỏ đi.
Đọc nhầm trục → con trỏ đi sai hướng.

## Know-how — quy tắc "nhìn từ đầu mũi tên"
Đứng ở **đầu mũi tên** của trục xoay, nhìn về phía remote. Chiều quay dương = **ngược chiều kim đồng hồ**
theo mắt mình (12 giờ → 9 giờ → 6 giờ → 3 giờ). Quy tắc bàn tay phải (right-hand rule) cho kết quả giống hệt:
ngón cái chỉ theo trục, bốn ngón cong theo chiều dương.

## Q&A — 2026-10-22
**Hỏi:** "Tôi chưa hiểu quy tắc bàn tay phải lắm."
**Trả lời:** Dùng cách "nhìn từ đầu mũi tên" ở trên. Ví dụ pitch: đứng bên trái remote (đầu trục +y), nhìn vào
remote. TV và mũi remote ở bên trái mắt mình. Ngược chiều kim đồng hồ: trái → xuống. Vậy pitch dương = mũi chúc xuống.

## Pitfall — 2026-10-22
**Câu hỏi:** roll dương, cạnh phải của remote đi lên hay xuống?
**Trả lời sai đã gặp:** "đi lên".
**Đúng:** đi **xuống**. Đứng ở chỗ TV nhìn lại thì trái/phải bị **đảo**: cạnh phải của remote nằm ở 9 giờ theo
mắt mình. Ngược chiều kim đồng hồ: 9 giờ → 6 giờ = xuống.
**Cách tránh:** trước tiên xác định "cạnh này nằm ở đâu **theo mắt mình**", rồi mới quay theo đồng hồ.

## Pitfall — 2026-10-21 (kiểm tra đầu vào)
Nói "yaw xoay quanh trục ngang" vì thấy mũi đi ngang. Đã sửa ngày 2026-10-22: yaw quay quanh trục **dọc** (z).
