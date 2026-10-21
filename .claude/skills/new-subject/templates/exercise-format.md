# Exercise format — <subject>

Used by `/exercise` and `/review`. Pick and adapt one style below; delete the others.

## Folder layout
`exercises/NN-<topic-slug>/` containing:
- `README.md` — task, requirements, constraints, stretch goal, connection to the final project (mentor writes)
- <checker files — mentor writes>
- <files the learner creates/fills>

## How to check
<exact commands or procedure>

## Exercise types to rotate
- <type> — <description>

## Scaffold levels for this subject
What the starter material looks like at each level (rules: `/exercise` → "Scaffold levels").
- **S3 Guided:** <starter files/template + numbered `TODO` steps (what + how) + pseudocode for hard steps + ≥1 `YOU DECIDE`>
- **S2 Outline:** <starter structure + `TODO` goals (what only)>
- **S1 Spec:** <README requirements + checker, no starter content>
- **S0 Blank:** <requirements only; learner also writes the checks>

---

### Style A — Programming (tests as checker)
- Mentor writes: README, build file (only after the learner wrote one once), **tests**, empty stubs/declarations.
- Learner writes: the implementation.
- Check: build + run tests (e.g. `ctest`, `pytest`, `npm test`); optionally sanitizers / linters.
- Types: implement-from-tests · debug lab (planted bug) · refactor without breaking tests · measure (predict, then benchmark).
- Scaffold levels: S3 = stub files with signatures and empty bodies, numbered `// TODO` comments with
  plain-language pseudocode, `// YOU DECIDE:` comments · S2 = signatures + `// TODO` goals only · S1 = README +
  tests, learner creates the files · S0 = README only, learner also writes the tests.

### Style B — Language learning
- Mentor writes: README with the task and the grammar/vocabulary targets. The grading rubric is applied when
  grading and not shown before the attempt (the mentor has no memory between sessions, so the targets in the
  README are what grading is based on).
- Learner writes: translations, short texts, dialogues, or records audio and pastes a transcript.
- Check: mentor grades against the rubric (accuracy, grammar points, vocabulary, naturalness), then the learner corrects.
- Types: translate · write about X using grammar Y · role-play dialogue · error correction · dictation from a text.
- Scaffold levels: S3 = sentence frames with gaps, a word bank, numbered `TODO` hints per sentence, ≥1 free
  `YOU DECIDE` sentence · S2 = topic + required grammar points per paragraph · S1 = topic + targets only ·
  S0 = open topic; learner also says which grammar they used.

### Style C — Theory / maths / concepts
- Mentor writes: problem set (graded difficulty).
- Learner writes: solutions with reasoning in `solutions.md`.
- Check: mentor checks reasoning step by step, not just the final answer.
- Types: compute · prove/justify · explain to a beginner · find the flaw in a given argument · apply to a real case.
- Scaffold levels: S3 = solution outline with numbered steps to fill in, the method named, ≥1 `YOU DECIDE` step ·
  S2 = the steps' goals only · S1 = the problem only · S0 = the learner also writes a check (example, special case).
