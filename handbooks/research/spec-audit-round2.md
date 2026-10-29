# Requirement audit — code-first revision

Scope: `origin/main...HEAD`, plus the current working revision, 2026-10-18.

- **“at least one for math and other for implementations”**: satisfied by `content/finals-math.tex` and `content/finals-implementations.tex`, with separately generated PDFs under `handbooks/pdf/`. The implementation volume also imports `content/finals-specialists/chapter.tex`.
- **“repo format for latex in landscape”**: the two entry points use native `kactlpkg` and three-column `multicols*`; the landscape setting belongs to the shared package. Root must confirm rendered orientation and readability after this revision. Markdown sources are supplementary and must not be mistaken for the printed edition.
- **“be clear about what does each thing”**: the revised math chapter retains every helper import, groups formulas by problem trigger, and places concise Use/Requires/Pitfall notes immediately before code. It preserves important contracts: prime modulus and table limit, CRT incompatibility/overflow, exact-decimal parser limits, positive factorization inputs, geometry bounds, and trial-division cost.
- **“a lot of text ... improve the code part”**: `content/finals-math/chapter.tex` was reduced from 1,554 to 701 whitespace-delimited words, including equations/LaTeX. Long worked examples and tutorial paragraphs were removed; Fuzzy Factorization and decimal-period derivations remain as compact identities. Generator/code formatting and other chapters are owned by the root revision.
- **Research relevance**: the printed math chapter retains the two explicitly requested 2025 weaknesses, Fuzzy Factorization and Displaying Decimals; it also covers exact arithmetic, gcd/prime exponents, modular arithmetic, counting/contributions, expectation, digit remainder states, and integer geometry. `handbooks/research/coverage-audit.md` records overlapping regional topic evidence and specialist coverage limits.

## Limits requiring honest delivery

This is a contest reference, not assurance of every regional topic or independent mastery. `DecimalPeriod.h` computes an individual fraction; the full 2025 D aggregation remains unbenchmarked and is labeled accordingly. Specialist references preserve upstream assumptions and may require dependencies/model adaptation. A successful PDF build alone does not test algorithms: the root should report actual test results, visual checks, and remaining limits separately. No paper/material rule for the finals was supplied, so compliance with a page cap is not established.
