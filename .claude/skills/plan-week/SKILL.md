---
name: plan-week
description: Plan the learner's coming week into concrete sessions from subject/roadmap.md, progress, weak spots and recent journal entries. Use at the start of a week or when the learner asks what to do next.
---

# /plan-week

1. Read `subject/roadmap.md`, `learning/progress.md`, `learning/weak-spots.md`, and the last 2–3 journal entries.
2. Ask how many hours the learner actually has this week and on which days (default: the budget in `subject.md`).
3. Produce a plan:

   | Session | Duration | Activities | Done when |
   |---|---|---|---|

   Per session: ~10 min `/quiz` warm-up · 40–60% hands-on · ≤30% new theory · ~10 min wrap-up.
   Hands-on share grows as a phase advances. Include due items from any pack tracks (e.g. design lessons).
4. Save it under "This week" in `learning/progress.md` (replace the previous plan), set "Week of" to this
   week's Monday, and reset "Hours this week". Create these fields if they are missing.

## Sprint mode
If `subject.md` → "Course mode" is **sprint**, plan the **whole course** once, one row per day from
`subject/roadmap.md` (Day 1, Day 2, …). For a 1-hour day: ~5 min warm-up · ~20 min lesson · ~30 min exercise
or tool work · ~5 min wrap-up with a showcase entry. Put any learning-tool build (`/lab`) before Day 1's
exercise, and the final deliverable (`/present`) plus `/checkpoint` on the last day. Save it under
"This week" as "Course plan". If the learner falls behind, re-plan the remaining days honestly (merge or cut
topics — say which) instead of squeezing more into one hour.
