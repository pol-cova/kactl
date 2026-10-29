# Validation of the October 28 edition

## Commands run locally

```sh
CXX=clang++ sh handbooks/test.sh
CXX=clang++ SANITIZE=1 sh handbooks/test.sh
python3 handbooks/build.py
```

All ten test programs passed in optimized C++17 builds and with AddressSanitizer plus UndefinedBehaviorSanitizer. Flags include `-Wall -Wextra -Wshadow`. The sanitizer run uses `-O1 -g`; the ordinary run uses `-O2`. Tests use fixed seeds and assertions, without `NDEBUG`.

## Independent oracles and boundaries

| Program | Checks |
|---|---|
| core_test.cpp | 350 random directed graph instances against Floyd-Warshall or repeated relaxations; 100 grids against explicit graph BFS; 200 component/DSU cases against transitive closure; 150 small minimum spanning forests against exhaustive edge subsets; 100 DAG orders and injected cycles; 200 trees against parent climbing and BFS distances; 250 arrays with naive interval queries/updates, windows, frequencies, compression, binary search; 300 knapsack/LIS instances against subset enumeration or independent DP. Includes empty ranges, noncommutative segment merges, zero-weight 0-1 items, and INF boundaries. |
| math_test.cpp | 8,040 small signed division pairs against mathematical floor/ceil; inverse and modular powers over moduli 2..80; 62,500 shared-factor CRT combinations against enumerated solutions; factorizations/SPF for 1..10,000; totients through 1,000 by coprime counts; 14,520 fraction shapes against direct long division; binomial values against Pascal recurrence; 3,000 inversion arrays against pairs; 2,000 Fuzzy constructions against exact 128-bit inequalities and a 1001-digit exponent check; 3,000 geometry cases and large-coordinate boundaries. |
| digit_dp_test.cpp | 1,000 pattern/modulus/leading-zero configurations against exhaustive completion enumeration, including smallest reconstruction and explicit one-digit-zero behavior. |
| kactl_test.cpp | 600 random strings against naive KMP/Z answers; 350 SCC graphs against reachability; 350 2-SAT cases against all assignments; 350 max-flow cases against all s-t cuts; 350 matching cases against exhaustive matching. Boundary checks for hull/area/intersection, Aho duplicate patterns, binary lifting, 150 line insertions with naive maxima over 51 query points, and matrix Fibonacci powers 0..39. |

The selected printed KACTL headers compile in the adapter. This does not claim that all existing KACTL algorithms were revalidated. Original upstream status comments remain upstream evidence.

`audit_test.cpp` adds full-width signed arithmetic, 2,000 sliding-window comparisons near `LLONG_MAX`, 1,000 interval-union comparisons, decimal endpoints, CRT overflow/modulus-one, Fenwick rank and digit-zero checks. The audit fixed pre-shrink window overflow and a DSU member-name collision with KACTL’s `sz` macro.

The integration program, `native_modules_test.cpp`, compiles the 29 core/math generated module headers together and checks their dependency integration, including the `sz` macro. All 35 generated headers also compiled individually with KACTL macros and aliases under Clang C++17. Four helper oracle suites were additionally run against generated modules with sanitizers before the interval addition. The exact printed greedy example passed 1,000 exhaustive subset comparisons with sanitizers.

`case_studies_test.cpp` adds 500 donation instances against exhaustive subset compatibility, 500 capacity instances against every cut partition, 1,000 signed-sum arrays against endpoint enumeration, 150 random toggle systems plus all 512 targets of the 3x3 board against state BFS, and 300 grids plus explicit boundaries against time-layer hazard/player propagation. It also replays returned escape paths. The same source runs against both the authoritative header and the generated printed modules, so seven programs produce eight executions per test mode. Empty inputs, impossible paths, strict ties, duplicate donations, and capacities near `LLONG_MAX` are covered.

## PDF checks

The final editions use native KACTL A4 landscape. Math and implementation references use three columns; the worked-patterns book uses two columns and 10pt prose for derivations. The PDFs have 6, 10, and 17 pages respectively. Numbered contents and in-book page references connect recognition cues to worked cases. Code uses native source imports and hashes. All pages were rendered and inspected for clipping, overlaps, tables, and headers. Builds finish without LaTeX errors, undefined references, or overfull-box warnings. Page boxes were verified as landscape.

The source and build scripts are editable. Original KACTL headers are printed through the unchanged native preprocessor. Generated helper modules carry authorship, source, description and validation status metadata.

## Limits

These are local oracle checks, not new online-judge acceptances. Full maximum-constraint Displaying Decimals aggregation is excluded. Recursive specialist snippets can need stack planning. Arithmetic and memory bounds remain part of each caller's contract. No full contest rehearsal or event-specific printed-material compliance has been claimed. CI configuration is provided; local tests alone do not establish that a remote workflow has completed.

## Print and evidence revision

Helper source lines fit 63 columns using two-space tab stops. Formatting is committed; builds do not require clang-format. Reference notes retain preconditions, costs, and selection cues. The separate worked book supplies derivations, counterexamples, traces, adaptation boundaries, and practice answers without a page-count cap. The regional and Mexico/Masters round-three reports distinguish primary-source examples from the older provisional regional classifications. Mexico/Masters coverage is a spot audit, not a complete historical corpus.

Case review corrected numerical traces, clarified that negative weights break a particular greedy solver rather than the monotonicity of capacity feasibility, and distinguished team capacity edges from job-demand edges. Primary-source facts and independent derivations are separated in the three pattern-source ledgers.

## October 28 expansion

The three new suites run against both authoritative sources and generated printed modules. Together with the case-studies rerun, ten test programs produce fourteen executions per test mode. Optimized Clang C++17 and AddressSanitizer/UndefinedBehaviorSanitizer runs pass. All 14 new generated headers also compile independently with KACTL macros and aliases; the previous 35-header validation remains recorded above.

- Math: all divisors for 1..300 against direct enumeration; minimum kth-power multipliers for n=1..80 and k=1..4 against enumerated perfect powers, with an overflow case; factorial valuations against factoring each factorial term; 500 GCD histograms and equality checks against pair enumeration; circle cycles for n=1..80 and steps -100..100 against explicit walks.
- DP/trees: 400 complete cost matrices against all permutations; 600 small trees against all matching edge subsets and explicit ancestor walks for subtree ranges; every valid binary string through length 12 against rank reconstruction, plus an unsigned-rank saturation boundary.
- Practical: 1,000 linked-array cases against sorted key/index records; 500 inventories with 20 successive requests each against full per-order accumulation; oversized 64-bit request rollback; 300 arrays with 30 threshold queries each against direct scans.

The three PDFs were rebuilt, all pages rendered and visually checked, and selected new code pages inspected at larger size. No overfull boxes or undefined references were reported. The incorrect Flip board illustration was fixed to rows **., *.., ... . Passing code tests alone did not establish the correctness of the old illustration.

Preconditions remain explicit: positive bounded GCD inputs, distinct prime factor bases, valid recipe IDs and nonnegative quantities, valid query intervals, symmetric adjacency for trees, and bounded complete cost matrices. The helpers do not claim to validate every caller precondition. The binary unranking example is synthetic and does not implement the entire 2024 Inversion Insight solution.

## Prime and divisor query follow-up

`number_queries_test.cpp` passed optimized and sanitizer builds against both authoritative and generated printed modules. It checks primality through 10,000 by trial division; 1,000 random inclusive count/list queries; 1,208 small segmented intervals; narrow intervals near 10^6, 10^9, and 10^12; all divisor counts/sums through 1,000 by direct enumeration; and invalid-range rejection. All three complete example programs compiled and produced their expected sample output. This adds one test program and two executions per mode to the full runner, now eleven programs and sixteen executions. The earlier unchanged suites retain the checks recorded above.

Only the math PDF was rebuilt for this addition, now six pages. All its pages were rendered and checked, with the new code page inspected at full size; no overfull boxes or undefined references were reported. Bounds in the documentation are enforced by the helper constructors/functions; no unrestricted 64-bit prime-counting claim is made.
