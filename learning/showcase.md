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
