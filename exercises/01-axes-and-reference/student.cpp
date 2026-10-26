// Bài 01 — debug lab: dấu &, dấu của góc, và trục xoay.
// Code dưới đây có 3 lỗi cài sẵn. Làm theo TODO 1 → 6 theo thứ tự. Đọc README.md trước.
// Quy ước trục và dấu: tools/imu-sim/README.md, mục 6.

#include "student.h"

// Hàm phụ: cộng thêm deg độ vào một góc.
// TODO 2: Tests của cả 3 hàm turnLeft, tiltDown, twistRight đều fail, kể cả hàm viết đúng.
//         Hãy xem tham số `angle` của hàm addTo: hàm sửa biến gốc hay sửa một bản sao?
//         Đọc lại ghi chú: learning/knowledge/cpp/pass-by-value-vs-reference.md. Sửa lỗi, rồi chạy lại tests.
static void addTo(float& angle, float deg)
{
    angle += deg;
}

// TODO 3: Sau TODO 2, test turnLeft vẫn fail. Mở tools/imu-sim/README.md mục 6:
//         mũi sang trái thì yaw dương hay âm? So với code bên dưới, rồi sửa.
void turnLeft(Angles& a, float deg)
{
    addTo(a.yaw, +deg);
}

// Hàm này đúng. Đừng sửa. Dùng nó làm mẫu.
void tiltDown(Angles& a, float deg)
{
    addTo(a.pitch, deg);
}

// TODO 4: Vặn cổ tay là roll. roll xoay quanh trục nào, và lưu trong trường (field) nào của Angles?
//         So với code bên dưới, rồi sửa.
void twistRight(Angles& a, float deg)
{
    addTo(a.roll, +deg);
}

// TODO 5: Viết hàm lookUp: mũi remote ngẩng LÊN deg độ.
//   Pseudocode:
//     - ngẩng lên / chúc xuống là chuyển động nào trong 3 chuyển động? → chọn đúng trường của Angles
//     - mũi ngẩng LÊN thì góc đó tăng hay giảm? (README mục 6, hoặc quy tắc "nhìn từ đầu mũi tên")
//     - cộng hoặc trừ deg vào trường đó
//   YOU DECIDE: gọi addTo trực tiếp, hay gọi lại một hàm có sẵn ở trên?
//               Viết lựa chọn của bạn và lý do (1 câu) vào README.md, mục "Câu trả lời của bạn".
void lookUp(Angles& a, float deg)
{
    addTo(a.pitch, -deg);
}

// STRETCH (TODO 6): Xoay deg độ theo chiều DƯƠNG quanh trục axis ('x', 'y' hoặc 'z').
//   Kết quả cần có: mỗi trục đổi đúng một góc; ký tự khác thì không đổi góc nào.
void rotateAbout(Angles& a, char axis, float deg)
{
    switch (axis) {
        case 'x':
            twistRight(a, deg);
            break;
        case 'y':
            tiltDown(a, deg);
            break;
        case 'z':
            turnLeft(a, deg);
            break;
        default:
            // Không làm gì cả nếu axis không hợp lệ
            break;
    }
}
