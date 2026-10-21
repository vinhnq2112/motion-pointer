---
name: new-subject
description: Set up the learning kit for a new subject — interview the learner about what they want to learn, their real level, goals, time and tools, then generate subject/subject.md, subject/roadmap.md, subject/exercise-format.md, subject/review-checklist.md and learning/progress.md, and install optional packs and agents. Use when subject/subject.md says NOT CONFIGURED or the learner wants to configure a new subject.
---

# /new-subject

## 0. Safety check
If `subject/subject.md` is already configured (does not contain `NOT CONFIGURED`), warn: this replaces the current
subject. Recommend copying the `learning-kit` template to a new folder instead. Continue only if the learner
explicitly confirms; never touch `learning/journal/` or `learning/knowledge/` of an existing subject.

If this folder is a git repo whose history is the template's (e.g. `git log` shows "Add learning kit template")
— the folder was copied together with the template's `.git` — offer to remove `.git` and start a fresh repo,
so the subject has its own history.

## 1. Interview (1–2 questions at a time, conversational)
1. **What** do you want to learn, and **why** (job, project, curiosity)?
2. **Current level** — ask for a self-rating, then verify with 2–4 short diagnostic questions of rising difficulty.
   Record the *verified* level, not just the self-rating. Also check the **prerequisites** the subject needs
   (e.g. basic geometry for a sensor subject) and ask about known weak areas ("I'm bad at spatial thinking").
   Record them under "Learning needs"; each missing prerequisite becomes a short foundations topic.
3. **Final goal** — what should you be able to *do* at the end? Push for a concrete, demonstrable outcome
   (a project, an exam, a conversation, a portfolio piece). This becomes the anchor for "why before how".
   Ask whether there is a **final deliverable** for other people (a talk, slides, a report): audience,
   language, length, format. Slides default to an editable PowerPoint file (`/present`).
4. **Time** — hours/week, typical number and length of sessions, any deadline. If the whole course is
   **≤ 2 weeks or ≤ ~15 hours**, propose **sprint mode**: one phase per day, one plan for the whole course,
   one checkpoint at the end. Say honestly what fits in that time and what does not.
5. **Environment & tools** — what you use or want to use. For technical subjects, check what is installed
   (run version commands) and note it.
6. **Learning preferences** — theory-first or hands-on-first, preferred references, language of explanations.
   If the chat language differs from the language of the field (or a later translation is wanted), agree on a
   **term language** (e.g. "terms stay in English") and the **translate-later** language.
   Also explain the scaffold levels (S3 Guided → S0 Blank, `CLAUDE.md` rule 9) and agree on the project rule,
   e.g. "project work: S2 at most, S0 from Phase N".
7. **Learning tools** — is the real thing missing, invisible or slow to try (no hardware, a hidden process,
   3D movement, a large system)? If yes, propose a learning tool (`CLAUDE.md` rule 10, `/lab`): what it shows,
   the plug-in interface for the learner's code, its ground truth and knobs.
8. **Software project?** — does it involve building software where requirements, architecture and design
   matter? If yes, offer the `software-project` pack.

## 2. Propose, then agree
Present a short proposal and adjust until the learner agrees:
- Phases (5–12; sprint mode: one per day) with topics, an **honest** time estimate at their hours/week, and
  evidence-based exit criteria. Interleave theory and practice; start applying to the final project early to
  keep motivation. Put foundations topics and building the learning tool first, and the final deliverable last.
- How exercises are made and checked (see `.claude/skills/new-subject/templates/exercise-format.md` — pick the
  matching style), including what each scaffold level S3–S0 contains for this subject.
- Review checklist priorities.
- Knowledge categories (3–6).
- Optional subject agents (e.g. a performance coach, a pronunciation coach) — only if clearly useful.
- The learning tool(s), if any: what each shows, its plug-in interface, and when it gets built.

## 3. Write the files
Fill these from `.claude/skills/new-subject/templates/`, replacing every `<placeholder>` and deleting
template comments and options that don't apply:
- `subject/subject.md` ← `templates/subject.md`
- `subject/roadmap.md` ← `templates/roadmap.md`
- `subject/exercise-format.md` ← `templates/exercise-format.md`
- `subject/review-checklist.md` ← `templates/review-checklist.md`
- `learning/progress.md` ← `templates/progress.md`
- `learning/glossary.md` ← `templates/glossary.md` (when a term language is set)
- `learning/showcase.md` ← `templates/showcase.md` (when there is a final deliverable)
Create the practice, project and learning-tool folders named in `subject.md`.
Replace `README.md` (the template's) with a short subject README: what is learned, links to `GUIDE.md`,
`subject/subject.md`, `subject/roadmap.md`, `learning/progress.md`, and how to start (`claude` → "hi").
Delete `CHANGELOG.md` (kit-only) from the subject.

## 4. Packs and agents
- For each chosen pack: copy `packs/<pack>/skills/*` → `.claude/skills/` and `packs/<pack>/agents/*` →
  `.claude/agents/`, then follow the "After installing" steps in `packs/<pack>/README.md`.
- Create agreed subject agents in `.claude/agents/<name>.md` (frontmatter: name, description, tools) and list
  them in `subject.md`.

## 5. Finish
- If the folder is not a git repo, run `git init`, then make a first commit after the learner agrees.
- Run `/kit-sync status` once so the sync state starts clean.
- Summarize what was created in a short table.
- Tell the learner: **restart `claude`** so new skills and agents load, then say "hi" and run `/plan-week`
  (and `/lab` first if a learning tool is needed before the first lesson).
