// imu-sim: window app. Left: the remote in 3D with its axes, rotation axis and motion direction.
// Right: the virtual TV with a menu and the pointer moved by the learner's onImuSample.
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

#include "raylib.h"
#include "rlgl.h"
#include "runner.h"

using imusim::Euler;
using imusim::M3;
using imusim::V3;

namespace {

const int kWinW = 1280, kWinH = 720;
const int kViewW = 640, kViewH = 520;
const Rectangle kTv = {660, 52, 600, 337.5f};

const Color kBg = {16, 17, 21, 255};
const Color kText = {225, 228, 235, 255};
const Color kDim = {140, 146, 160, 255};
const Color kAxisX = {235, 80, 80, 255};
const Color kAxisY = {80, 205, 100, 255};
const Color kAxisZ = {90, 150, 255, 255};
const Color kSpin = {250, 205, 50, 255};    // rotation axis
const Color kMove = {245, 245, 245, 255};   // motion direction

const char* kTiles[12] = {"Phim", "Nhạc", "Ảnh", "Thể thao", "Tin tức", "Trò chơi",
                          "Cài đặt", "Lịch", "Thời tiết", "Bản đồ", "Ứng dụng", "Tìm kiếm"};

struct UiFont { Font font; float size; };
UiFont gBig, gSmall;

V3 add(V3 a, V3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
V3 sub(V3 a, V3 b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
V3 mul(V3 a, double s) { return {a.x * s, a.y * s, a.z * s}; }
double dot(V3 a, V3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
V3 cross(V3 a, V3 b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }
double len(V3 a) { return std::sqrt(dot(a, a)); }
V3 unit(V3 a) { const double l = len(a); return l > 1e-12 ? mul(a, 1.0 / l) : V3{}; }

// World frame (x toward TV, y left, z up) → raylib frame (X right, Y up, Z toward the viewer).
Vector3 toRay(V3 w) { return {(float)-w.y, (float)w.z, (float)-w.x}; }

std::vector<int> uiCodepoints()
{
    std::vector<int> cps;
    for (int c = 32; c < 127; ++c) cps.push_back(c);
    for (int c = 160; c <= 383; ++c) cps.push_back(c);   // Latin-1 + Latin Extended-A (°, ², ×, đ)
    for (int c : {416, 417, 431, 432, 0x2013, 0x2014, 0x2022, 0x2190, 0x2191, 0x2192, 0x2193}) cps.push_back(c);
    for (int c = 0x1EA0; c <= 0x1EF9; ++c) cps.push_back(c); // Vietnamese
    return cps;
}

UiFont loadFont(int size)
{
    std::vector<int> cps = uiCodepoints();
    for (const char* path : {"C:/Windows/Fonts/arial.ttf", "C:/Windows/Fonts/segoeui.ttf"}) {
        if (!FileExists(path)) continue;
        Font f = LoadFontEx(path, size, cps.data(), (int)cps.size());
        if (f.texture.id != 0) {
            SetTextureFilter(f.texture, TEXTURE_FILTER_BILINEAR);
            return {f, (float)size};
        }
    }
    return {GetFontDefault(), (float)size};
}

void text(const UiFont& f, const std::string& s, float x, float y, Color c)
{
    DrawTextEx(f.font, s.c_str(), {x, y}, f.size, 1, c);
}

float textWidth(const UiFont& f, const std::string& s) { return MeasureTextEx(f.font, s.c_str(), f.size, 1).x; }

// Box in body coordinates (centre c, half sizes h), rotated by R, with simple light shading.
void drawBox(const M3& R, V3 c, V3 h, Color col)
{
    const V3 light = unit({-0.5, -0.4, 0.9});
    const double hs[3] = {h.x, h.y, h.z};
    for (int a = 0; a < 3; ++a) {
        for (int s = -1; s <= 1; s += 2) {
            V3 axis[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
            const V3 n = mul(axis[a], s);
            const V3 u = mul(axis[(a + 1) % 3], hs[(a + 1) % 3]);
            const V3 v = mul(axis[(a + 2) % 3], hs[(a + 2) % 3]);
            const V3 fc = add(c, mul(n, hs[a]));
            const V3 p[4] = {sub(sub(fc, u), v), sub(add(fc, u), v), add(add(fc, u), v), add(sub(fc, u), v)};
            const double shade = 0.45 + 0.55 * std::max(0.0, dot(R * n, light));
            const Color fcol = {(unsigned char)(col.r * shade), (unsigned char)(col.g * shade),
                                (unsigned char)(col.b * shade), 255};
            Vector3 w[4];
            for (int i = 0; i < 4; ++i) w[i] = toRay(R * p[i]);
            DrawTriangle3D(w[0], w[1], w[2], fcol);
            DrawTriangle3D(w[0], w[2], w[3], fcol);
        }
    }
}

void drawArrow(V3 from, V3 to, float r, Color c)
{
    const V3 d = sub(to, from);
    const double l = len(d);
    if (l < 1e-6) return;
    const double head = std::min(0.2, l * 0.4);
    const V3 base = add(from, mul(d, (l - head) / l));
    DrawCylinderEx(toRay(from), toRay(base), r, r, 12, c);
    DrawCylinderEx(toRay(base), toRay(to), r * 2.8f, 0.0f, 12, c);
}

// Curved arrow around `axis`: shows the direction of rotation (right-hand rule).
void drawSpinArc(V3 centre, V3 axis, double radius, Color c)
{
    const V3 a = unit(axis);
    const V3 helper = std::fabs(a.z) < 0.9 ? V3{0, 0, 1} : V3{1, 0, 0};
    const V3 u = unit(cross(helper, a));
    const V3 v = cross(a, u);
    const int n = 30;
    const double end = 1.5 * imusim::kPi;
    V3 prev = add(centre, mul(u, radius));
    for (int i = 1; i <= n; ++i) {
        const double ang = end * i / n;
        const V3 p = add(centre, add(mul(u, radius * std::cos(ang)), mul(v, radius * std::sin(ang))));
        DrawCylinderEx(toRay(prev), toRay(p), 0.014f, 0.014f, 6, c);
        prev = p;
    }
    const V3 tangent = add(mul(u, -std::sin(end)), mul(v, std::cos(end)));
    DrawCylinderEx(toRay(prev), toRay(add(prev, mul(tangent, 0.14))), 0.045f, 0.0f, 10, c);
}

Vector2 project(V3 w, const Camera3D& cam) { return GetWorldToScreenEx(toRay(w), cam, kViewW, kViewH); }

struct Tile { Rectangle r; const char* name; };

std::vector<Tile> menuTiles(const imusim::Knobs& k)
{
    std::vector<Tile> tiles;
    const float x0 = 0.08f * k.screenW, x1 = 0.92f * k.screenW;
    const float y0 = 0.20f * k.screenH, y1 = 0.92f * k.screenH;
    const float w = (x1 - x0) / 4, h = (y1 - y0) / 3, gap = 0.012f * k.screenW;
    for (int i = 0; i < 12; ++i) {
        const int col = i % 4, row = i / 4;
        tiles.push_back({{x0 + col * w + gap, y0 + row * h + gap, w - 2 * gap, h - 2 * gap}, kTiles[i]});
    }
    return tiles;
}

std::string fmt(const char* f, double a, double b, double c) { return TextFormat(f, a, b, c); }

} // namespace

int main(int argc, char** argv)
{
    Options opt;
    std::string error;
    if (!parseOptions(argc, argv, opt, error)) {
        std::fprintf(stderr, "error: %s\n%s", error.c_str(), optionsHelp());
        return 2;
    }
    if (opt.help) {
        std::printf("imu-sim: window app.\n%s", optionsHelp());
        return 0;
    }
    const bool shotMode = !opt.screenshotPath.empty();

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(kWinW, kWinH, "imu-sim");
    SetTargetFPS(60);
    gBig = loadFont(20);
    gSmall = loadFont(17);
    RenderTexture2D view = LoadRenderTexture(kViewW, kViewH);

    Camera3D cam{};
    cam.position = toRay({-3.6, -2.5, 1.9});
    cam.target = toRay({0.45, 0, -0.1});
    cam.up = {0, 1, 0};
    cam.fovy = 42;
    cam.projection = CAMERA_PERSPECTIVE;

    imusim::Knobs knobs = opt.knobs;
    Session session(knobs);
    Euler manual, target;
    int scenarioIdx = -1;
    double scenarioT = 0;
    if (opt.scenario != "all") {
        const auto& list = imusim::scenarios();
        for (size_t i = 0; i < list.size(); ++i)
            if (list[i].name == opt.scenario) scenarioIdx = (int)i;
    }

    bool paused = false, showAxis = true, showTruth = true;
    double acc = 0;
    V3 spinAxisWorld{};
    double spinRate = 0, spinHold = 0;
    int selected = -1;
    double selectedTimer = 0;

    const double noiseSteps[] = {0, 0.5, 1, 2};
    const double biasSteps[] = {0, 0.2, 0.5, 1};
    const double tremorSteps[] = {0, 0.1, 0.3};
    auto cycle = [](double cur, const double* steps, int n) {
        for (int i = 0; i < n; ++i)
            if (std::fabs(steps[i] - cur) < 1e-9) return steps[(i + 1) % n];
        return steps[0];
    };
    auto stopScenario = [&]() {
        if (scenarioIdx >= 0) {
            manual = target = session.intended;
            scenarioIdx = -1;
        }
    };
    auto startScenario = [&](int i) {
        manual = target = Euler{};
        session = Session(knobs);
        scenarioIdx = i;
        scenarioT = 0;
    };

    while (!WindowShouldClose()) {
        const double frameDt = shotMode ? 1.0 / 60.0 : std::min((double)GetFrameTime(), 0.1);

        // ---- input ----
        if (!shotMode) {
            const Vector2 md = GetMouseDelta();
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && (md.x != 0 || md.y != 0)) {
                stopScenario();
                target.yaw -= md.x * 0.15;    // drag right → nose right → yaw negative
                target.pitch += md.y * 0.15;  // drag down → nose down → pitch positive
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && md.x != 0) {
                stopScenario();
                target.roll += md.x * 0.3;    // drag right → right side down → roll positive
            }
            const double k = 60.0 * frameDt;
            auto key = [&](int code, double& v, double d) {
                if (IsKeyDown(code)) { stopScenario(); v += d; }
            };
            key(KEY_LEFT, target.yaw, k);
            key(KEY_RIGHT, target.yaw, -k);
            key(KEY_UP, target.pitch, -k);
            key(KEY_DOWN, target.pitch, k);
            key(KEY_Q, target.roll, -k);
            key(KEY_E, target.roll, k);
            target.pitch = std::clamp(target.pitch, -80.0, 80.0);
            target.roll = std::clamp(target.roll, -170.0, 170.0);

            for (int i = 0; i < (int)imusim::scenarios().size() && i < 9; ++i)
                if (IsKeyPressed(KEY_ONE + i)) startScenario(i);
            if (IsKeyPressed(KEY_R)) {
                scenarioIdx = -1;
                manual = target = Euler{};
                session = Session(knobs);
            }
            if (IsKeyPressed(KEY_N)) { knobs.noise = cycle(knobs.noise, noiseSteps, 4); session.setKnobs(knobs); }
            if (IsKeyPressed(KEY_B)) { knobs.gyroBias = cycle(knobs.gyroBias, biasSteps, 4); session.setKnobs(knobs); }
            if (IsKeyPressed(KEY_T)) {
                knobs.handTremor = cycle(knobs.handTremor, tremorSteps, 3);
                session.setKnobs(knobs);
            }
            if (IsKeyPressed(KEY_A)) showAxis = !showAxis;
            if (IsKeyPressed(KEY_G)) showTruth = !showTruth;
            if (IsKeyPressed(KEY_P)) paused = !paused;
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                selected = -1;
                const auto tiles = menuTiles(knobs);
                for (int i = 0; i < (int)tiles.size(); ++i)
                    if (CheckCollisionPointRec({session.pointer.x, session.pointer.y}, tiles[i].r)) selected = i;
                selectedTimer = 1.5;
            }
        }

        // ---- simulation: fixed steps at sampleRate ----
        if (!paused) {
            const double dt = 1.0 / knobs.sampleRate;
            acc += frameDt;
            while (acc >= dt) {
                acc -= dt;
                if (scenarioIdx >= 0) {
                    const imusim::Scenario& sc = imusim::scenarios()[scenarioIdx];
                    const double tMid = scenarioT + dt / 2, tEnd = scenarioT + dt;
                    session.step(sc.angles(tMid), sc.rates(tMid), sc.angles(tEnd));
                    scenarioT = tEnd;
                    if (scenarioT >= sc.duration()) {
                        manual = target = sc.angles(sc.duration());
                        scenarioIdx = -1;
                    }
                } else {
                    Euler rate = (target - manual) * 12.0;
                    rate.yaw = std::clamp(rate.yaw, -400.0, 400.0);
                    rate.pitch = std::clamp(rate.pitch, -400.0, 400.0);
                    rate.roll = std::clamp(rate.roll, -400.0, 400.0);
                    session.step(manual + rate * (dt / 2), rate, manual + rate * dt);
                    manual = manual + rate * dt;
                }
            }
        }
        selectedTimer = std::max(0.0, selectedTimer - frameDt);

        const M3 R = imusim::bodyToWorld(session.actual);
        const V3 w = session.omegaBody;
        const double wLen = len(w);
        if (wLen > 3.0) {
            spinAxisWorld = unit(R * w);
            spinRate = wLen;
            spinHold = 0.6;
        } else {
            spinHold = std::max(0.0, spinHold - frameDt);
        }

        // ---- 3D view ----
        BeginTextureMode(view);
        ClearBackground({24, 26, 33, 255});
        BeginMode3D(cam);
        rlPushMatrix();
        rlTranslatef(0, -0.9f, 0);
        DrawGrid(24, 0.5f);
        rlPopMatrix();

        rlDisableBackfaceCulling();
        drawBox(R, {0, 0, 0}, {0.8, 0.22, 0.08}, {78, 82, 94, 255});           // remote body
        drawBox(R, {0.32, 0, 0.09}, {0.08, 0.08, 0.015}, {235, 235, 240, 255}); // OK button (on top)
        drawBox(R, {0.62, 0.12, 0.09}, {0.035, 0.035, 0.012}, {220, 50, 50, 255}); // power button
        drawBox(R, {0.81, 0, 0}, {0.012, 0.16, 0.055}, {25, 25, 28, 255});     // front window (points at TV)
        rlEnableBackfaceCulling();

        const V3 tipX = R * V3{1.5, 0, 0}, tipY = R * V3{0, 0.95, 0}, tipZ = R * V3{0, 0, 0.85};
        drawArrow({}, tipX, 0.025f, kAxisX);
        drawArrow({}, tipY, 0.025f, kAxisY);
        drawArrow({}, tipZ, 0.025f, kAxisZ);

        const bool showSpin = showAxis && spinHold > 0;
        V3 moveFrom{}, moveTo{};
        bool showMove = false;
        if (showSpin) {
            Color c = kSpin;
            c.a = (unsigned char)(255 * std::min(1.0, spinHold / 0.3));
            const V3 a = spinAxisWorld;
            DrawCylinderEx(toRay(mul(a, -1.2)), toRay(mul(a, 1.0)), 0.012f, 0.012f, 8, c);
            drawArrow(mul(a, 1.0), mul(a, 1.3), 0.02f, c);
            drawSpinArc(mul(a, 0.95), a, 0.24, c);

            // Motion direction of the point that moves fastest (nose, or the y-arrow tip when only rolling).
            if (wLen > 3.0) {
                const V3 wRad = mul(R * w, imusim::kPi / 180.0);
                const V3 nose = R * V3{0.8, 0, 0};
                const V3 side = tipY;
                const V3 vNose = cross(wRad, nose), vSide = cross(wRad, side);
                const bool useNose = len(vNose) >= 0.25 * len(vSide); // prefer the nose
                const V3 p = useNose ? nose : side;
                const V3 v = useNose ? vNose : vSide;
                if (len(v) > 0.05) {
                    moveFrom = p;
                    moveTo = add(p, mul(unit(v), std::clamp(len(v) * 0.5, 0.3, 0.8)));
                    drawArrow(moveFrom, moveTo, 0.022f, kMove);
                    showMove = true;
                }
            }
        }
        EndMode3D();

        auto label = [&](V3 p, const char* s, Color c) {
            const Vector2 sp = project(p, cam);
            text(gBig, s, sp.x + 6, sp.y - 10, c);
        };
        label(tipX, "x", kAxisX);
        label(tipY, "y", kAxisY);
        label(tipZ, "z", kAxisZ);
        if (showSpin) label(mul(spinAxisWorld, 1.35), "trục xoay", kSpin);
        if (showMove) label(moveTo, "hướng chuyển động", kMove);
        text(gSmall, "Remote: x = tới TV · y = sang trái · z = lên trên", 12, 10, kText);
        if (showAxis)
            text(gSmall, "Vàng = trục xoay (vật quay quanh nó) · Trắng = hướng chuyển động", 12, 32, kDim);
        else
            text(gSmall, "Trục xoay đang ẩn (A để hiện)", 12, 32, kDim);
        EndTextureMode();

        // ---- 2D screen ----
        BeginDrawing();
        ClearBackground(kBg);
        DrawTextureRec(view.texture, {0, 0, (float)kViewW, -(float)kViewH}, {0, 0}, WHITE);

        // TV
        const float s = kTv.width / knobs.screenW;
        const Rectangle bezel = {kTv.x - 8, kTv.y - 8, kTv.width + 16, kTv.height + 16};
        DrawRectangleRounded(bezel, 0.03f, 8, {38, 38, 44, 255});
        DrawRectangleRec(kTv, {14, 20, 36, 255});
        text(gSmall, TextFormat("TV ảo %d×%d px", knobs.screenW, knobs.screenH), kTv.x, 16, kDim);
        text(gBig, "Menu", kTv.x + 0.08f * kTv.width + 4, kTv.y + 0.07f * kTv.height, kText);
        const auto tiles = menuTiles(knobs);
        for (int i = 0; i < (int)tiles.size(); ++i) {
            const Rectangle tr = {kTv.x + tiles[i].r.x * s, kTv.y + tiles[i].r.y * s, tiles[i].r.width * s,
                                  tiles[i].r.height * s};
            const bool hover = CheckCollisionPointRec({session.pointer.x, session.pointer.y}, tiles[i].r);
            Color fill = hover ? Color{50, 95, 170, 255} : Color{32, 42, 66, 255};
            if (i == selected && selectedTimer > 0) fill = {40, 150, 80, 255};
            DrawRectangleRounded(tr, 0.12f, 6, fill);
            const float tw = textWidth(gSmall, tiles[i].name);
            text(gSmall, tiles[i].name, tr.x + (tr.width - tw) / 2, tr.y + tr.height / 2 - 9, kText);
        }
        if (showTruth) {
            const Vector2 tp = {kTv.x + (float)session.truth.x * s, kTv.y + (float)session.truth.y * s};
            DrawCircleLinesV(tp, 9, kSpin);
            DrawCircleLinesV(tp, 10, kSpin);
        }
        const bool off = session.pointer.x < 0 || session.pointer.y < 0 || session.pointer.x > knobs.screenW - 1 ||
                         session.pointer.y > knobs.screenH - 1;
        const float pxs = std::clamp(session.pointer.x, 0.0f, (float)knobs.screenW - 1);
        const float pys = std::clamp(session.pointer.y, 0.0f, (float)knobs.screenH - 1);
        const Vector2 pp = {kTv.x + pxs * s, kTv.y + pys * s};
        DrawCircleV(pp, 7, off ? Color{230, 60, 60, 255} : WHITE);
        DrawCircleLinesV(pp, 7, BLACK);
        if (off) text(gSmall, "Con trỏ ra ngoài màn hình!", kTv.x + 8, kTv.y + kTv.height - 26, {255, 90, 90, 255});

        // Numbers
        float y = 410;
        const float x = 660;
        text(gSmall, "Góc thật (ground truth), °", x, y, kDim);
        text(gBig, fmt("yaw %+7.1f   pitch %+7.1f   roll %+7.1f", session.actual.yaw, session.actual.pitch,
                       session.actual.roll), x, y + 18, kText);
        y += 48;
        text(gSmall, "Dữ liệu IMU gửi cho code của bạn", x, y, kDim);
        if (showAxis)
            text(gBig, fmt("gyro  °/s     x %+7.1f  y %+7.1f  z %+7.1f", session.sample.gyro.x, session.sample.gyro.y,
                           session.sample.gyro.z), x, y + 18, kText);
        else
            text(gBig, "gyro  °/s     (đang ẩn)", x, y + 18, kText);
        text(gBig, fmt("accel m/s²  x %+7.2f  y %+7.2f  z %+7.2f", session.sample.accel.x, session.sample.accel.y,
                       session.sample.accel.z), x, y + 42, kText);
        y += 74;
        const double err = std::hypot(session.pointer.x - session.truth.x, session.pointer.y - session.truth.y);
        text(gSmall, TextFormat("Con trỏ của bạn (trắng) %5.0f, %5.0f   ·   đúng (vòng vàng) %5.0f, %5.0f   ·   sai số %.0f px",
                                session.pointer.x, session.pointer.y, session.truth.x, session.truth.y, err),
             x, y, kText);
        y += 22;
        text(gSmall, TextFormat("Plugin: %s", pluginName()), x, y, kDim);
        if (selectedTimer > 0)
            text(gSmall, selected >= 0 ? TextFormat("Đã chọn: %s", kTiles[selected]) : "Đã bấm OK: không trúng ô nào",
                 x + 300, y, {120, 220, 140, 255});

        // Status + help
        DrawRectangle(0, kViewH, kWinW, kWinH - kViewH, {22, 23, 29, 255});
        float hy = kViewH + 12;
        std::string mode;
        if (paused) mode = "TẠM DỪNG (P)";
        else if (scenarioIdx >= 0) {
            const auto& sc = imusim::scenarios()[scenarioIdx];
            mode = TextFormat("Scenario %d: %s  %.1f / %.0f s — %s", scenarioIdx + 1, sc.name.c_str(), scenarioT,
                              sc.duration(), sc.description.c_str());
        } else mode = "Điều khiển bằng tay (chuột / phím)";
        text(gBig, mode, 12, hy, kSpin);
        hy += 30;
        text(gSmall, "Kéo chuột trái: ngang = yaw, dọc = pitch  ·  Kéo chuột phải ngang = roll  ·  "
                     "Phím ← → = yaw, ↑ ↓ = pitch, Q / E = roll", 12, hy, kText);
        hy += 22;
        std::string sc = "Scenario:";
        for (size_t i = 0; i < imusim::scenarios().size(); ++i)
            sc += TextFormat("  %d %s", (int)i + 1, imusim::scenarios()[i].name.c_str());
        text(gSmall, sc, 12, hy, kText);
        hy += 22;
        text(gSmall, "R đặt lại  ·  Space / Enter = nút OK (chọn ô)  ·  A ẩn/hiện trục xoay  ·  "
                     "G ẩn/hiện con trỏ đúng  ·  P tạm dừng", 12, hy, kText);
        hy += 30;
        text(gSmall, TextFormat("Knobs:  N noise %.1f °/s   ·   B gyroBias %.1f °/s   ·   T handTremor %.1f°   ·   "
                                "sampleRate %.0f Hz   ·   screen %d×%d",
                                knobs.noise, knobs.gyroBias, knobs.handTremor, knobs.sampleRate, knobs.screenW,
                                knobs.screenH),
             12, hy, kDim);

        if (shotMode && session.time >= opt.shotAt) {
            rlDrawRenderBatchActive();
            Image img = LoadImageFromScreen();
            // With Windows display scaling, the read area can be larger than the window: keep the window part.
            if (img.width > GetRenderWidth() || img.height > GetRenderHeight())
                ImageCrop(&img, {0, (float)(img.height - GetRenderHeight()), (float)GetRenderWidth(),
                                 (float)GetRenderHeight()});
            ExportImage(img, opt.screenshotPath.c_str());
            UnloadImage(img);
            EndDrawing();
            break;
        }
        EndDrawing();
    }

    UnloadRenderTexture(view);
    if (gBig.font.texture.id != GetFontDefault().texture.id) UnloadFont(gBig.font);
    if (gSmall.font.texture.id != GetFontDefault().texture.id) UnloadFont(gSmall.font);
    CloseWindow();
    return 0;
}
