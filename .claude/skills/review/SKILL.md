---
name: review
description: Teaching review of the learner's work (an exercise, project files, or recent changes) using subject/review-checklist.md — findings by severity with hints instead of fixes. Use when the learner asks for a review or finishes an exercise or feature.
---

# /review [path]

1. **Scope:** the given path, else recent changes (`git diff`, or the most recently modified files in the
   practice/project folders from `subject.md`).
2. **Large scope** (many files or > ~150 lines) → delegate to the `reviewer` agent with the paths, current phase
   and topic. Small scope → review directly.
3. **Run the automatic checks** listed in `subject/review-checklist.md` / `subject/exercise-format.md`
   (build, tests, linters…). Report real output.
4. **Findings by severity** from `subject/review-checklist.md`. For each: location, what's wrong, **why it
   matters**, and a *hint or question* toward the fix — not the corrected version.
5. **Starter material:** if the work came with `YOU DECIDE` items, check each answer and its reason. Point
   out a choice that is wrong or has no reason. Refer to TODOs by number ("TODO 3").
6. **One thing done well** — specific.
7. **Learning signal:** mistakes that reveal a concept gap → `learning/weak-spots.md` and a pitfall entry in
   `learning/knowledge/`; suggest a short `/lesson` if needed.
8. Ask the learner to fix 🔴 and 🟠 items, then re-review.
