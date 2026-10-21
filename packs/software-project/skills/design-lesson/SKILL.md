---
name: design-lesson
description: Teach one software design / specification technique (requirements, NFRs, architecture, diagrams, API design, ADRs, traceability, docs) from the design track D1–D12, using annotated worked examples and a critique exercise, then have the learner apply it to their own project docs. Use when the learner asks how to write a spec/design, or when a design-track lesson is due.
---

# /design-lesson [D-id or topic]

Goal: learn one design technique in 45–60 minutes and apply it to the learner's real project docs
(docs dir from `subject.md` → Software-project settings).

1. **Pick the lesson.** Argument, else the next unfinished D-item in `learning/progress.md` → Design track that
   belongs to the current or next phase (see the Design track table in `subject/roadmap.md`).
   Say what the technique is and **what goes wrong in projects without it** (2–3 sentences).
2. **Probe.** Ask how the learner handles this in their job or past projects; build on that experience.
3. **Teach** in small chunks with the worked examples in this skill's `examples/` folder
   (*thumbgen*, a small CLI tool — deliberately unrelated to the learner's project):
   what the artifact is for and who reads it → structure and rules of a good one → walk the example and point out
   *why* each part is written that way. One check question per chunk.
4. **Critique exercise.** Write a short flawed sample inline (~10–20 lines: vague requirements, cyclic module
   diagram, one-option ADR, API that hides ownership…). The learner lists the problems; compare and discuss misses.
5. **Apply to the project.** The learner writes the corresponding section of their real doc (create it from the
   `/design-doc` template if missing). You ask and critique; you do not write their content. One section only.
6. **Explain-back:** "When would you write this, and what makes it good?" Score 1–5; < 4 → weak spots.
7. **Close.** Save to `learning/knowledge/design/<topic>.md` (`/note` format) including a short checklist; mark the
   D-item in `learning/progress.md`; suggest `/design-doc` to continue or `architect` review when complete.

## Design track

| ID | Lesson | Core points |
|---|---|---|
| D1 | Specs & ADRs basics | Requirements (what/why) vs design (how) vs plan (when). Docs serve future-you and reviewers. ADR: context, ≥2 options, decision, consequences. Lightweight beats perfect. |
| D2 | Requirements & use cases | Use cases from user goals. One requirement = one testable statement. IDs (`REQ-F-001`). Acceptance criteria. "Shall" vs "should". No implementation in requirements. Scope & non-goals. |
| D3 | Non-functional requirements & budgets | Performance, memory, startup, portability, build time, usability. Each: metric, target, conditions, how measured. Budgets split per subsystem. |
| D4 | Architecture & module boundaries | Module = responsibility + public interface + hidden details. Layering, dependency direction, no cycles, third-party libs isolated. Mermaid `graph`. C4-lite: context → modules → components. |
| D5 | Data flow & sequence diagrams | Data vs control flow. Mermaid `sequenceDiagram` of the core loop/request. Who owns data, where it's copied/freed. Threading boundaries. |
| D6 | Test plans & traceability | Unit / integration / benchmark / manual. Requirement → design → test. Tag tests with IDs. `/trace`. |
| D7 | Interface / API design | Small surface, hard to misuse, consistent naming. Ownership and errors visible in signatures. Header/interface-first with a usage example before implementation. |
| D8 | Abstractions for variation points | Find the real variation (e.g. two backends, two platforms). Interface at the right level. YAGNI: one implementation now, a seam for the second. |
| D9 | Performance-aware design docs | Document data layout, access patterns, sizes/counts, memory budget per subsystem, hot vs cold data. Justify with measurements. |
| D10 | Living docs & drift | Docs change with code. Review checklist. Superseded ADRs are marked, never deleted. Drift detection (`/trace`). |
| D11 | API usability & user docs | Write the getting-started guide and example *before* finalizing the API. Pit of success. Tutorials vs how-tos vs reference. |
| D12 | Release docs | Semantic versioning, API stability, CHANGELOG, performance report (method, hardware, results vs budgets), release checklist. |
