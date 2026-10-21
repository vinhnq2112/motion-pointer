# Example 4 — Architecture Decision Record

# ADR-0002: Parallelism strategy

- **Status:** Accepted (2026-10-15)
- **Related:** REQ-N-001 (throughput), REQ-N-002 (peak memory)

## Context
Sequential processing measured 14 images/s on the reference laptop (decode dominates, ~70 ms/image).
REQ-N-001 needs ≥ 100 images/s. The machine has 8 hardware threads. Each decoded 12 MP image is ~36 MB.

> 💡 **Why:** context states the *forces* with real numbers — the decision should follow from them.

## Options

### A. Stay sequential, optimize decoding
- ➕ simplest, no concurrency bugs
- ➖ even 2× faster decode = 28 images/s; cannot meet REQ-N-001

### B. One thread per file (`std::thread` for every image)
- ➕ easy to write
- ➖ 1000 files → 1000 threads; peak memory = 1000 × 36 MB → violates REQ-N-002; thread creation overhead

### C. Fixed pool of N workers pulling from a shared queue of file paths
- ➕ bounded memory (≈ N × 36 MB), uses all cores, predictable
- ➖ needs a thread-safe queue and careful error collection; more code

## Decision
Option C with N = `std::thread::hardware_concurrency()`, overridable with `--jobs`.

## Consequences
- Expected ~8 × 14 ≈ 110 images/s → meets REQ-N-001 with small margin; benchmark must confirm.
- Peak memory ≈ 290 MB → close to the 300 MB budget; `--jobs` lets users trade speed for memory.
- Each worker owns one reusable thumbnail `Image` (see module `resize`).
- Revisit if decoding becomes I/O-bound (then more threads than cores may help).

> 💡 **Why:** at least two real options, honest downsides of the chosen one, and a trigger for revisiting.
> A superseded ADR is never deleted — it gets `Status: Superseded by ADR-00NN`.
