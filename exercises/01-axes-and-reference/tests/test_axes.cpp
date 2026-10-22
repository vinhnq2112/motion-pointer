// Tests for exercise 01. Mentor-owned.
// Each test calls the learner's function, then uses the imu-sim physics (bodyToWorld) to show
// where the nose / right side of the remote really went.
// Usage: test_axes <turnLeft|tiltDown|twistRight|lookUp|rotateAbout>

#include <cmath>
#include <cstdio>
#include <cstring>

#include "imusim_core.h"
#include "student.h"

using imusim::V3;

static int failures = 0;

static void check(bool ok, const char* what)
{
    std::printf("  [%s] %s\n", ok ? " OK " : "FAIL", what);
    if (!ok) ++failures;
}

static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

static void printAngles(const char* label, const Angles& a)
{
    std::printf("  %-28s yaw %+6.1f   pitch %+6.1f   roll %+6.1f\n", label, a.yaw, a.pitch, a.roll);
}

// Where body-frame vector v points in the room after rotating by a.
static V3 world(const Angles& a, V3 v) { return imusim::bodyToWorld({a.yaw, a.pitch, a.roll}) * v; }

static void showNose(const Angles& a)
{
    const V3 n = world(a, {1, 0, 0});
    std::printf("  mũi remote (trục x) chỉ tới:  tới TV %+.2f   trái %+.2f   lên %+.2f\n", n.x, n.y, n.z);
}

static void testTurnLeft()
{
    std::printf("turnLeft(a, 10): mũi remote quay sang TRÁI 10°\n");
    Angles a;
    turnLeft(a, 10);
    printAngles("góc sau khi gọi:", a);
    showNose(a);
    check(a.yaw != 0 || a.pitch != 0 || a.roll != 0, "a đã thay đổi (hàm sửa biến gốc, không phải bản sao)");
    check(world(a, {1, 0, 0}).y > 0.1, "mũi remote đi sang TRÁI (trái > 0)");
    check(near(a.yaw, 10) && near(a.pitch, 0) && near(a.roll, 0), "yaw = +10, pitch = 0, roll = 0");
}

static void testTiltDown()
{
    std::printf("tiltDown(a, 10): mũi remote chúc XUỐNG 10°\n");
    Angles a;
    tiltDown(a, 10);
    printAngles("góc sau khi gọi:", a);
    showNose(a);
    check(a.yaw != 0 || a.pitch != 0 || a.roll != 0, "a đã thay đổi (hàm sửa biến gốc, không phải bản sao)");
    check(world(a, {1, 0, 0}).z < -0.1, "mũi remote đi XUỐNG (lên < 0)");
    check(near(a.yaw, 0) && near(a.pitch, 10) && near(a.roll, 0), "yaw = 0, pitch = +10, roll = 0");
}

static void testTwistRight()
{
    std::printf("twistRight(a, 10): cạnh PHẢI của remote đi XUỐNG 10°\n");
    Angles a;
    twistRight(a, 10);
    printAngles("góc sau khi gọi:", a);
    showNose(a);
    const V3 right = world(a, {0, -1, 0}); // right side = -y
    std::printf("  cạnh phải (−y) chỉ tới:        tới TV %+.2f   trái %+.2f   lên %+.2f\n", right.x, right.y, right.z);
    check(a.yaw != 0 || a.pitch != 0 || a.roll != 0, "a đã thay đổi (hàm sửa biến gốc, không phải bản sao)");
    check(right.z < -0.1, "cạnh phải đi XUỐNG (lên < 0)");
    const V3 n = world(a, {1, 0, 0});
    check(std::fabs(n.y) < 1e-3 && std::fabs(n.z) < 1e-3, "mũi remote đứng yên (vẫn chỉ thẳng vào TV)");
}

static void testLookUp()
{
    std::printf("lookUp(a, 5): mũi remote ngẩng LÊN 5°\n");
    Angles a;
    lookUp(a, 5);
    printAngles("góc sau khi gọi:", a);
    showNose(a);
    check(a.yaw != 0 || a.pitch != 0 || a.roll != 0, "a đã thay đổi");
    check(world(a, {1, 0, 0}).z > 0.05, "mũi remote đi LÊN (lên > 0)");
    check(near(a.yaw, 0) && near(std::fabs(a.pitch), 5) && near(a.roll, 0), "chỉ một góc đổi, đổi đúng 5°");
    Angles b;
    tiltDown(b, 5);
    lookUp(b, 5);
    printAngles("tiltDown 5 rồi lookUp 5:", b);
    check(near(b.yaw, 0) && near(b.pitch, 0) && near(b.roll, 0), "chúc xuống 5° rồi ngẩng lên 5° → về 0");
}

static void testRotateAbout()
{
    std::printf("rotateAbout (stretch): xoay 10° theo chiều DƯƠNG quanh 'x', 'y', 'z'\n");
    const char axes[] = {'x', 'y', 'z'};
    for (char ax : axes) {
        Angles a;
        rotateAbout(a, ax, 10);
        char label[40];
        std::snprintf(label, sizeof label, "rotateAbout(a, '%c', 10):", ax);
        printAngles(label, a);
        // Positive rotation about body axis k: (k+1) axis turns toward (k+2) axis (right-hand rule).
        const int k = ax - 'x';
        V3 from{}, toward{};
        (&from.x)[(k + 1) % 3] = 1;
        (&toward.x)[(k + 2) % 3] = 1;
        const V3 moved = world(a, from);
        const double dot = moved.x * toward.x + moved.y * toward.y + moved.z * toward.z;
        char what[80];
        std::snprintf(what, sizeof what, "quanh '%c': chiều dương đúng quy tắc bàn tay phải, 10°", ax);
        check(std::fabs(dot - std::sin(10 * imusim::kPi / 180)) < 1e-3, what);
    }
    Angles a;
    rotateAbout(a, 'w', 10);
    check(near(a.yaw, 0) && near(a.pitch, 0) && near(a.roll, 0), "ký tự 'w' (không phải trục): không đổi góc nào");
}

int main(int argc, char** argv)
{
    const char* name = argc > 1 ? argv[1] : "";
    if (!std::strcmp(name, "turnLeft")) testTurnLeft();
    else if (!std::strcmp(name, "tiltDown")) testTiltDown();
    else if (!std::strcmp(name, "twistRight")) testTwistRight();
    else if (!std::strcmp(name, "lookUp")) testLookUp();
    else if (!std::strcmp(name, "rotateAbout")) testRotateAbout();
    else {
        std::printf("usage: test_axes <turnLeft|tiltDown|twistRight|lookUp|rotateAbout>\n");
        return 2;
    }
    std::printf("%s\n", failures ? "FAILED" : "PASSED");
    return failures ? 1 : 0;
}
