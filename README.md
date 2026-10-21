# Learning Kit

A reusable Claude Code kit that turns Claude into a personal learning mentor for any subject:
Socratic lessons, exercises with checkers and starter material (TODOs, pseudocode) that fades as you improve,
teaching reviews, spaced-repetition quizzes, an automatic
knowledge base of everything you ask, and progress tracking against a roadmap. Also: mentor-built learning
tools (simulators, visualizers), visuals-first lessons, a term glossary with translation, short "sprint" courses,
and a final PowerPoint deck built from your own key messages.

**This folder is the template.** Copy it (without `.git`) for each new subject, open `claude` in the copy,
run `/new-subject`. Improvements flow back and forth with `/kit-sync`.

👉 Read **[GUIDE.md](GUIDE.md)** — setup, the 3-step session, a typical week, commands, syncing, FAQ.

Version: see [VERSION](VERSION) · changes: [CHANGELOG.md](CHANGELOG.md)

```
learning-kit/
├── CLAUDE.md               # mentor rules (subject-independent)
├── GUIDE.md                # how to use the kit
├── VERSION, CHANGELOG.md
├── kit-sync.ps1            # sync kit files between a subject and this template
├── .claude/
│   ├── skills/             # new-subject, plan-week, lesson, exercise, review, quiz, note, checkpoint,
│   │                       # lab, translate, present (+ build-pptx.mjs), kit-sync
│   └── agents/reviewer.md  # generic reviewer, driven by subject/review-checklist.md
├── packs/
│   └── software-project/   # optional: design-lesson (+ examples), design-doc, trace, architect agent
├── subject/                # filled by /new-subject
└── learning/               # progress, weak spots, journal, knowledge base
```

Requirements: Claude Code, Windows PowerShell 5.1+ (for `kit-sync.ps1`), git. Optional: Node.js (for `/present`).
