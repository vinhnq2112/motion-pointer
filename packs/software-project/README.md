# Pack: software-project

For subjects where you **build a piece of software** and want to learn the full lifecycle:
requirements → design → code → test → build/CI → release.

## Adds
| Item | Type | Purpose |
|---|---|---|
| `/design-lesson [D-id]` | skill | Design track D1–D12: how to write requirements, NFRs, architecture, diagrams, API designs, ADRs, test plans, docs. Includes annotated worked examples (`examples/`). |
| `/design-doc <type>` | skill | Guided writing of requirements / architecture / module design / ADR in your project |
| `/trace` | skill | Requirement → design → test traceability matrix + docs-vs-code drift check |
| `architect` | agent | Reviews your design documents |

## Install
Copy `skills/*` → `.claude/skills/` and `agents/*` → `.claude/agents/` (`/new-subject` does this).

## After installing
1. Fill the **Software-project settings** section in `subject/subject.md` (docs dir, requirements file,
   public API location, tests dir, benchmarks dir).
2. Add to `subject/roadmap.md` a **Design track** table mapping D1–D12 (see `skills/design-lesson/SKILL.md`)
   to the phases, each placed *just before* the phase that needs it, with what it is applied to.
   Add the rule: *each project milestone's design docs exist, were reviewed by `architect`, and `/trace` shows
   every in-scope requirement has a design element and a test.*
3. Add a **Design track** status table (ID · Lesson · Status · Explain-back) to `learning/progress.md`.
4. Add `design` to the knowledge categories in `subject.md`, and list the pack's commands there.
