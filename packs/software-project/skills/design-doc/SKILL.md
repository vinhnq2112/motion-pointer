---
name: design-doc
description: Guide the learner to write requirements, architecture, detailed module designs, or ADRs for their project through questions and critique, then have them reviewed by the architect agent. Use when starting a milestone, a new module, or making a significant technical decision.
---

# /design-doc <requirements | architecture | module <name> | adr <title>>

The learner writes the document. You ask questions, propose structure, and challenge weak points.
`<docs>` = docs dir from `subject.md` → Software-project settings.

## Conventions
- Requirement IDs: `REQ-F-NNN` (functional), `REQ-N-NNN` (non-functional), each with acceptance criteria and a
  `Milestone:` tag. Use cases: `UC-N`.
- Design docs start with `Satisfies: <REQ ids>`; ADRs with `Related: <REQ ids>` (used by `/trace`).
- Diagrams in **Mermaid** inside the Markdown (`graph TD` for dependencies, `sequenceDiagram` for flows).
- Worked examples: `.claude/skills/design-lesson/examples/`.

## Templates (create the file with headings only; the learner fills it in)
**requirements** → `<docs>/requirements.md`
Vision · Users & use cases · Functional requirements (ID, statement, acceptance criteria, source) ·
Non-functional requirements (ID, metric, target, conditions, how measured) · Constraints · Non-goals · Open questions

**architecture** → `<docs>/architecture.md`
Context · Modules & responsibilities · Dependency rules (diagram) · Key data flows (sequence diagram) ·
Threading model · Memory/resource strategy · Error-handling policy · Extension points

**module <name>** → `<docs>/modules/<name>.md`
Purpose · Public API sketch + usage example · Data structures & ownership · Lifetimes · Performance notes ·
Test plan (with requirement IDs) · Alternatives considered

**adr <title>** → `<docs>/adr/NNNN-<slug>.md`
Status · Related · Context (with numbers) · Options (≥2, pros/cons) · Decision · Consequences

## Process
0. If the matching design-track lesson isn't done yet, suggest `/design-lesson <D-id>` first (learner decides).
1. Explain the purpose of this doc type and what "good" looks like; point to the matching example file.
2. Go section by section: 2–3 guiding questions, the learner writes, you critique. Push for **testable** statements.
3. When complete → `architect` agent review; discuss findings.
4. If requirements or design changed → `/trace`.
5. Record in `learning/progress.md`; save new insights to `learning/knowledge/design/`.
