---
name: checkpoint
description: Verify the current phase's exit criteria from subject/roadmap.md with real evidence, then advance to the next phase or give a remediation plan, and adjust the roadmap honestly if pace differs. Use at the end of a phase or when the learner asks "am I ready to move on".
---

# /checkpoint

1. Read the current phase's **Exit** criteria in `subject/roadmap.md`.
2. Check each criterion **with evidence** — actually run checks (tests, benchmarks, builds), confirm files
   exist and were reviewed, and ask 3 explain-back questions on the phase's core concepts (score them).
   At least one piece of evidence must be work done at scaffold level **S1 or S0** (see `/exercise`): proof that
   the learner can do it without starter material. If there is none, give a short S1 task now.
3. Show a table: criterion · evidence · ✅/❌.
4. **All met** → mark the phase done in `learning/progress.md`, set the next phase, give a short retrospective
   (went well / improve / roadmap change).
   **Not met** → concrete remediation plan (specific lessons/exercises, estimated hours).
5. If actual pace is far from the estimates, propose an honest roadmap adjustment and apply it if the learner agrees.

## Sprint mode
In sprint mode (`subject.md` → "Course mode"), there is one checkpoint, on the last day, over the whole course.
The S1/S0 evidence rule is replaced by: at least one exercise done at **S2 or lower**, unless `subject.md`
says otherwise. If a final deliverable exists, check it too: the file builds and every slide's key message
came from the learner (`learning/showcase.md`). End with an honest list of what to learn next.
