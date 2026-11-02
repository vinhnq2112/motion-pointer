# Showcase

Raw material for the final deliverable (`/present`): PowerPoint tiếng Anh, 60 phút, cho đồng nghiệp beginner.
One entry per finished topic, added at session end. The key message is written by the learner, in their own words.

<!-- Entry format:
## <topic> — <date>
- **Key message:** <one sentence the audience should remember>
- **Visual:** <screenshot / diagram path, or what to capture from imu-sim>
- **Evidence:** <a real number or result, e.g. "drift after 60 s: 14° → 0.8° with bias calibration">
- **Demo idea:** <what to show live, or none>
- **Audience quiz:** (optional) <short questions to ask the audience during the talk, with answers>
-->

## Rotation axis vs motion direction (yaw, pitch, roll) — 2026-10-27
- **Key message:** The rotation axis and the motion direction are different: the axis is perpendicular to the motion.
  Tip: the rotation axis is the stick that stays still.
  (Learner's words: "They are different. Trục xoay vuông góc với hướng chuyển động.")
- **Visual:** `imu-sim --mystery`, phím A: remote xoay với mũi tên trục xoay. Chụp 3 ảnh: roll (mũi đứng yên), yaw, pitch.
  Thêm bảng: `turnLeft` → +z, `tiltDown` → +y, `twistRight` → +x.
- **Evidence:** exercise 01: 5/5 test pass; 3/3 chuyển động bí ẩn chỉ đúng trục. Lỗi hay gặp: "con trỏ đi lên" → chọn x (sai), đúng là y.
- **Demo idea:** chạy chuyển động bí ẩn 1 (roll), hỏi khán giả "trục nào?" trước khi nhấn A.

## Gyro: from rotation speed to angle (integration) — 2026-10-29
- **Key message:** A gyro gives the rotation speed, not the angle: new angle = old angle + rotation speed × dt.
  (Learner's words: "gyro cho tốc độ quay của góc, góc mới = góc cũ + tốc độ quay * dt")
- **Visual:** angle chart in `imu-sim` (key C), true angle vs computed angle:
  `imu-sim.exe --scenario yaw-sweep --screenshot chart.png --at 6` (with the exercise 02 plug-in).
  Plus a table: "turning, at 10°: gyro 15 °/s · held still at 20°: gyro 0 °/s".
- **Evidence:** exercise 02: 6/6 tests, max error 0.000° (yaw-sweep, pitch-sweep, 50/100/200 Hz).
  Forgetting `dt`: 2000° instead of 20°. Bias 0.5 °/s for 30 s → 15° drift (predicted 15°, measured 15.00°)
  = 600 px of pointer drift while the remote lies still.
- **Demo idea:** run `yaw-sweep`, press R in the middle: the computed line stays offset forever →
  "integration only knows changes, never the true angle" → lead-in to the drift section.
- **Audience quiz:** (from `/quiz` 2026-10-30; the learner answered 3/3)
  1. *Compute the angle.* Start at yaw = 0°. Turn at `gyro.z = 30` °/s for 0.5 s, 100 Hz,
     code `yaw += gyro.z * dt`. Yaw after 0.5 s? → **15°** (30 × 0.5; or 50 samples × 0.3°).
  2. *Does an error go away?* Remote lies still. One bad sample adds 5° by mistake. Then turn left 20°
     and right 20°, back to the start (true yaw = 0°). Computed yaw? → **5°**: integration only adds
     changes, so the error stays forever.
  3. *Find the bug.* `yaw += gyro.z * 0.01f;` — the simulator now runs at 200 Hz. Turn at 30 °/s
     for 0.5 s (true yaw = 15°). Computed yaw? → **30°**: `dt` is hard-coded as 0.01 s, but at 200 Hz
     it is 0.005 s. Fix: use `s.dt`.

## From angle to pointer + screen bound (gain, clamp) — 2026-11-01
- **Key message:** Gain turns an angle into pixels (how many pixels per degree); clamp keeps the pointer on the screen.
  (Learner's words: "đổi góc thành con trỏ dùng gain. Nghĩa là 1 góc di chuyển tương ứng với bao nhiêu pixel. Giữ con trỏ
  trong màn hình thì dùng clamp")
- **Visual:** screen coordinates diagram: (0,0) top-left, y grows down, centre (960, 540).
  Formulas `x = 960 − gain × yaw`, `y = 540 + gain × pitch` with "why − / why +".
  Number line with clamp: −240 → 0, 560 → 560, 2500 → 1919.
  Screenshot: `imu-sim.exe --gain 40 --scenario wild --screenshot wild.png --at 1.6` (pointer stuck at the edge).
- **Evidence:** exercise 03: 7/7 tests, pointer error 0.01 px (yaw/pitch sweeps), 0 of 1000 samples off-screen in `wild`;
  headless: off-screen 0 in all 6 scenarios. Gain choice: 20 px/° → 48° to reach the edge · 40 → 24°, 0.5° tremor = 20 px ·
  80 → 12°, tremor = 40 px. Chose 40.
  Limit of the simple model: after `wild`, angle error 21.75° (pointer at 1830, 332 instead of 960, 540), because
  `yaw += gyro.z × dt` ignores tilt. Body-rate → Euler-rate conversion: 2.32° (`figure`: 1.94° → 0.01°). → "next step" slide.
- **Demo idea:** run `wild` with the gain-40 plug-in: the pointer hits the edges but never leaves the TV.
  Then show the stretch result: "add the change, then clamp" leaves a 240 px offset → lead-in to Day 6 (edge fixes drift).
- **Audience quiz:** (from the Day 3 lesson and exercise 03; the learner answered all of them)
  1. *Which sign?* Yaw +10° (nose turns left), gain 40, centre x = 960. New x? → **560**: yaw > 0 is left, screen x grows
     to the right, so subtract.
  2. *Off the screen?* Yaw +30°, gain 40. x? Still on a 1920-wide TV? → **−240, no** → clamp to 0.
     (Common slip: using 1080, the screen height, as the centre.)
  3. *Does clamp leave an error?* Yaw +30° (pointer clamped at 0), then back to 0°. Pointer recomputed from the angle every
     sample. Final x? → **960**: nothing is remembered.
  4. *Same, but "add the change, then clamp".* Final x? → **1200**: 240 px of motion were lost at the edge, so the pointer is
     now 240 px away from where the remote points.

## Noise, bias and drift — 2026-11-02 (in progress; key message, visual, evidence added at session end)
- **Audience quiz:** (from `/quiz` 2026-11-02)
  1. *Lost samples.* Start at yaw 0°. Turn left 30°, but 20 samples are lost, so the code adds only 25°. Turn right back to
     the true 0° (no samples lost). Computed yaw? Is the pointer at the centre? → **−5°, no**: 960 − 40 × (−5) = 1160, so
     the pointer is 200 px right of centre. Integration never compares with the true angle, so the error stays.
  2. *Drift from bias.* The remote lies still. Gyro bias +0.5 °/s, 100 Hz, `yaw += gyro.z * dt`, gain 40. After 60 s:
     yaw? pointer x? → **30°** (0.005° × 6000 samples, or 0.5 °/s × 60 s) → x = 960 − 1200 = −240 → clamped to **0**: the
     pointer has drifted to the left edge while nobody touched the remote.
     (Common slip: 0.5 × 60 × 0.01 = 0.3°, using `dt` and the total time together.)
