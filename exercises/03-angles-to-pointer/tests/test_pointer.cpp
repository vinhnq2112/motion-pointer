// Tests for exercise 03. Mentor-owned.
// Unit tests for clampf / anglesToPointer, then onImuSample through the simulator (Session) against the
// ground-truth pointer, computed with the learner's own gain (kGain).
// Usage: test_pointer <integrate|clamp|center|direction|bound|plugin-sweep|plugin-wild|stretch-relative>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#include "imusim_core.h"
#include "runner.h"
#include "student.h"

static int failures = 0;

static void check(bool ok, const char* what)
{
    std::printf("  [%s] %s\n", ok ? " OK " : "FAIL", what);
    if (!ok) ++failures;
}

static bool near(float a, float b, float tol = 1e-3f) { return std::fabs(a - b) < tol; }

static ImuSample sample(float dt, Vec3 gyro) { return {dt, gyro, {0, 0, 9.81f}}; }

static void testIntegrate()
{
    std::printf("integrateGyro (code của bạn từ bài 02)\n");
    Angles a;
    integrateGyro(a, sample(0.01f, {0, 0, 15}));
    integrateGyro(a, sample(0.02f, {30, 10, -5}));
    std::printf("  2 sample → yaw %+.4f  pitch %+.4f\n", a.yaw, a.pitch);
    check(near(a.yaw, 0.05f, 1e-4f), "yaw = 15 × 0.01 + (−5 × 0.02) = 0.05°");
    check(near(a.pitch, 0.2f, 1e-4f), "pitch = 10 × 0.02 = 0.2°");
}

static void testClamp()
{
    std::printf("clampf(v, 0, 1919)\n");
    const float in[] = {-240, 0, 560, 1919, 2500, -0.5f, 1919.5f};
    const float want[] = {0, 0, 560, 1919, 1919, 0, 1919};
    for (int i = 0; i < 7; ++i) {
        const float got = clampf(in[i], 0, 1919);
        char what[80];
        std::snprintf(what, sizeof what, "clampf(%.1f) = %.1f (bạn: %.1f)", in[i], want[i], got);
        check(near(got, want[i]), what);
    }
    const float got = clampf(-5, -10, 10);
    char what[80];
    std::snprintf(what, sizeof what, "clampf(-5, -10, 10) = -5 (bạn: %.1f) · số âm trong vùng giữ nguyên", got);
    check(near(got, -5), what);
}

static void showPointer(const char* label, const PointerState& p)
{
    std::printf("  %-38s → x %8.1f  y %8.1f\n", label, p.x, p.y);
}

static void testCenter()
{
    std::printf("anglesToPointer: góc = 0 → con trỏ ở giữa màn hình\n");
    const Angles zero;
    const PointerState a = anglesToPointer(zero, 40, 1920, 1080);
    showPointer("yaw 0, pitch 0, TV 1920 × 1080", a);
    check(near(a.x, 960) && near(a.y, 540), "TV 1920 × 1080: (960, 540)");
    const PointerState b = anglesToPointer(zero, 40, 1280, 720);
    showPointer("yaw 0, pitch 0, TV 1280 × 720", b);
    check(near(b.x, 640) && near(b.y, 360), "TV 1280 × 720: (640, 360) · không viết thẳng 960 / 540");
}

static void testDirection()
{
    std::printf("anglesToPointer: chiều và độ lớn (TV 1920 × 1080)\n");
    struct Case { float yaw, pitch, gain, x, y; const char* what; };
    const Case cases[] = {
        {+10, 0, 40, 560, 540, "yaw +10° (quay trái), gain 40 → x = 560 (sang trái)"},
        {-5, 0, 40, 1160, 540, "yaw −5° (quay phải), gain 40 → x = 1160 (sang phải)"},
        {0, +3, 40, 960, 660, "pitch +3° (chúc xuống), gain 40 → y = 660 (đi xuống)"},
        {0, -3, 40, 960, 420, "pitch −3° (ngẩng lên), gain 40 → y = 420 (đi lên)"},
        {+10, +3, 20, 760, 600, "yaw +10°, pitch +3°, gain 20 → (760, 600)"},
    };
    for (const Case& c : cases) {
        Angles a;
        a.yaw = c.yaw;
        a.pitch = c.pitch;
        char label[48];
        std::snprintf(label, sizeof label, "yaw %+.0f, pitch %+.0f, gain %.0f", c.yaw, c.pitch, c.gain);
        const PointerState p = anglesToPointer(a, c.gain, 1920, 1080);
        showPointer(label, p);
        check(near(p.x, c.x) && near(p.y, c.y), c.what);
    }
}

static void testBound()
{
    std::printf("anglesToPointer: con trỏ luôn trong màn hình (screen bound)\n");
    struct Case { float yaw, pitch, screenW, screenH, x, y; const char* what; };
    const Case cases[] = {
        {+30, 0, 1920, 1080, 0, 540, "yaw +30° → x = −240 → kẹp ở mép trái 0"},
        {-85, 0, 1920, 1080, 1919, 540, "yaw −85° → kẹp ở mép phải 1919"},
        {0, -40, 1920, 1080, 960, 0, "pitch −40° → kẹp ở mép trên 0"},
        {0, +60, 1920, 1080, 960, 1079, "pitch +60° → kẹp ở mép dưới 1079"},
        {+70, +50, 1280, 720, 0, 719, "TV 1280 × 720, yaw +70°, pitch +50° → góc dưới trái (0, 719)"},
    };
    for (const Case& c : cases) {
        Angles a;
        a.yaw = c.yaw;
        a.pitch = c.pitch;
        char label[48];
        std::snprintf(label, sizeof label, "yaw %+.0f, pitch %+.0f, TV %.0f × %.0f", c.yaw, c.pitch, c.screenW,
                      c.screenH);
        const PointerState p = anglesToPointer(a, 40, c.screenW, c.screenH);
        showPointer(label, p);
        check(near(p.x, c.x) && near(p.y, c.y), c.what);
    }
}

struct RunStats {
    double errMax = 0;
    int offScreen = 0, samples = 0;
    float minX = 1e9f, maxX = -1e9f, minY = 1e9f, maxY = -1e9f;
    double truthMinX = 1e9, truthMaxX = -1e9, truthMinY = 1e9, truthMaxY = -1e9;
};

// Runs scenarios through onImuSample (Session), one after the other. Prints one row per second if table.
static RunStats runPlugin(std::initializer_list<const char*> names, bool table)
{
    imusim::Knobs k;
    k.pointerGain = kGain > 0 ? kGain : 40;
    Session s(k);
    RunStats r;
    // Header written by hand: printf widths count bytes, and Vietnamese letters take 2–3 bytes.
    if (table) std::printf("   t (s)   x đúng    x bạn   y đúng    y bạn   sai số (px)\n");
    for (const char* name : names) {
        const imusim::Scenario* sc = imusim::findScenario(name);
        const double dt = 1.0 / k.sampleRate;
        const int n = (int)std::lround(sc->duration() * k.sampleRate);
        if (table) std::printf("  scenario %s\n", name);
        for (int i = 0; i < n; ++i) {
            const double tEnd = (i + 1) * dt, tMid = tEnd - dt / 2;
            s.step(sc->angles(tMid), sc->rates(tMid), sc->angles(tEnd));
            const PointerState& p = s.pointer;
            const double err = std::hypot(p.x - s.truth.x, p.y - s.truth.y);
            r.errMax = std::max(r.errMax, err);
            ++r.samples;
            if (!(p.x >= 0 && p.y >= 0 && p.x <= k.screenW - 1 && p.y <= k.screenH - 1)) ++r.offScreen;
            r.minX = std::min(r.minX, p.x), r.maxX = std::max(r.maxX, p.x);
            r.minY = std::min(r.minY, p.y), r.maxY = std::max(r.maxY, p.y);
            r.truthMinX = std::min(r.truthMinX, s.truth.x), r.truthMaxX = std::max(r.truthMaxX, s.truth.x);
            r.truthMinY = std::min(r.truthMinY, s.truth.y), r.truthMaxY = std::max(r.truthMaxY, s.truth.y);
            if (table && (i + 1) % (int)std::lround(k.sampleRate) == 0)
                std::printf("  %6.1f  %7.1f  %7.1f  %7.1f  %7.1f  %8.1f\n", tEnd, s.truth.x, p.x, s.truth.y, p.y,
                            err);
        }
    }
    return r;
}

static void testPluginSweep()
{
    std::printf("onImuSample: yaw-sweep rồi pitch-sweep qua simulator, so với con trỏ đúng (gain = kGain)\n");
    char what[112];
    std::snprintf(what, sizeof what, "kGain = %.1f px/° nằm trong khoảng 10 → 200 (YOU DECIDE)", kGain);
    check(kGain >= 10 && kGain <= 200, what);
    const RunStats r = runPlugin({"yaw-sweep", "pitch-sweep"}, true);
    std::printf("  sai số con trỏ lớn nhất: %.2f px\n", r.errMax);
    check(r.maxX - r.minX > 1 && r.maxY - r.minY > 1, "con trỏ có di chuyển (cả x và y)");
    check(r.errMax < 5, "con trỏ của bạn luôn cách con trỏ đúng < 5 px (đúng chiều, đúng gain)");
}

static void testPluginWild()
{
    std::printf("onImuSample: scenario wild (vung tay mạnh, góc rất lớn)\n");
    const RunStats r = runPlugin({"wild"}, false);
    std::printf("  x của bạn: %.1f → %.1f · y của bạn: %.1f → %.1f\n", r.minX, r.maxX, r.minY, r.maxY);
    std::printf("  x đúng:    %.1f → %.1f · y đúng:    %.1f → %.1f\n", r.truthMinX, r.truthMaxX, r.truthMinY,
                r.truthMaxY);
    char what[112];
    std::snprintf(what, sizeof what, "sample ngoài màn hình: %d / %d (phải bằng 0)", r.offScreen, r.samples);
    check(r.samples > 0 && r.offScreen == 0, what);
    // With a small gain the true pointer may never reach an edge: only ask for the edges the truth touches.
    const bool left = r.truthMinX > 0.5 || r.minX <= 0.5f, right = r.truthMaxX < 1918.5 || r.maxX >= 1918.5f;
    const bool top = r.truthMinY > 0.5 || r.minY <= 0.5f, bottom = r.truthMaxY < 1078.5 || r.maxY >= 1078.5f;
    check(left && right && top && bottom,
          "con trỏ chạm mép giống con trỏ đúng (kẹp ở mép, không dừng trước mép)");
}

static void testStretchRelative()
{
    std::printf("Stretch: moveXByChange (TV rộng 1920, gain 40)\n");
    const float one = moveXByChange(960, +1, 40, 1920);
    std::printf("  x = 960, dYaw = +1° → x mới %.1f\n", one);
    check(near(one, 920), "quay trái 1° → con trỏ sang trái 40 px");
    float x = 960, want = 960;
    bool inside = true;
    for (int i = 0; i < 60; ++i) {
        const float d = i < 30 ? +1.0f : -1.0f; // 30 sample: yaw 0 → +30°, rồi 30 sample: +30° → 0
        x = moveXByChange(x, d, 40, 1920);
        want = std::clamp(want - 40 * d, 0.0f, 1919.0f);
        if (x < 0 || x > 1919) inside = false;
        if (i == 29) std::printf("  yaw = +30° → x của bạn %.1f\n", x);
    }
    std::printf("  yaw về 0° → x của bạn %.1f\n", x);
    check(inside, "con trỏ luôn trong màn hình");
    check(near(x, want), "x lúc cuối khớp với cách 'cộng phần thay đổi rồi kẹp' (so với số bạn đoán trong README)");
}

int main(int argc, char** argv)
{
    const char* name = argc > 1 ? argv[1] : "";
    if (!std::strcmp(name, "integrate")) testIntegrate();
    else if (!std::strcmp(name, "clamp")) testClamp();
    else if (!std::strcmp(name, "center")) testCenter();
    else if (!std::strcmp(name, "direction")) testDirection();
    else if (!std::strcmp(name, "bound")) testBound();
    else if (!std::strcmp(name, "plugin-sweep")) testPluginSweep();
    else if (!std::strcmp(name, "plugin-wild")) testPluginWild();
    else if (!std::strcmp(name, "stretch-relative")) testStretchRelative();
    else {
        std::printf("usage: test_pointer <integrate|clamp|center|direction|bound|plugin-sweep|plugin-wild|"
                    "stretch-relative>\n");
        return 2;
    }
    std::printf("%s\n", failures ? "FAILED" : "PASSED");
    return failures ? 1 : 0;
}
