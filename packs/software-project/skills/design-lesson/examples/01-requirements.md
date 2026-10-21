# Example 1 — Requirements: weak vs good

## ❌ Weak version

```
thumbgen requirements
- The tool should be fast.
- It should support all image formats.
- It must be easy to use.
- Low memory usage.
- Use a thread pool to process images.
- Maybe add a GUI later.
```

Problems:
| Line | Problem |
|---|---|
| "should be fast" | Not measurable. Fast compared to what, on what input, on what machine? |
| "all image formats" | Unbounded scope — there are hundreds. Can never be "done". |
| "easy to use" | Subjective with no test. |
| "Low memory usage" | No number, no conditions. |
| "Use a thread pool" | This is a **design decision**, not a requirement. It belongs in an ADR. |
| "Maybe a GUI later" | Neither in scope nor explicitly out of scope — creates confusion. |
| all | No IDs → can't be referenced from design or tests. |

---

## ✅ Good version

# thumbgen — Requirements v0.2

## 1. Vision
Generate thumbnails for large photo folders quickly on a normal laptop, from the command line,
so they can be used by a static photo gallery.

> 💡 **Why:** one or two sentences that every later decision can be checked against.

## 2. Users & use cases
- **UC-1** — A photographer runs `thumbgen ./photos --size 256` and gets a thumbnail for every supported image.
- **UC-2** — A build script runs thumbgen in CI and must detect failure from the exit code.
- **UC-3** — The user re-runs on the same folder; only new or changed images are processed.

> 💡 **Why:** use cases come from user *goals*. Requirements are derived from them, so nothing is invented.

## 3. Functional requirements
| ID | Requirement | Acceptance criteria | Source |
|---|---|---|---|
| REQ-F-001 | The tool shall read PNG and JPEG files from an input folder (non-recursive). | Folder with 3 PNG + 2 JPEG + 1 TXT → 5 thumbnails, TXT ignored and reported. | UC-1 |
| REQ-F-002 | The tool shall write thumbnails whose longest side equals `--size` (default 256), keeping aspect ratio. | 1000×500 input, size 256 → 256×128 output. | UC-1 |
| REQ-F-003 | The tool shall exit with code 0 on success, 1 if any file failed, 2 on invalid arguments. | Corrupted JPEG in input → exit code 1, other files still processed. | UC-2 |
| REQ-F-004 | The tool shall skip images whose thumbnail is newer than the source. | Second run with no changes processes 0 files. | UC-3 |

> 💡 **Why:** one statement per row, "shall" = mandatory, each has a concrete acceptance test, and a
> traceable source. No word about *how* (no thread pool, no library names).

## 4. Non-functional requirements
| ID | Requirement | Target | Conditions | How measured |
|---|---|---|---|---|
| REQ-N-001 | Throughput | ≥ 100 images/s | 1000 JPEGs of 12 MP, size 256, 8-core laptop, SSD | `bench/throughput` median of 5 runs |
| REQ-N-002 | Peak memory | ≤ 300 MB | same input as REQ-N-001 | peak working set (Task Manager / `GetProcessMemoryInfo`) |
| REQ-N-003 | Platforms | Windows 10+, Ubuntu 22.04+ | x64 | CI builds and tests on both |
| REQ-N-004 | Usability | A first-time user produces thumbnails from `--help` alone in < 2 min | 3 test users | manual test |

> 💡 **Why:** every NFR has metric + target + conditions + method. Even "easy to use" became testable.

## 5. Constraints
- C++17, CMake, no runtime dependencies beyond the executable.

## 6. Non-goals (v1)
- GUI, recursive folders, formats other than PNG/JPEG, video, cloud storage.

> 💡 **Why:** saying "no" explicitly prevents scope creep and arguments later.

## 7. Open questions
- Should EXIF orientation be respected? (affects REQ-F-002) — decide before v0.3.
