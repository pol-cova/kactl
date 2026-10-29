# Code correctness audit, second round

Scope: October 18 finals helpers in `handbooks/include`, their generated `content/finals-*` modules, selected upstream KACTL contracts and local tests. Compared `origin/main...HEAD`; original algorithm sources are unchanged. Read `TEAM.md`. This is local evidence, not new judge acceptance or a contest rehearsal.

## Confirmed finding

**P2 — Sliding-window accumulator overflows even when every entry, limit, and answer fit signed 64 bits.** Before repair, `handbooks/include/core.hpp:258` executes `sum += a[r]` in `long long`. Reproduction:

```cpp
count_subarrays_at_most({LLONG_MAX,1},LLONG_MAX); // expected 2
```

UBSan reports `9223372036854775807 + 1` at that line; wrapping can count the invalid two-element window. Although the book's global arithmetic contract asks callers to bound intermediates, this particular intermediate is larger than the limit immediately before shrinking, making its failure easy to miss. Fixed by shrinking before addition using `sum > limit-a[r]`; entries above the limit reset the window. This keeps the accumulator within the limit without a wider type. The regression suite passes.

A second integration defect appeared in CI: the DSU member `sz` collided with KACTL’s `sz(x)` macro in the constructor. Renamed it `sizes` and added that macro to the generated-module integration test.

## Added verification

`handbooks/tests/audit_test.cpp` adds:

- 2,000 arrays with values near `LLONG_MAX` and near zero; independent quadratic oracle accumulates in 128 bits. Includes explicit overflowing windows and zero-valued boundary windows.
- Signed floor/ceiling division across `LLONG_MIN`, `LLONG_MAX`, and mixed-sign divisors, checked by exact quotient inequalities rather than floating point.
- Fixed-point parsing at both signed 64-bit endpoints, including 18 fractional places and long leading-zero input.
- Full-width modular products; powers with exponent zero/modulus one; CRT with modulus one, identical large moduli, and an unrepresentable LCM.
- Fenwick order statistics at target `LLONG_MAX`, empty Fenwick search, and binary search over `[0,LLONG_MAX)`.
- Explicit digit-DP one-digit-zero behavior for both leading-zero policies.

The original **28 generated headers compiled individually** with only their declared dependencies under Clang C++17 and `-Wall -Wextra -Wshadow`. This supplements the existing combined-header integration test, which could hide missing includes.

## Contract checks

No additional reproducible correctness defect was identified within documented domains. Specifically: Dijkstra's finite distances must be below its `INF`; grids must be rectangular with flattened indices fitting `int`; components requires undirected input; tree/LCA requires a connected tree; compressed rank is an insertion position unless the value exists; Fenwick selection needs nonnegative frequencies; prime combinations requires a prime modulus and `n<p`; geometry needs its coordinate/product bounds; digit reconstruction forbids even one-digit zero when requested. These restrictions must remain visible beside their code when prose is cut.

Specialist source checks confirm the important distinctions: Dinic mutates residual state and subsequent calls return additional flow; 2-SAT uses `~x` literals; SCC IDs are reverse-topological; integer segment construction truncates nonintegral intersections; polygon area requires nonempty input. The original algorithms remain unchanged.

## Test result

The first sanitizer execution reproduced the sliding-window finding. Final repaired-suite results are recorded by the main editor in `handbooks/VALIDATION.md`; do not treat the initial failure as a passing run.

After adding interval union, all 29 generated headers compile individually with KACTL macros and aliases under Clang C++17. The GNU compiler on this Mac failed to locate SDK assert.h before reading our code; GNU validation belongs to CI.
