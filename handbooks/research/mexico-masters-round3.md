# Mexico Finals, repechaje, and Masters companion audit

Audit date: 2026-10-18. This is a small statement-based sample, not a complete corpus or topic-frequency study. The walkthroughs below are independent derivations from the published statements; I did not consult jury solutions or claim judge acceptance.

## Keep the events distinct

ICPC México lists Gran Premio qualifiers, repechaje, Final Regional de México, and TCMX as separate events. In 2024 its repechaje had 11 problems; the Final Regional listing has 12 and describes the LATAM regional hosted by Brazil. The latter is the Mexico edition of a Latin American contest, not a Mexico-only set. See the [ICPC México contest index](https://www.icpcmexico.org/contests), [2024 Repechaje](https://www.icpcmexico.org/contests/gpmx-2024-repechaje), and [2024 Final Regional](https://www.icpcmexico.org/contests/regional-mexico-2024). The ICPC Contest Archive series index lists eight Mexico Finals editions from 2019–2026, but does not establish complete coverage of repechajes or Masters.

ICPC Masters México is a separate alumni contest under regional rules. The 2022 problem book is titled “ICPC Masters Mexico 2022”; the 2023 event was explicitly announced for ICPC alumni and has its own [ICPC Mexico announcement](https://regional2023.icpcmexico.org/masters) and [Codeforces gym](https://codeforces.com/gym/104758). The [2026 Masters page](https://masters2026.icpcmexico.org/en/) describes a worldwide alumni championship. These are useful practice, not student Finals history.

## What two actual statements show

The [2022 Masters problem book](https://acm.iteso.mx/share/ICPCMXMasters2022_problems.pdf) gives concrete training examples. Page numbers below are the printed problem pages.

**F — Flip, p. 9:** each click toggles a cell in a fixed 3×3 board and its orthogonal neighbors; input has at most 50 boards. Enumerate all `2^9 = 512` click subsets, simulate each subset, and take the minimum number of clicks producing the target. Clicking the same cell twice cancels, so an optimal solution never needs more than one click per cell. This is a small exhaustive search, with no graph template needed. The statement itself supplies no unusual numeric bound beyond the fixed board and `P≤50`.

**H — Hidden Number, p. 11:** find the smallest positive value not representable as a subset sum; `N≤10^6`, each value is in `[1,10^6]`. Sort ascending and maintain that all sums from 1 through `reach` are constructible. If the next value `x≤reach+1`, including it extends the covered interval to `reach+x`; if `x>reach+1`, `reach+1` is the first gap and cannot be formed. Return the first gap, or `reach+1` after all values. The proof is by induction on the sorted prefix; the total fits 64-bit (`≤10^12`). This is a useful greedy proof and overflow exercise, not a BFS task.

## Companion coverage and practical gap

The implementation companion explicitly teaches ordinary/grid/multi-source BFS and introduces DP, range structures, DSU/Kruskal, topological order, and Dijkstra. The math companion teaches exact bounds and how to prove greedy rules, but does not give the sorted-prefix proof for “smallest nonrepresentable subset sum.” The companion therefore offers the right reasoning tools for H, while a worked example would make their use concrete. F exposes another small teaching gap: fixed-size exhaustive search and transition simulation are simple, approachable techniques that should be taught before specialist templates.

The original KACTL graph chapter imports topological sort and many advanced graph routines, but no BFS. The data-structures chapter comments out ordinary `UnionFind.h` and imports rollback union-find instead. Thus the handbooks add foundational material absent from the default printed KACTL notebook; an included KACTL specialist should not be mistaken for an appropriate first graph lesson. See the native [graph chapter](https://github.com/pol-cova/kactl/blob/7532cfe6c1eda7279d6e182b83d41e47e7760658/content/graph/chapter.tex) and [data-structures chapter](https://github.com/pol-cova/kactl/blob/7532cfe6c1eda7279d6e182b83d41e47e7760658/content/data-structures/chapter.tex).

For a team with BFS as its only graph experience, keep the progression practical: grid and state-graph BFS; then DSU/Kruskal, topological order, and Dijkstra; in parallel, practice tiny exhaustive search, sorted greedy with proof, prefix/frequency methods, and beginner DP. Treat matching/flow, SCC/2-SAT, LCA, advanced geometry, and transforms as later study tied to a specific past statement. This ordering is a coaching judgment, not a prediction about a future set.

## Evidence boundary

The event separation and listed problem constraints come from organizer/event materials. F and H approaches above are statement-derived independent solutions, not official-editorial claims or tested submissions. Two examples cannot support recurrence or frequency claims; a wider audit needs the exact edition list and problem-by-problem statement/editorial review, with missing years reported separately. No title-based algorithm labels are used here.

Implemented after this audit: the print edition now includes the sorted-prefix invariant and a greedy C++ example for H, plus the 512-mask recipe for F. The exact printed H snippet passed 1,000 exhaustive subset-oracle comparisons with sanitizers. These local tests do not constitute a judge submission.
