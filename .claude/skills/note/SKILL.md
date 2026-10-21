---
name: note
description: Save, find, or review entries in the learner's knowledge base (learning/knowledge/) — questions with answers, lesson summaries, pitfalls and know-how, organised by topic. Use when the learner says "note this", "save this", "remember this", "what did I learn about X", or asks to search their notes.
---

# /note [save <text> | find <query> | review <topic> | index]

With no argument: `save` the most recent question and answer from this conversation.

## Layout
`learning/knowledge/<category>/<topic>.md`, categories from `subject.md` → "Knowledge categories"
(add a new category only if none fits). `learning/knowledge/index.md` lists every topic file.
Topic slugs are lowercase-kebab. Check `index.md` first and reuse an existing topic file when it matches.

## Topic file format
```markdown
# <Topic>

## Summary
<3–8 bullets: the learner's current understanding — merge/rewrite as knowledge grows>

## Q&A
### <question in the learner's words> — YYYY-MM-DD
<concise answer, 3–10 lines; optional example ≤15 lines>
- **Applies to:** <where this matters for the final goal/project>
- **See also:** [[other-topic]] · <reference link>

## Pitfalls
- <mistake> → <why> → <how to avoid>

## Know-how
- <practical tip>
```

## save
1. Choose category + topic file; create it if new and add it to `index.md`.
2. Add the entry to the right section. Write it **standalone** — readable months later without this conversation.
3. Update the Summary if the overall picture changed. No duplicates: improve an existing entry instead.
4. Confirm in one line: `📝 Saved → learning/knowledge/<path> (<section>)`.

## find <query>
Search `learning/knowledge/`; show matches as file + heading + 2-line excerpt. Nothing found → say so and offer
to answer and save.

## review <topic>
Show the Summary, then ask 2–3 questions from its Q&A/Pitfalls.

## index
Rebuild `index.md`: topics by category, entry counts, last-updated date.
