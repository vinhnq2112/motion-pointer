---
name: lesson
description: Teach the next topic from subject/roadmap.md (or a topic given as argument) as a Socratic lesson tied to the learner's final goal, with checks, a predict/try task and explain-back, then save it to the knowledge base. Use when the learner says "lesson", "teach me", "next topic".
---

# /lesson [topic]

Goal: one focused topic in 30–60 minutes. Never more than one major concept per lesson.

1. **Pick the topic.** Argument, else the next unfinished item of the current phase in `subject/roadmap.md`
   (see `learning/progress.md`). State the topic and **why the final goal needs it** (from `subject/subject.md`)
   in 2–3 sentences.
2. **Probe prior knowledge.** 1–2 questions about what the learner already thinks. Wait. Adjust depth.
   If an answer shows a missing **prerequisite**, stop and teach a 5–10 minute foundations chunk first
   (or, if it is bigger, add it to the roadmap before this topic and say so).
3. **Teach in small chunks** (each ≤ ~20 lines of prose + one short example):
   - **show it first** when `subject.md` → "Learning needs" asks for visuals (`CLAUDE.md` rule 11): a small
     diagram (ASCII or Mermaid), or a step in the learning tool — "do X in the tool, watch Y" — and ask what the
     learner saw before you explain it
   - the idea in plain words, then the precise rule
   - a minimal example — ideally first *what goes wrong* without the concept, then the fix
   - one level deeper: how it actually works underneath
   - common pitfalls
   After each chunk ask one check question and wait. Never dump all chunks at once.
4. **Predict / try.** A small task to attempt *before* being told the answer (predict the output, translate a
   sentence, solve a mini problem). For practical subjects, have the learner actually do it in the practice folder.
5. **Explain-back.** The learner explains the concept as if to a beginner. Score 1–5 with one-line justification;
   < 4 → `learning/weak-spots.md`.
6. **Close.**
   - 3-line summary + one reference from the preferred references in `subject.md`.
   - Save to `learning/knowledge/` (`/note` format): Summary bullets, pitfalls, and every question asked.
   - Add new terms to `learning/glossary.md` if the subject has one.
   - If there is a final deliverable, note a showcase idea for this topic (a visual, a number to measure).
   - Update "Current topic" in `learning/progress.md`.
   - Suggest next step: usually `/exercise` on this topic.

Respect the scope/standard and teaching specifics in `subject.md`. If the learner is confused, go one level
more concrete, not sideways.
