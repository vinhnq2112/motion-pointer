# Learning Kit — User Guide

A personal mentor for learning anything with Claude Code. The mentor teaches with questions, gives you
practice with checkers, reviews your work, quizzes you on what you forget, keeps notes of everything you ask,
and tracks your progress against a roadmap.

---

## 1. Start a new subject (once, ~20 min)

**Option 1 — on GitHub (recommended):** open the `learning-kit` repo → **Use this template** → create a new
(private) repo, e.g. `japanese-n4`. Then:

```
git clone https://github.com/<you>/japanese-n4
cd japanese-n4
claude
/new-subject
```

**Option 2 — local copy** (without the template's `.git`, so the subject gets its own history):

```
robocopy learning-kit my-new-subject /E /XD .git .kit
cd my-new-subject
claude
/new-subject
```

You don't need the kit folder next to your subject: `/kit-sync` uses `../learning-kit` if it exists, otherwise it
clones the kit itself (URL in `kit.json`) into `~/.learning-kit/`.

The mentor interviews you: what you want to learn, your current level (with a few check questions),
your final goal, hours per week, tools. It then writes your `subject/` files and a roadmap.
**Restart `claude`** afterwards so new skills/agents load, then run `/plan-week`.

## 2. Every session — just 3 things

| You | What happens |
|---|---|
| Say **"hi"** | Mentor says where you are, today's goal, asks a warm-up question, suggests the next step |
| **Work and ask anything** | Lessons, exercises, reviews. Every real question gets answered **and saved to your notes** |
| Say **"done"** | Journal, progress and weak spots are updated; you get the next step for next time |

You don't need to remember commands — the mentor always suggests the next one. Plain language works:
"teach me X", "give me practice", "check my work", "quiz me", "save this", "I'm done".

💡 Start a **fresh `claude` session for each study session** (or `/clear`). Your progress lives in files, so
nothing is lost, and a fresh session stays fast and focused.

## 3. A typical week (example: 7 hours)

| Session | Time | Flow |
|---|---|---|
| Mon | 2 h | `/quiz` warm-up 10' → `/lesson` 45' → `/exercise` 60' → "done" |
| Wed | 2 h | warm-up → finish exercise → `/review` → fix → "done" |
| Sat | 3 h | warm-up → project work (apply what you learned) → `/review` → "done" |

Start of each week: `/plan-week`. End of each phase: `/checkpoint`.

### Short course (sprint mode)
For a course of up to 2 weeks or ~15 hours (e.g. 1 hour a day for 7 days), `/new-subject` proposes **sprint
mode**: one phase per day, `/plan-week` plans every day at once, and `/checkpoint` runs only on the last day.
A 1-hour day: warm-up 5' → `/lesson` 20' → `/exercise` 30' → "done" 5' (with a showcase entry).

### Learning tools, visuals, two languages, final slides
- **Learning tools (`/lab`):** if you can't see or try the real thing (no hardware, 3D movement, a hidden
  process), the mentor builds a simulator or visualizer for you. You write only the part you are learning,
  behind a small plug-in interface; the tool measures your result with real numbers.
- **Visuals first:** set "Learning needs" in `subject.md` (e.g. "weak at spatial thinking") and every new
  idea is shown first — a diagram or something to watch in the learning tool — then explained.
- **Two languages:** learn in your language while technical terms stay in the field's language. Terms go into
  `learning/glossary.md`; later `/translate en` converts your notes into `translations/en/`.
- **Final slides (`/present`):** after each topic you give one key message (saved in `learning/showcase.md`).
  At the end, `/present` turns them into an editable PowerPoint file (needs Node.js), checks every slide, and
  rehearses the talk with you.

### How much help exercises give (scaffold levels)
Exercises come with starter material that gets smaller as you improve. The goal is that you can work from a
blank page at the end.

| Level | You get | Example (programming) |
|---|---|---|
| **S3 Guided** | Template + numbered `TODO` steps (what and how) + pseudocode + `YOU DECIDE` choices | `// TODO 2: free the buffer in the destructor` |
| **S2 Outline** | Template + `TODO` goals (what only) | `// TODO: the class owns the buffer` |
| **S1 Spec** | Requirements + checker (e.g. tests) | README + `tests/` |
| **S0 Blank** | Requirements only — you also write the checks | README |

- Every new topic starts at **S3**. After a passed exercise with explain-back 4+, the next exercise is one level
  lower. If an exercise fails, the level goes one up.
- `YOU DECIDE: …` marks a choice the mentor leaves to you on purpose. `/review` checks your answer.
- Say **"more help"** or **"less help"** any time to move one level up or down.
- `/checkpoint` needs at least one piece of work done at S1 or S0.
- Your current level per topic is in `learning/progress.md` → "Scaffold levels".

## 4. Commands

| Command | When |
|---|---|
| `/new-subject` | Once, to set up a subject |
| `/plan-week` | Start of each week |
| `/lesson [topic]` | Learn the next (or a given) topic |
| `/exercise [topic]` | Practice with a checker (tests, answer check…) and starter material at your level |
| `/review [path]` | Get feedback on your work — hints, not fixes |
| `/quiz [n]` | Refresh what you might forget |
| `/note` · `/note find X` · `/note review X` | Save / search / revisit your notes |
| `/checkpoint` | "Am I ready for the next phase?" |
| `/lab [tool]` | Build, extend or fix your simulator / visualizer |
| `/translate [lang] [path]` | Translate your notes and docs (originals stay) |
| `/present` | Build your final PowerPoint from the showcase |
| `/kit-sync [push\|pull]` | Share kit improvements between this subject and the template (section 8) |

**software-project pack** (for subjects where you build software): `/design-lesson`, `/design-doc`, `/trace`
and the `architect` agent — learn to write requirements, architecture, module designs and ADRs.

## 5. Your files

| Path | What | Edit it yourself? |
|---|---|---|
| `subject/subject.md` | Your profile, goals, time, tools, settings | ✅ anytime |
| `subject/roadmap.md` | Phases, topics, exit criteria | ✅ anytime — the mentor follows it |
| `subject/exercise-format.md` | How exercises are built and checked | ✅ |
| `subject/review-checklist.md` | What reviews look for | ✅ |
| `learning/progress.md` | Where you are | mentor updates it |
| `learning/weak-spots.md` | Topics to revisit | mentor updates it |
| `learning/journal/` | One entry per session day | mentor writes it |
| `learning/knowledge/` | **Your notes**: Q&A, summaries, pitfalls, know-how, by topic | ✅ both of you |
| `learning/glossary.md` | Technical terms with translations | ✅ both of you |
| `learning/showcase.md` | Key messages + visuals for the final deliverable | ✅ both of you |
| `tools/` | Learning tools (simulators, visualizers) | mentor maintains it (`/lab`) |
| `presentation/` | `slides.md` source + the built `.pptx` | ✅ edit `slides.md`, then rebuild |
| `translations/` | Translated copies of your docs | `/translate` writes it |

The knowledge base is plain Markdown — readable in VS Code, Obsidian or GitHub.

## 6. FAQ

- **The mentor did my work for me.** Say "you broke rule 1" — it will step back to hints.
- **I'm behind schedule / missed a week.** Normal. Tell the mentor or run `/plan-week`; `/checkpoint` can
  adjust the roadmap honestly.
- **Too easy / too hard.** Say so — the mentor adapts. For exercises, say "less help" / "more help" (section 3). To change it permanently, edit "Current level" in `subject.md`.
- **Back up my progress.** The folder is a git repo: commit `subject/` and `learning/` regularly.
- **I improved the kit while learning — how do I share it?** See section 8.

## 7. Customizing

- **Skills** (`.claude/skills/<name>/SKILL.md`): the commands. Edit the instructions in plain English.
- **Agents** (`.claude/agents/*.md`): specialist reviewers. Add subject-specific ones and list them in `subject.md`.
- **Packs** (`packs/<name>/`): optional bundles of skills + agents. Install by copying `packs/<name>/skills/*`
  into `.claude/skills/` and `packs/<name>/agents/*` into `.claude/agents/` (`/new-subject` does this for you).

## 8. Syncing improvements between a subject and the kit

You will improve the kit while learning ("make `/quiz` ask harder questions", "add a skill for X").
Make the change in your subject folder, then share it with `/kit-sync`:

| You want to… | Command |
|---|---|
| See what differs | `/kit-sync` (status) |
| Send improvements from this subject to the template | `/kit-sync push` |
| Get template improvements into this subject (e.g. made in another subject) | `/kit-sync pull` |

- **What syncs:** kit files only — `CLAUDE.md`, `GUIDE.md`, `kit-sync.ps1`, skills and agents from the template,
  and installed pack skills/agents. **Never** `subject/`, `learning/`, your exercises/project, or subject agents.
- **Keeps the kit generic:** before pushing, the mentor checks the change for subject-specific content and moves
  it into `subject/*.md`, so the template stays usable for any subject.
- **Safe:** every action is a dry run first. Files changed on both sides are reported as conflicts and merged by
  the mentor — never silently overwritten. Both repos get a commit.
- **Flow between subjects:** subject A `push` → kit (GitHub) → subject B `pull`.
- **Where the kit is:** `../learning-kit` if you have it there (edit it directly if you like); otherwise
  `/kit-sync` keeps its own clone in `~/.learning-kit/learning-kit`, updated from GitHub. On a new machine just
  clone your subject — syncing works right away.
- **Update notice:** at session start the mentor checks GitHub for a newer kit release (`kit-sync.ps1 check`) and
  tells you in one line.
- **Releases:** say "release the kit" — the mentor bumps `VERSION`, updates `CHANGELOG.md`, tags `vX.Y.Z` and pushes.
- Without Claude: `powershell -ExecutionPolicy Bypass -File kit-sync.ps1 status | push | pull [-Apply]`.
