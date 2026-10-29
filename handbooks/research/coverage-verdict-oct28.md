# Are the finals handbooks enough?

Reviewed October 28, 2026. Eleven calendar days remain before the user's November 8 event.

## Verdict

The books are useful references with tested building blocks. They are not a complete collection of reductions for the historical corpus. More patterns are warranted, particularly where the books show a routine but not how to derive its inputs or DP state. This audit recommends additions; it does not claim those additions already appear in the PDFs.

The local atlas contains 116 rows across 2017–2025. Its confidence field contains 64 `high`, 25 `official-editorial`, and 27 rows across medium, low, unresolved, and several independently derived statuses. These labels are inherited research metadata, not an independent acceptance audit. Mexico/Masters coverage remains a spot sample. A percentage of problems the handbook will solve would therefore be unsupported.

## Factorization: what is already present, what is missing

If “tri factorization” means trial factorization, it already appears in `FactorSieve.h`, together with a smallest-prime-factor sieve. The Fuzzy Factorization worked case also uses it after reducing the huge input to a ten-digit prefix. If the intended meaning is factoring into three factors, that needs the exact constraints: distinctness, positivity, ordering, and whether factors must be prime change the problem.

The useful addition is a decision card and worked exponent examples:

| Input or question | Method | Why / limit |
|---|---|---|
| One moderate positive integer | Trial division, remove each full prime power | Worst-case O(sqrt(n)); use `p <= n/p` to avoid overflow. Estimate total work across all queries. |
| Many integers bounded by a feasible V | SPF sieve then repeated division | O(V) preprocessing in the current implementation; O(log x) divisions per query; memory proportional to V. |
| Many pairwise divisibility/GCD questions, small V | Frequency array and sums over multiples | Replaces quadratic pairs with value-domain work. Requires positive values or explicit zero treatment. |
| Divisibility of products, perfect powers, minimum multiplier | Prime exponent vectors | Compare/subtract exponents instead of constructing overflowing products. |
| Arbitrary large exact integers | An appropriately bounded primality/factorization implementation | Current trial division is insufficient. Original KACTL has Miller–Rabin/Pollard rho, but this checkout's primality header advertises only up to 7e18. Do not label it unrestricted uint64 support without adapting and validating its modular arithmetic. |
| Huge decimal integer with allowed relative error | Prove a bounded-prefix approximation first | Fuzzy Factorization is this case; exact huge-integer factorization is unnecessary. |

A useful synthetic trace: 72 = 2^3 * 3^2. It has (3+1)(2+1)=12 positive divisors. The smallest positive multiplier making it a square is 2, because the exponent of 2 is odd. In general the minimum multiplier making n a kth power supplies exponent `(k-e%k)%k` for each prime. Products and output size still need bounds. For factorial divisibility, compute v_p(n!) by repeated division by p and sum the quotients; do not construct n!.

A second missing worked reduction is exact GCD pair counting. For positive values <=V, compute M[d], the number divisible by d. Descending d, calculate E[d] = M[d]*(M[d]-1)/2 - sum(E[2d], E[3d], ...). E[d] counts unordered distinct-index pairs with exact GCD d. This is O(V log V), and E[1] answers coprime pairs. It is an independent derivation using the partition of pairs by exact GCD. The current per-fixed-b gcd formula is related but not this complete recipe.

Practice statements checked directly: [Counting Divisors](https://cses.fi/problemset/task/1713), [Counting Coprime Pairs](https://cses.fi/problemset/task/2417), and [Divisor Analysis](https://cses.fi/problemset/task/2182). They establish task shapes and constraints, not historical LATAM frequency. Divisor Analysis includes modular divisor products and is a later exercise, not a necessary first lesson.

## Additions ranked for this team

These priorities are coaching judgment based on the reported math, DP, and graph gaps. They are not measured solve probabilities.

| Priority | Addition | Present coverage / missing step | Evidence or practice |
|---|---|---|---|
| First | Factorization selection, exponent reasoning, divisor generation | Factor/SPF code and divisor formulas exist; add worked transformations and bounded-batch choice | CSES arithmetic tasks above; existing Fuzzy case |
| First | GCD orbits on a circle | GCD and totient exist; no worked orbit partition | 2017 J Jumping Frog in regional ledger |
| First | Tree DP from child summaries | Tree traversal and LCA exist; no worked child-state recurrence | 2017 G Gates of Uncertainty in ledger; CSES Tree Matching |
| First | Subset DP for ordered choices | Toggle enumeration exists; it does not retain a last item or reuse equivalent partial histories | 2017 A Arranging Tiles, 2023 K Keen on Order in ledger |
| Next | Offline threshold queries + Fenwick | Sorting, compression and Fenwick exist separately; explain when to activate data before a query | 2018 L Looking for the Risk Factor in ledger |
| Next | Exact GCD pair counts over multiples | Existing fixed-b gcd classes need adaptation; use the complete descending recurrence above | CSES Counting Coprime Pairs |
| Next | Base digits become equations/divisors | Remainder representation exists; base-dependent algebra is missing | 2023 F Forward and Backward, verified by math audit |
| Later | Lexicographic unranking | Smallest valid reconstruction exists, but kth-answer block skipping is different | 2024 I Inversion Insight; counts must be exact/saturated, not modular |

Tree DP practice: [Tree Matching](https://cses.fi/problemset/task/1130) supplies a simple maximum-edge matching objective on a tree. A two-state child-combination derivation is more useful here than starting with general matching code. This is a recommended learning route, not a claim that tree DP is the only solution.

Subset DP recognition: n is small enough for 2^n states and different histories with the same used set and last item have identical future choices. Define dp[mask][last] before coding, prove that those fields are sufficient, and budget O(n*2^n) memory and O(n^2*2^n) transitions. A mask alone is insufficient when the next cost depends on the last item. Do not infer this state from the word “subset” alone.

GCD orbit trace: on 12 positions, repeatedly adding 8 visits 0,8,4,0. There are gcd(12,8)=4 disjoint cycles, each of length 3. The reachable positions from s are exactly those congruent to s modulo 4. A safety condition must still be checked within each cycle.

## A correctness fix before the next print

Direct source inspection found an inconsistent synthetic board in `content/finals-patterns/model-cases.tex`. The printed trace currently contains rows `*..`, `**.`, `*..`, while the following explanation says the target has only bits 0,1,3 and is produced by pressing the top-left cell. The intended three rows are `**.`, `*..`, `...`. The helper and its tests are separate from this incorrect illustrative trace. Correct the trace and regenerate the PDF before printing; this audit has not changed the PDFs. This also shows why passing code tests is not enough to certify all explanatory examples.

## Print more versus learn more

There is no page cap, so an advanced appendix can be useful for a teammate who already understands the method. That does not make every advanced method worth learning in eleven days. Keep tested core algorithms easy to find. Add a worked model with each new family, including a wrong approach and a small counterexample.

Do not spend the remaining preparation time primarily on Pollard rho, FFT/NTT, HLD, or broad geometry expansion unless a teammate can already recognize and adapt that tool. Practice selecting and modifying the core patterns in unfamiliar statements. If a basic BFS/DP solution still requires following a worked example line by line, another advanced template will not remove that bottleneck.

The current Displaying Decimals case remains deliberately partial: it explains a single fraction, not the full maximum-constraint all-pairs algorithm. That is a concrete coverage limit, not something to conceal by adding another factorization routine.

## Evidence trail

- [Math audit](math-gap-audit-oct28.md): direct print inspection and primary-source checks for 2023, 2024, 2025 examples.
- [Implementation audit](implementation-gap-audit-oct28.md): separate review of reductions and implementation coverage.
- [Regional evidence](regional-evidence-round3.md) and [Mexico/Masters evidence](mexico-masters-round3.md): scope and provenance limits.
- The CSV corpus and year-specific reports remain under the workspace outputs directory outside this clone.

This turn changed research notes only. Existing PDF contents, code validation, and online-judge acceptance status have not been expanded by this audit.

## Implementation follow-up

Later on October 28, the requested math, tree/subset DP, subtree, unranking, linked-sort, inventory, and threshold-query additions were integrated and locally tested. The Flip trace was corrected and all PDFs rebuilt. Earlier statements above describe the audit state before these changes; see handbooks/VALIDATION.md for current checks.
