---
name: present
description: Build the learner's final presentation (an editable PowerPoint .pptx by default) from learning/showcase.md — audience, story outline, one idea per slide, visuals from the learning tool, speaker notes — in the deliverable language set in subject/subject.md. Use when the learner says "make the slides", "presentation", "final talk", or the roadmap's last phase needs the deliverable.
---

# /present

The presentation is the learner's proof that they can explain the subject to other people. Rule: **the learner
writes the key message of every slide** (one sentence — that is the explain-back). The mentor shapes the story,
wording, layout, visuals and speaker notes around those messages. Never invent a key message the learner has
not given or confirmed.

## 1. Gather
1. Read `subject/subject.md` → "Final deliverable" (audience, language, length, format) and `learning/showcase.md`.
2. Ask, one question at a time, only what is missing: who the audience is and what they already know; how many
   minutes; the one thing the audience must remember.
3. If `learning/showcase.md` has gaps for topics the talk needs, ask the learner for those key messages now.

## 2. Outline first
Propose a story in 8–15 slides (about 1 slide per minute) and wait for agreement:
title → the problem (why the audience should care) → one slide per key idea, simple to hard →
demo / results with real numbers → limits and lessons → summary (the one thing to remember) → questions.
Show it as a table: # · slide title · key message · visual.

## 3. Write the source
Write `presentation/slides.md` in the deliverable language, in this format:

```
---
title: <talk title>
subtitle: <one line>
author: <learner name>
date: <YYYY-MM-DD>
---
# <slide title>
- <bullet, max ~8 words>
  - <sub-bullet>
![<caption>](images/<file>.png)
Note: <speaker notes: what to say, 2–4 sentences>

# <slide title>
<one sentence alone on a slide = big statement slide>
```

Rules: one idea per slide · at most 5 bullets · a picture beats a paragraph · every technical term the
audience may not know is explained on the slide where it first appears · glossary terms from
`learning/glossary.md` are used exactly · code blocks only when they are the point (≤ 8 lines).
Visuals: screenshots from the learning tool (`/lab`) and diagrams in `presentation/images/`. Ask the learner
to take the screenshots you name (state exactly what must be on screen), or generate charts with a script
from real data. Never use a picture that shows numbers you did not measure.

## 4. Build the file
Default format: **PowerPoint (.pptx)**, editable. Other formats only if `subject.md` asks for them.
1. Once per subject: in `presentation/`, run `npm init -y` and `npm install pptxgenjs` (needs Node.js;
   check with `node --version`). Add `presentation/node_modules/` to `.gitignore`.
2. Build: `node .claude/skills/present/build-pptx.mjs presentation/slides.md presentation/<name>.pptx`
3. **Check it.** On Windows with PowerPoint installed, export each slide to PNG and look at every image:
   ```
   $p = New-Object -ComObject PowerPoint.Application
   $pr = $p.Presentations.Open("<full path>.pptx", $true, $false, $false)
   foreach ($s in $pr.Slides) { $s.Export("<folder>\slide$($s.SlideIndex).png", 'PNG', 1280, 720) }
   $pr.Close(); $p.Quit()
   ```
   Fix overflowing text, empty slides and unreadable images in `slides.md`, then build again.
   Without PowerPoint, say that you could not check the layout visually.
4. Tell the learner the file path, that the `.pptx` is fully editable in PowerPoint, and that `slides.md`
   is the source to change and rebuild (and to translate with `/translate`).

## 5. Rehearse
Offer one rehearsal: the learner explains each slide in their own words; give honest feedback on clarity
and timing (`CLAUDE.md` rules 5–6). Save weak explanations to `learning/weak-spots.md`.

Tool notes: on Windows, if `python` opens the Microsoft Store or says "not found", Python may still be
installed — use `py` instead. The builder itself needs only Node.js.
