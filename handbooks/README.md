# Mexico finals companions

Three native KACTL landscape books built around the team's reported weaknesses and the 2017-2025 Latin American regional research. The original KACTL notebook remains available unchanged.

| Book | Use it for | Printable file |
|---|---|---|
| [Worked patterns](../content/finals-patterns.tex) | Recognize a model, derive it, trace an example, adapt tested code, and identify changes that invalidate it | [patterns-workbook.pdf](pdf/patterns-workbook.pdf) |
| [Math reference](../content/finals-math/chapter.tex) | Exact arithmetic, divisibility, modular structure, counting, expectation, proofs, decimal representations, geometry predicates | [math-handbook.pdf](pdf/math-handbook.pdf) |
| [Implementations](../content/finals-implementations/chapter.tex), with [specialists](../content/finals-specialists/chapter.tex) | Choosing and implementing graph/range/DP methods; contracts, examples, pitfalls; selected existing KACTL code | [implementation-handbook.pdf](pdf/implementation-handbook.pdf) |

Start with the worked book when the algorithm is unclear. Use its recognition routes, then the worked case, and finally the matching implementation. The math and implementation references keep three columns for lookup. The worked book uses two columns and 10pt prose for derivations. All three use `content/tex/kactlpkg.sty`, landscape pages, numbered contents, and native code imports. There is no page-count target.

The worked book includes 14 cases: signed prefix counts; approximation; single-fraction decimals; smallest missing subset sum; exact scaling; parity-state BFS; hazard deadlines; Enigma reconstruction; Fundraising dominance DP; interval union; toggle subsets; assignment/flow; capacity search; and budget DP. Named contest problems and synthetic exercises are labeled separately. Displaying Decimals is explicitly a component walkthrough, not a complete all-pairs implementation.

`include/core.hpp`, `include/math.hpp`, `include/digit_dp.hpp`, and `include/case_studies.hpp` hold the C++ helpers. The builder generates the printed headers directly from these files. Edit native chapter/fragment files for printed prose; the longer Markdown files remain background reading. Keep the original `kactl.pdf` for wider specialist coverage.

## Conventions

- C++17, zero-based indices, half-open ranges `[l,r)` unless an entry says otherwise.
- Graphs use adjacency lists. Insert both directions for undirected edges.
- Distances and counts use `long long`; intermediate arithmetic sometimes uses compiler extension `__int128`. Bounds still matter.
- Core helpers have no dependence on KACTL macros. Existing KACTL snippets retain their own aliases, macros, dependencies, and authorship. Do not mix them blindly.
- The headers use `#pragma once` and inline free functions. Copy each required module once; selected original KACTL snippets retain their own single-translation-unit conventions.
- Local randomized tests are recorded in `VALIDATION.md`. They are not online-judge submissions. KACTL status comments describe upstream testing, not new tests performed for these books.

The user confirmed there is no page limit. Explanations now prioritize choosing and adapting a method; advanced source availability still does not establish that the team can use it under contest conditions.

## Build and test

From the repository root:

```sh
make handbooks
sh handbooks/test.sh
SANITIZE=1 sh handbooks/test.sh
```

Build uses the same Python, pdflatex, LaTeX packages, shell escape, and GNU cpp hashing as the original KACTL. No ReportLab is used. On macOS, the builder can use installed `cpp-14` or another versioned GNU cpp for hashing. Tests require a C++17 compiler; set `CXX=clang++` or `CXX=g++`. PDF output is in `handbooks/pdf/`. The existing `make kactl` target is unchanged.

Native main files: `content/finals-math.tex`, `content/finals-implementations.tex`, and `content/finals-patterns.tex`. The first two import their `chapter.tex`; implementations also imports `content/finals-specialists/chapter.tex`. Use `python3 handbooks/build.py --generate-only` to refresh modules without compiling PDFs.

The companion workflow builds all three PDFs and runs the helper tests on pull requests. Its artifact is separate from the existing KACTL release artifact.

## Scope and sources

The research inventoried 116 problems across nine annual regional sets. Technique families overlap, and some classifications remain provisional. The books use the observed patterns to choose coverage, rather than claiming to predict the next set.

The math book emphasizes representation and proof because Fuzzy Factorization primarily requires an approximation argument. The implementation book includes basic traversal and shortest paths because the team reported limited graph experience. Advanced templates are retained for team members already able to adapt them.

The worked edition adds five tested adaptations: strict donation chains, hazard-safe grid paths, toggle subsets, capacity search, and signed exact-sum counts. Historical support and remaining gaps are recorded in [regional evidence](research/regional-evidence-round3.md) and [Mexico/Masters evidence](research/mexico-masters-round3.md). These are source-checked samples, not an exhaustive Masters/repechaje topic census.

See [provenance](research/provenance.md), [coverage audit](research/coverage-audit.md), and [validation](VALIDATION.md). Original code notices are preserved. New authored companion prose and helper code are CC0, excluding original KACTL source excerpts, whose individual notices still apply.

Worked-case source ledgers: [math](research/math-pattern-sources.md), [graphs and DP](research/graph-dp-pattern-sources.md), [model changes](research/model-pattern-sources.md).
