# Math pattern case sources and evidence

Reviewed 2026-10-19. This note records the primary materials used for the print walkthroughs in `content/finals-patterns/math-cases.tex`. The derivations are explanatory work for the handbook; a primary statement confirms the task and its constraints, not that an implementation was accepted.

## 2025 F — Fuzzy Factorization (`case:fuzzy`)

Primary source: [official 2025 Latin American Regional contest statement, Problem F](https://scorelatam.naquadah.com.br/latam-2025/contest.pdf), printed pages 9–10 (PDF pages 9–10). It specifies an integer up to `10^1000`, relative error at most `10^-9`, and prime factors at most `10^18`. The statement's sample also illustrates replacing `1073741825` by `2^30`.

The leading-prefix construction is independently derived: write `X=m*10^e+r` after keeping ten digits, so `0<=r<10^e` and, for inputs longer than ten digits, `m>=10^9`; therefore `r/X<1/m<=10^-9`. Factor `m` by trial division, then combine the added factors of `10^e`. The walkthrough's arithmetic example uses `1234567890=2*3^2*5*3607*3803`; this factorization is offered as a worked arithmetic trace, not as a contest sample. The repository helper's implementation and constraints are documented in `content/finals-math/FuzzyFactorization.h` and the nearby math chapter entry.

## 2025 D — Displaying Decimals (`case:decimals`, `case:orbits`)

Primary source: [official 2025 Latin American Regional contest statement, Problem D](https://scorelatam.naquadah.com.br/latam-2025/contest.pdf), printed pages 7–8 (PDF pages 7–8). It asks for the expected digit count over ordered pairs, permits `N<=10^5` and values through `10^5`, and defines display examples including an integer result and a recurring decimal. Its sample table provides per-ratio digit counts.

The per-ratio formula follows from long division: after reducing `a/b`, factors 2 and 5 determine the terminating prefix; the remaining denominator `r` coprime to 10 has period `ord_r(10)`. The statement examples establish that the count includes the integer-part zero for proper fractions: `3/14 = 0.2142857` takes 8 digits. Example `1/28` is independently traced by the remainders modulo 7. The helper `finals_math::decimal_shape` handles one ratio. The source statement does not validate the proposed all-pairs gcd aggregation or its maximum-size performance. The existing research analysis explicitly marks that aggregation unbenchmarked; the case retains that limit and does not present it as a complete accepted solution.

## Masters Mexico 2022 H — Hidden Number (`case:missing`)

Primary source: [ICPC Masters Mexico 2022 problem book](https://acm.iteso.mx/share/ICPCMXMasters2022_problems.pdf), Problem H, printed page 11 (PDF page 10). The statement sets positive list entries through `10^6` and `N<=10^6`, and asks for the smallest positive value with no subsequence sum. It includes two examples; the interval proof and worked trace in the case are independent derivations from the statement.

The invariant is that sorted-prefix subset sums cover every integer in `[0,R]`. The next positive value `x` extends that interval to `R+x` exactly when `x<=R+1`; otherwise `R+1` is impossible. The resulting total can reach `10^12`, so the print guidance uses a 64-bit accumulator. This example is a statement-derived solution, not an official editorial claim or judge-acceptance claim.

## Synthetic exact scaling and remainder orbit (`case:orbits`)

This is a deliberately synthetic exercise, not a named contest problem. Its arithmetic uses the documented repository helpers `finals_math::parse_fixed` and `finals_math::decimal_shape`: parse `0.750` and `2.100` at scale `10^3` as 750 and 2100; their ratio reduces to `5/14`, with one nonrepeating digit and a six-digit period. The orbit is checked directly by long division. The 2025 D statement is linked only for its display convention; it is not the source of this synthetic input.

## Source use

No long statement text is reproduced. URLs point to organizer/contest primary materials. Historical-frequency claims are outside the scope of these cases. For broader corpus findings, see the year-specific analysis files and `handbooks/research/provenance.md`.

## Audit notes (2026-10-19)

The math cases were checked against the repository helper contracts and the cited primary statements. The display formula counts the integer-part zero before the decimal point, not a zero after the point; for `1/28`, the count is one integer digit plus two prefix digits plus six recurring digits. The cases link to statement PDFs with LaTeX `\href` syntax. The unbenchmarked `O(V log^3 V)` full-aggregation estimate was removed; no accepted full-limit algorithm is claimed. The synthetic `5/14` case remains explicitly synthetic. The factorization example `1234567890 = 2 * 3^2 * 5 * 3607 * 3803` and the displayed remainder cycles were checked arithmetically.
