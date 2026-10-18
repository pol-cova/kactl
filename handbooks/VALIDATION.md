# Validation of the October 18 edition

## Commands run locally

```sh
CXX=clang++ sh handbooks/test.sh
CXX=clang++ SANITIZE=1 sh handbooks/test.sh
python3 handbooks/build.py
```

All four test programs passed in optimized C++17 builds and with AddressSanitizer plus UndefinedBehaviorSanitizer. Flags include `-Wall -Wextra -Wshadow`. The sanitizer run uses `-O1 -g`; the ordinary run uses `-O2`. Tests use fixed seeds and assertions, without `NDEBUG`.

## Independent oracles and boundaries

| Program | Checks |
|---|---|
| core_test.cpp | 350 random directed graph instances against Floyd-Warshall or repeated relaxations; 100 grids against explicit graph BFS; 200 component/DSU cases against transitive closure; 150 small minimum spanning forests against exhaustive edge subsets; 100 DAG orders and injected cycles; 200 trees against parent climbing and BFS distances; 250 arrays with naive interval queries/updates, windows, frequencies, compression, binary search; 300 knapsack/LIS instances against subset enumeration or independent DP. Includes empty ranges, noncommutative segment merges, zero-weight 0-1 items, and INF boundaries. |
| math_test.cpp | 8,040 small signed division pairs against mathematical floor/ceil; inverse and modular powers over moduli 2..80; 62,500 shared-factor CRT combinations against enumerated solutions; factorizations/SPF for 1..10,000; totients through 1,000 by coprime counts; 14,520 fraction shapes against direct long division; binomial values against Pascal recurrence; 3,000 inversion arrays against pairs; 2,000 Fuzzy constructions against exact 128-bit inequalities and a 1001-digit exponent check; 3,000 geometry cases and large-coordinate boundaries. |
| digit_dp_test.cpp | 1,000 pattern/modulus/leading-zero configurations against exhaustive completion enumeration, including smallest reconstruction and explicit one-digit-zero behavior. |
| kactl_test.cpp | 600 random strings against naive KMP/Z answers; 350 SCC graphs against reachability; 350 2-SAT cases against all assignments; 350 max-flow cases against all s-t cuts; 350 matching cases against exhaustive matching. Boundary checks for hull/area/intersection, Aho duplicate patterns, binary lifting, 150 line insertions with naive maxima over 51 query points, and matrix Fibonacci powers 0..39. |

The selected printed KACTL headers compile in the adapter. This does not claim that all existing KACTL algorithms were revalidated. Original upstream status comments remain upstream evidence.

## PDF checks

The final math book has 13 A4 pages; the implementation book has 32. Both have page-numbered contents, PDF bookmarks, recognition tables, and source-derived code modules. Rendered pages were inspected for layout, code clipping, tables, and page furniture. Text extraction checks reject unexpanded code directives and missing-glyph squares. Arithmetic asterisks are preserved, including the divisor example `tau=4*3=12`.

The source and build scripts are editable. KACTL source docblocks with LaTeX markup are omitted from printed code excerpts; source paths, author names, and license labels remain printed, while original files retain the complete notices.

## Limits

These are local oracle checks, not new online-judge acceptances. Full maximum-constraint Displaying Decimals aggregation is excluded. Recursive specialist snippets can need stack planning. Arithmetic and memory bounds remain part of each caller's contract. No full contest rehearsal or event-specific printed-material compliance has been claimed. CI configuration is provided; local tests alone do not establish that a remote workflow has completed.
