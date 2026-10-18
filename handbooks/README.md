# Mexico finals companions

Two references built around the team's reported weaknesses and the 2017-2025 Latin American regional research. The original KACTL notebook remains available unchanged.

| Book | Use it for | Printable file |
|---|---|---|
| [Math reasoning and formulas](math.md) | Exact arithmetic, divisibility, modular structure, counting, expectation, proofs, decimal representations, geometry predicates | [math-handbook.pdf](pdf/math-handbook.pdf) |
| [Algorithms and implementations](implementations.md), with [specialist appendix](specialists.md) | Choosing and implementing graph/range/DP methods; contracts, examples, pitfalls; selected existing KACTL code | [implementation-handbook.pdf](pdf/implementation-handbook.pdf) |

Start with the recognition tables, then check the relevant API's assumptions. Both PDFs contain page-numbered contents and code module lookup. `include/core.hpp`, `include/math.hpp`, and `include/digit_dp.hpp` are the authoritative editable C++17 helpers. The printed source is generated directly from these files. Keep both books beside the original `kactl.pdf` for wider specialist coverage.

## Conventions

- C++17, zero-based indices, half-open ranges `[l,r)` unless an entry says otherwise.
- Graphs use adjacency lists. Insert both directions for undirected edges.
- Distances and counts use `long long`; intermediate arithmetic sometimes uses compiler extension `__int128`. Bounds still matter.
- Core helpers have no dependence on KACTL macros. Existing KACTL snippets retain their own aliases, macros, dependencies, and authorship. Do not mix them blindly.
- The headers use `#pragma once` and inline free functions. Copy each required module once; selected original KACTL snippets retain their own single-translation-unit conventions.
- Local randomized tests are recorded in `VALIDATION.md`. They are not online-judge submissions. KACTL status comments describe upstream testing, not new tests performed for these books.

The material limits for the specific finals have not been supplied. These are full reference editions; confirm page, binding, and font rules before printing, then select sections the team can use. No claim is made that every specialist topic will be learned before November 8.

## Build and test

From the repository root:

```sh
python3 -m pip install -r handbooks/requirements.txt
python3 handbooks/build.py
sh handbooks/test.sh
SANITIZE=1 sh handbooks/test.sh
```

Build requires Python and ReportLab, without LaTeX or network access after installation. Tests require a C++17 compiler; set `CXX=clang++` or `CXX=g++`. PDF output is in `handbooks/pdf/`. The existing `make kactl` target is unchanged.

The companion workflow builds both PDFs and runs the helper tests on pull requests. Its artifact is separate from the existing KACTL release artifact.

## Scope and sources

The research inventoried 116 problems across nine annual regional sets. Technique families overlap, and some classifications remain provisional. The books use the observed patterns to choose coverage, rather than claiming to predict the next set.

The math book emphasizes representation and proof because Fuzzy Factorization primarily requires an approximation argument. The implementation book includes basic traversal and shortest paths because the team reported limited graph experience. Advanced templates are retained for team members already able to adapt them.

See [provenance](research/provenance.md), [coverage audit](research/coverage-audit.md), and [validation](VALIDATION.md). Original code notices are preserved. New authored companion prose and helper code are CC0, excluding original KACTL source excerpts, whose individual notices still apply.
