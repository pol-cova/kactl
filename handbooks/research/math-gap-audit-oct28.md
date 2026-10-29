# Math coverage audit for the finals handbook

Audit date: 2026-10-28. Scope: the printed math chapter at
`content/finals-math/chapter.tex`, its imported headers, and the regional
problem research ledgers already in this repository. This is an editorial
coverage audit, not a prediction of the next contest or a claim that a handbook
can solve every problem.

## Assessment

The chapter is useful as a compact math reference: it covers exact integer and
decimal arithmetic, modular powers/inverses, shared-factor CRT, trial
factorization and SPF, totients, a bounded-value gcd histogram, remainder DP,
rational decimal periods, the 2025 approximation construction, standard
counting and expectation, inversions, proof checks, and integer geometry. The
source comments state the important domains and limits. In particular,
`FactorSieve.h` already supplies both trial division and an SPF sieve, and the
chapter says when trial division stops being appropriate.

So if “tri factorization” means **trial factorization**, the fundamental
pattern is already there and worth learning. It is practical for a single
moderate integer, for the bounded core in Fuzzy Factorization, and through SPF
for many values under a feasible maximum. Do not turn that into “trial divide
any 64-bit input”: the chapter already warns that repeated unrestricted
64-bit inputs need a faster method. Pollard rho/Miller–Rabin exist in the
original KACTL number-theory folder, but belong in an advanced appendix or
specialist reference, not the first-response checklist.

The larger opportunity is adding a few recognition-and-proof patterns that
are visible in the corpus but not yet explained in the printed math chapter.
The existing print chapter is already dense; these belong in a short companion
page or targeted insert, rather than adding a broad template catalog.

## High-value additions

| Priority | Missing or thin pattern | Recognition and technique | Caveat | Evidence |
|---|---|---|---|---|
| 1 | **GCD partitions of a cyclic process** | A fixed step on a cycle of size `N` visits one residue class modulo `gcd(step,N)`. Group steps by that gcd; count the valid step sizes with totient weights when needed. | The gcd identifies the orbit partition, not whether that orbit satisfies the problem's safety condition. Handle step zero and the problem's range of steps explicitly. | 2017 J, *Jumping Frog*, is described in the [regional problem ledger](../../../outputs/analysis-2017-2019.md#2017-j--jumping-frog). This extends the chapter's gcd/factorization content with an actionable reduction. |
| 2 | **Ranked construction by completion counts** | For lexicographic unranking, at each position count completions under each next choice; subtract whole blocks until the target rank falls in one. This changes an enormous enumeration into counting plus selection. | Rank counts must be exact or safely saturated at the original `K`; modular counts cannot be compared with a rank. Prove any DP width/cutoff instead of trusting an experimental bound. 2024's official sketch's stated cutoff was disproved for `n=101, K=4e18`; see the [independent correction](../../../outputs/review-math-and-dp.md#2024-i-the-official-small-inversion-claim-is-too-strong). | 2024 I, *Inversion Insight*, statement and [official solution sketches](https://codeforces.com/gym/105505/attachments/download/28286/icpcla24_solution_sketches.pdf). The existing audit explicitly notes this is missing from print coverage. |
| 3 | **Base representation becomes divisor enumeration** | When a number has exactly two base-`b` digits and both digits are equal to `d`, write `N=d(b+1)`. Enumerate divisors `d`, recover `b=N/d-1`, then check digit and base constraints. Check small bases directly when their representation has more digits. | Verify `b>=2`, `1<=d<b`, digit length, duplicates between the direct and two-digit cases, and integer-square-root boundaries. This is an algebraic representation reduction, not a need for factorization of `N` first. | 2023 F, *Forward and Backward*, in the [official 2023 solution PDF](https://icpcarchive.github.io/Latin%20American%20Contests/Latin%20American%20Regional%20Contest/2023%20Latin%20American%20Regional%20Contest/solution.pdf). |
| 4 | **Binary carries from exponent frequencies** | When the input gives powers of two by exponent, count each exponent and propagate pairs into the next bit. For 2020 D, the sum's binary population count determines whether it can be split into the required powers; the nonempty-box condition handles the one-set-bit case. | Define whether zero is allowed and what happens to a final carry. “Binary” alone does not imply a carry DP; some tasks reduce to a direct invariant. This example is a discrete carry argument, not a general-purpose binary DP. | 2020 D, *Dividing Candy*, is derived from its [official statement](https://icpcarchive.github.io/Latin%20American%20Contests/Latin%20American%20Regional%20Contest/2020%20Latin%20American%20Regional%20Contest/problems.pdf) and corroborated by a [solver-authored explanation](https://github.com/Diego1149/ICPC-Latam-2020/tree/main/D); the latter is not official jury material. See also the [2020–2022 ledger](../../../outputs/analysis-2020-2022.md). |

## Useful, but lower priority

- **Largest-prime-factor threshold queries:** if queries ask for values `x<=N`
  whose largest prime factor is at most `K`, sieve the largest prime factor,
  sort values and queries by `K`, and use a Fenwick tree for the prefix count.
  This complements SPF but is a specialized offline pattern (2018 L,
  *Looking for the Risk Factor*; [ledger](../../../outputs/analysis-2017-2019.md#2018-l--looking-for-the-risk-factor)).
- **Prime-factorization choice card:** the current code contract is already
  good, but a one-line decision guide would help: trial division for one small
  or bounded core; SPF for many inputs in `[1,V]`; Miller–Rabin plus Pollard
  rho only for genuinely large arbitrary 64-bit inputs with suitable
  factorization limits. The original [KACTL factorization header](../../content/number-theory/Factor.h)
  documents Pollard rho, but its randomized behavior and dependency chain need
  a separate understood-and-tested contract.
- **Bit carries / base digits:** this does not warrant general-purpose
  arbitrary-precision coverage. The winning move is often to process one
  digit/bit at a time and retain only the carry or remainder that affects the
  suffix; `chapter.tex` already teaches the decimal remainder version.

## Already covered well enough for a compact print reference

- **Trial factorization and totient:** the helper performs division through
  `p<=n/p`, removes full prime powers, and leaves a final prime remainder;
  the prose states `O(sqrt(n))`, SPF limits, and the arbitrary-64-bit caveat.
  For one moderate positive input, this is enough to use correctly. A short
  trace could improve teaching, but adding more trial-division code would have
  low return.
- **Modular arithmetic and CRT:** inverse existence, prime-only Fermat use,
  non-coprime compatibility, LCM output modulus, and overflow are explicit.
- **Fractions and periods:** reduction, 2/5 prefix, multiplicative order, and
  the distinction between one-ratio helper and unbenchmarked all-pairs
  aggregation are carefully bounded.
- **Fuzzy Factorization:** the leading-digit approximation proof identifies
  exactly why the huge input does not require huge-integer factorization. The
  official [2025 Problem F statement](https://scorelatam.naquadah.com.br/latam-2025/contest.pdf)
  permits relative error `10^-9`; the handbook's ten-digit construction
  supplies the reduction and conditions.
- **Counting, contributions, expectation, and proof checks:** these are
  concise conceptual formulas and pitfalls. They are a good index, though
  distinct from a library of complete contest reductions.

## Recommendation

Keep trial factorization in the core and explicitly teach its three-way
decision rule (one moderate number, bounded batch, arbitrary 64-bit). Add
worked pattern cards for gcd orbits and completion-count unranking first;
add base-representation divisors next. A binary-carry example is useful if
space allows. Treat each card as recognition, derivation, tiny trace, bound,
and caveat, not just a topic label or copied snippet.

These additions would close concrete gaps found in a nine-season research
corpus, but they do not promise coverage of all problem types or a solution to
every finals problem. The research itself marks specialist methods and some
full-limit implementations as unverified; preserve those qualifications.

## Source and evidence limits

Corpus observations here come from the repository's nine-season ledgers and
existing audit notes; their taxonomy is curated and overlapping, not official
problem tags. Primary examples link to organizer statements or official
solution material. Recommendations and priorities are editorial judgment.
No handbook code was edited in this audit, and no additional implementation
or performance claim was tested.
