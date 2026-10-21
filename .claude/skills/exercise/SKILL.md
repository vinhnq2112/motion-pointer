---
name: exercise
description: Create a hands-on practice task in the format defined by subject/exercise-format.md (with tests, rubric or problem set as checker), with starter material (TODO steps, pseudocode, templates) at the learner's current scaffold level, and guide the learner through it with the hint ladder, never giving the solution. Use after a lesson, when the learner asks for practice, or says "more help" / "less help".
---

# /exercise [topic]

1. **Topic:** argument, or the current topic in `learning/progress.md`.
2. **Read `subject/exercise-format.md`** and follow it exactly: folder layout, what the mentor creates, what the
   learner creates, how to check, exercise types, and what each scaffold level contains for this subject.
3. **Pick the scaffold level** (see "Scaffold levels" below) from the "Scaffold levels" table in
   `learning/progress.md`. A topic that is not in the table starts at **S3**. Project work uses the project
   rule in `subject.md` → "Scaffolding". Tell the learner the level in one line, e.g.
   "Level S2 Outline: TODO goals only, no pseudocode. Say 'more help' if you get stuck for long."
4. **Create** the next `exercises/NN-<topic-slug>/` (or the practice folder from `subject.md`) with the
   README, the checker, and the starter material for the level. **Never write the solution.**
   If you can, run the checker on the untouched starter material: it must build/run, and the core checks must
   fail. Report the real result. If you cannot run it, say so and don't guess numbers.
5. **Difficulty:** match the last explain-back score on the topic. Each exercise has a core part (30–60 min)
   and one stretch goal. Rotate exercise types across exercises. The stretch goal is always one level less
   scaffolded than the core part.
6. **Tell the learner exactly how to check their work** (commands or procedure).
7. **While they work:** use the hint ladder (L1→L4). For error messages, first ask what the message says.
8. **When done:** run `/review` on the result. Then, in `learning/progress.md`:
   - record the exercise under "Completed" with its level, e.g. `03-raii-buffer (S2) — passed`;
   - update the topic's row in "Scaffold levels" (rules below).
   Suggest the next step.

## Scaffold levels

Starter material helps the learner start. It must get smaller over time, so that in the end the learner can
work from a blank page. `subject/exercise-format.md` says what each level looks like for the subject.

| Level | Mentor gives | Learner does |
|---|---|---|
| **S3 Guided** | Starter files/template with structure (names, declarations, headings, sentence frames). Numbered `TODO` steps that say *what to do and how*. Pseudocode for the hard steps. Checker. | Writes the content for every TODO and answers the `YOU DECIDE` items. |
| **S2 Outline** | Starter structure plus `TODO` items that say only *what* the result must be, not how. No pseudocode. Checker. | Plans the steps and writes the content. |
| **S1 Spec** | README with requirements and the checker. No starter content. | Creates the structure and the content. |
| **S0 Blank** | Requirements only. | Creates the structure, the content **and** the checks (e.g. writes the tests). |

### Rules for starter material
1. **Never the solution.** A TODO or pseudocode line describes a step. It is never the finished work in
   another form. Pseudocode is plain-language steps, not code that can be copied line by line.
2. **At least one `YOU DECIDE`.** Even at S3, leave one or more important choices to the learner, marked
   `YOU DECIDE: <question>`, with a pointer to the concept (e.g. "see rule of 0/3/5"). Review checks the
   answers.
3. **Number the TODOs** (`TODO 1`, `TODO 2`, …) in the order the learner should do them, so hints and reviews can
   refer to them.
4. **Follow the writing style** in `CLAUDE.md`: every TODO is one short, clear sentence.
5. **Tests and checkers are not scaffolding.** They are provided at every level except S0.

### Moving between levels
- **Down one level** (less help): the exercise passed **and** the latest explain-back score on the topic is 4 or 5,
  **and** no L4 hint ("show me") was needed.
- **Same level:** the exercise passed but one of the conditions above is not met.
- **Up one level** (more help): the exercise did not pass, or the explain-back score is below 4.
- **The learner decides too:** "more help" / "less help" moves the current topic one level up / down right away.
  Record it in the table with "learner asked".
- **Never above S3, never below S0.** A topic at S0 that passes stays at S0.
- **Checkpoints and project work:** exit evidence for `/checkpoint` must include work done at S1 or S0.
  Project work follows the rule in `subject.md` → "Scaffolding" (for example "S2 at most, S0 from Phase 4").

### Table format in `learning/progress.md`
```
## Scaffold levels
| Topic | Level | Last exercise | Last change |
|---|---|---|---|
| RAII | S2 | 03-raii-buffer — passed, explain-back 4 | ↓ 2026-10-20 |
```
