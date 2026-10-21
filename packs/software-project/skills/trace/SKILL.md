---
name: trace
description: Build and check the traceability matrix for the learner's project — every requirement ID linked to design elements, tests and benchmarks — and detect drift between design docs and code. Use at milestone checkpoints, after design changes, or when the learner asks whether docs, code and tests are consistent.
---

# /trace [milestone]

Paths come from `subject.md` → Software-project settings (docs dir, requirements file, public API location,
tests dir, benchmarks dir).

## Conventions (teach the first time)
- Requirements have IDs `REQ-F-NNN` / `REQ-N-NNN` and a `Milestone:` tag.
- Design docs state `Satisfies: …`; tests and benchmarks reference IDs in their name or a comment (`// REQ-F-001`).

## Steps
1. Collect requirement IDs and milestones from the requirements file.
2. Search the docs dir for design references, and the tests/benchmarks dirs for test references.
3. Write `<docs>/traceability.md`:

   | Requirement | Milestone | Design | Tests | Benchmark | Status |
   |---|---|---|---|---|---|

   ✅ complete · ⚠️ design or test missing · ❌ neither · ⏳ future milestone
4. **Drift check:** compare the API in `<docs>/modules/*.md` with the real public API location; list
   missing/extra items and changed signatures.
5. Report gaps for the current milestone and drift, each with a **question** to resolve it (update doc? add
   test? drop requirement?). The learner fixes them — don't edit their docs or tests.
6. `/checkpoint` for a project milestone requires no ⚠️/❌ for in-scope requirements.
