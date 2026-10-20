# Regional evidence audit: printed handbook coverage

Audit date: 2026-10-18. Scope: compare the native print sources `content/finals-math/chapter.tex`, `content/finals-implementations/chapter.tex`, and `content/finals-specialists/chapter.tex` with the 2017–2025 Latin American Regional set. This is not a forecast for November 8, 2026.

## Coverage and evidence

The printed math chapter has compact, usable entries for exact decimals; floor/ceiling and bounds; modular powers/inverses and non-coprime CRT; factorization/totient and gcd-class aggregation; remainder DP and rational decimal periods; the 2025 F approximation construction; counting, inversions, expectation, proof checks, and integer geometry. The printed implementation chapter covers BFS, grids, weighted shortest paths, DAGs, DSU/Kruskal, tree order/LCA, compression, prefix/difference arrays, windows, binary search, Fenwick/segment trees, knapsack, counting loop order, LIS, and digit-pattern reconstruction. The printed specialists chapter supplies KMP/Z/Aho, SCC/2-SAT, Hopcroft–Karp/Dinic, binary lifting, geometry primitives, line container, and matrix powers with compact contracts.

The existing atlas classifies all 116 problems across nine sets into overlapping families, including graph/tree modeling 30, ordered/range/offline work 25, exact arithmetic 26, DP 22, counting/expectation 20, geometry 18, and matching/flow 8. These are curated memberships, some provisional; they are not official tags, exclusive counts, or predictions. [Corpus report](../../../outputs/research-results.md) · [priority audit](../../../outputs/review-priorities.md)

Primary-source spot checks below support the examples but do not establish historical frequency:

| Example and verified source | Print usefulness and gap |
|---|---|
| 2024 I, **Inversion Insight**: rank permutations by inversion count then lexicographically, N≤2e5, K≤4e18. [Statement](https://codeforces.com/gym/105505/attachments/download/28244/contest_onesided.pdf) · [solution sketches](https://codeforces.com/gym/105505/attachments/download/28286/icpcla24_solution_sketches.pdf) | Printed inversions and Fenwick support the ingredients, but there is no worked “count a block, skip it, select the next value” unranking example. The official sketch gives an experimental bound that the team's independent audit disproved at n=101,K=4e18 (x=16); preserve the proof-based bound caution. |
| 2024 J, **Jigsaw of Shadows**: real shadow intervals, union length, error tolerance 1e-4; editorial uses sorting and interval merging in O(N log N). Same [statement](https://codeforces.com/gym/105505/attachments/download/28244/contest_onesided.pdf) · [sketches](https://codeforces.com/gym/105505/attachments/download/28286/icpcla24_solution_sketches.pdf). | Printed geometry explains exact predicates, segment membership, numeric bounds, and floating-point caveats, the revised implementation chapter now includes a tested interval merge helper. Note the conversion `L=H/tan(theta)` and the stated output tolerance. |
| 2025 D, **Displaying Decimals**: expected digits for all ordered ratios, N≤1e5, modulo 998244353. [Official statement](https://scorelatam.naquadah.com.br/latam-2025/contest.pdf) | Printed decimal-period and expectation formulas cover the math; the chapter explicitly says maximum-size aggregation is unbenchmarked. Treat it as concept support, not a ready accepted full solution. |
| 2025 F, **Fuzzy Factorization**: 1,000-digit X, relative error ≤1e-9, factors ≤1e18. [Official statement](https://scorelatam.naquadah.com.br/latam-2025/contest.pdf) | The printed ten-leading-digit construction and bound directly address the reduction. Keep this high in math practice. |

Implemented after this audit: `merge_intervals` now appears in the printed implementation chapter, with finite-endpoint and half-open-range contracts. Its tests compare 1,000 random cases with a point-coverage oracle.

## Preparation priorities

For a novice graph/math team with three weeks, first practice the printed basics under time: (1) exact arithmetic, remainders, counting and proof of bounds; (2) BFS/Dijkstra, trees and simple range structures; (3) DP state and reconstruction. Then use specialists for one bipartite assignment model and one flow/cut model, drawing the graph and proving the correspondence before coding. The native specialists chapter provides APIs, not full problem reductions. Defer HLD, optimized DP, and deep number theory until those basics are reliable.

Interval union is now included. A full inversion-unranking walkthrough remains in study scope, outside the compact print edition. For each practice problem, check a tiny brute-force case, the reduction invariant, complexity/memory, and a boundary case.

## Evidence limits

I read the current native chapter sources directly; supplementary Markdown tutorials are not used to claim print coverage. The linked 2024 statement and official solution-sketch PDFs and 2025 official statement were opened for these examples. The historical count claims come only from the existing atlas/analyses, not this four-example sample. No all-history frequency claim or claim about the next contest follows from the spot checks. The 2025 D source supports the task shape, not the full aggregation runtime.
