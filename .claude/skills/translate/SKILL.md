---
name: translate
description: Translate the learner's learning documents (knowledge base, lesson notes, glossary, subject files, tool READMEs, presentation source) into another language using learning/glossary.md for consistent terms, writing into a parallel folder so the originals stay. Use when the learner says "translate", "convert the docs to English", or prepares material for other people.
---

# /translate [language] [path]

Default language: `subject.md` → "Language" → "Translate later to". Default scope: ask, offering
`learning/knowledge/`, `learning/glossary.md`, `learning/showcase.md`, tool READMEs, `presentation/slides.md`.

## Rules
1. **Originals stay.** Write to `translations/<lang-code>/<same relative path>` (e.g. `translations/en/learning/knowledge/…`).
   Translate in place only if the learner asks for it explicitly.
   Exception: `presentation/slides.md` → write `presentation/slides.<lang-code>.md` next to it, so
   `/present` can build that version too.
2. **Glossary first.** Every term in `learning/glossary.md` is translated exactly as the glossary says. If the
   glossary has no column for the target language, add it first and show the learner the new column
   before translating the files.
3. **Do not translate:** code, commands, file paths, identifiers, numbers with units, quoted error messages,
   Markdown links' targets, front-matter keys.
4. **Same meaning, same structure.** Keep headings, lists, tables and links the same, so files can be compared
   side by side. Follow the `CLAUDE.md` writing rules in the target language (plain, short sentences).
5. **Flag doubts** inline as `<!-- TRANSLATION CHECK: … -->` instead of guessing, and list them at the end.

## Steps
1. List the files in scope with their size; confirm the scope with the learner if it is more than ~20 files.
2. Translate file by file. For large scopes, do it in batches and report progress after each batch.
3. Re-check: every glossary term used consistently (search for the source-language terms that should be gone),
   links still work, code blocks unchanged.
4. Report: number of files, output folder, and the list of `TRANSLATION CHECK` items.
