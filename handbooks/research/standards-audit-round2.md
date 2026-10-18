# Standards and usability audit, round 2

Scope: the specialists chapter and its 15 imported APIs; checked against README.md, TEAM.md, and the actual header implementations.

## Documented standards

KACTL asks for useful, short, fast, tested, modifiable code, with terse names/macros, 63-character code lines and tab indentation. Upstream source and attribution were preserved. The chapter's prelude matches the imported macros/types. No new generic wrappers or duplicate implementations were introduced.

TEAM.md requires review before merge; this audit does not authorize merging. The original notebook's 25-page target is not automatically a combined limit for separate companion volumes.

## Findings addressed

The specialists chapter mixed printed code with long contracts for unprinted alternatives. During a contest these entries consume search space without supplying the referenced implementation. Removed those catalogs; retained all 15 printed imports. Notes now identify the API and its main input/output constraint in one or two source lines per import (under 500 words total prose).

Checked the easily miscopied contracts directly: KMP's nul separator; Aho's uppercase alphabet; SCC's descending component IDs and reused callback vector; TwoSat's bitwise-negated literals; matching's right-side result vector; Dinic's additional residual flow and unsafe self-loop indexing; binary lifting's root parent; geometry's integer truncation and overflow; maximum-line queries; and matrix column-vector orientation. No API mismatch found in the retained descriptions.

## Judgment calls, not violations

Removing the unprinted catalog is a usability choice, not a repository rule. Keeping upstream terse names is consistent with this repository, even where general application-code advice would prefer longer identifiers. The warning on recursive depth remains because several printed graph algorithms recurse. Geometry warnings remain because integer point types can silently produce incorrect coordinates.

This pass does not establish algorithm correctness through prose review; root verification covers compilation, stress checks, and rendered-page inspection separately.
