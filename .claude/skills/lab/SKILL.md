---
name: lab
description: Build, extend or fix a mentor-owned learning tool (simulator, visualizer, data generator) listed in subject/subject.md under "Learning tools" — for when the real thing is missing, invisible or slow to test. The learner's code plugs in through a small documented interface; the tool provides ground truth for measuring their work. Use when the learner says "build the simulator", "the lab is broken", "add X to the visualizer", or before the first exercise that needs the tool.
---

# /lab [tool]

A learning tool is **infrastructure, not the topic** (`CLAUDE.md` rule 10). The mentor writes and maintains it,
so the learner spends their time on the concept, not on graphics or plumbing. The learner's part is never
inside the tool.

## 1. Define (first time only)
Agree with the learner, then record under `subject.md` → "Learning tools":
- **What it shows** — the thing the learner cannot see or try for real (e.g. "a 3D device that moves like the
  real one, with its sensor readings as live graphs").
- **Plug-in interface** — the one small, stable boundary where the learner's code goes: inputs, outputs,
  units, call rate (e.g. "`update(input, dt)` → result, called 100 times per second"). Keep it tiny and
  write it down exactly; exercises are written against it.
- **Ground truth** — what the tool knows for sure that the learner's code must estimate, and which numbers it
  prints (error, drift over time, …). This is how exercises get checked with real numbers.
- **Knobs** — settings the lessons need (noise, error size, speed, recorded or scripted motions), each with a
  safe default.
- **Folder** (default `tools/<name>/`), language, build command, and dependencies. Prefer what the learner
  already has installed (`subject.md` → "Tools / environment"); fetch extra libraries with the build system
  (e.g. CMake FetchContent) rather than asking for manual installs.

## 2. Build in small steps
1. Smallest version that shows one thing correctly, then add features in the order lessons need them
   (`subject/roadmap.md`). Do not build features for later phases yet.
2. Ship a **default plug-in** that does something trivial (e.g. returns zero), clearly marked as a placeholder, so
   the tool runs before the learner writes anything. Exercises replace it; the placeholder is never a solution.
3. Write a `README.md` in the tool folder: how to build, how to run, controls, knobs, the plug-in interface,
   and what each on-screen element means (plain words, `CLAUDE.md` writing rules).
4. Add a smoke check (a small test or a headless run mode that prints numbers) so you and the learner can
   prove it works without looking at a window.

## 3. Verify — evidence, not hope
- Build and run it yourself. Report the real result. If you cannot open a window, run the headless/smoke mode
  and say that you could not check the visuals.
- Check correctness against something known: a scripted motion with an exact answer, a unit test of the maths,
  or a physical rule (e.g. "at rest, the reading equals gravity"). A tool that shows wrong behaviour teaches
  wrong ideas — this is the most important check.
- Ask the learner to run it once and confirm what they see, with an exact list of what should be on screen.

## 4. Use in teaching
- Lessons (rule 11): give "do this in the tool, watch that" steps with the expected observation.
- Exercises: the learner's code goes behind the plug-in interface; the checker can use the tool's ground
  truth (e.g. "error after 60 s < 5°").
- Screenshots of the tool go into `learning/showcase.md` and later into the presentation (`/present`).

## 5. Maintain
When the learner reports a bug, reproduce it first, fix it, re-run the smoke check, and add a line to the
tool's README "Changes" section. Keep the plug-in interface stable; if it must change, update every exercise
that uses it in the same step and tell the learner.
