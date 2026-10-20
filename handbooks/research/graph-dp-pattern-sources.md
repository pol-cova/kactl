# Graph and DP casebook source notes

These notes identify the primary statements checked for the worked cases and
record the contract checks that affect the explanations. The booklet text is
paraphrased; the links are for verification, not copied solutions.

## CSES Monsters

- Primary statement: <https://cses.fi/problemset/task/1194/>
- It specifies simultaneous one-cell moves for the player and monsters, an
  escape to any boundary square without sharing a square, one `A`, and the
  `.`, `#`, `A`, `M` alphabet. Constraints are height and width at most 1000.
- The helper `finals_cases::escape_monsters` in
  `handbooks/include/case_studies.hpp` uses that alphabet, assumes one `A` and
  a rectangular grid, and returns an optional UDLR path. The casebook's
  separate arrival-time explanation follows from the statement's simultaneous
  movement rule.

## 2017 Enigma

- Primary statement: <https://maratona.ime.usp.br/hist/2017/resultados/contest.pdf>
  (Problem E, pages 10--11 in the PDF).
- It asks for the smallest same-length multiple of `N` matching a digit/
  question-mark pattern, disallows leading zero, and specifies pattern length
  at most 1000, `1 <= N <= 1000`, and at least one `?`. No-solution output is
  `*`.
- The official judge presentation is linked from the contest materials at
  <https://maratona.ime.usp.br/hist/2017/resultados/presentation_brazil_nopause.pdf>.
  Its availability could not be confirmed by the web reader, so statements
  about the algorithmic method are grounded in the primary statement plus
  the directly inspectable local helper, not an attributed presentation quote.
- `finals_dp::complete_digits` is declared in
  `handbooks/include/digit_dp.hpp`; its contract includes a configurable
  leading-zero rule and optional result. The implementation uses a backward
  boolean remainder table and forward smallest-digit reconstruction.

## 2017 Fundraising

- Primary statement: <https://maratona.ime.usp.br/hist/2017/resultados/contest.pdf>
  (Problem F, pages 12--13 in the PDF).
- The statement says guests coexist when both coordinates are equal or one
  guest is strictly richer and strictly prettier than the other. It asks for
  the maximum total donations; `N <= 100000` and beauty, fortune, and donation
  are each in `[1, 10^9]`. Thus the original donation weights are positive.
- The official judge presentation is at
  <https://maratona.ime.usp.br/hist/2017/resultados/presentation_brazil_nopause.pdf>;
  its content was not available to the web reader during this check. The
  casebook's tie rules are also reflected in the local implementation.
- `finals_cases::max_donation_chain` in
  `handbooks/include/case_studies.hpp` supports nonnegative donation values,
  groups equal coordinate pairs, and batches equal-wealth updates. It uses
  zero-based compressed beauty positions and a half-open segment-tree query.
  The booklet's negative-weight discussion is a modification analysis; it is
  outside both the statement's positive-weight contract and this helper's
  stated guarantees.

## Local API references checked

- `handbooks/include/core.hpp`: `bfs` and `restore_path`, `grid_bfs`,
  `rank_of`, `Fenwick::prefix`, and `SegTree::query` establish the conventions
  used in the implementation chapter. Vertex/rank indices are zero-based;
  Fenwick updates translate those indices internally to one-based storage,
  and `prefix(r)` means the half-open range `[0,r)`.
- `handbooks/include/case_studies.hpp`: the case-specific helper contracts
  and the implementation details summarized above.
- `handbooks/include/digit_dp.hpp`: remainder-DP helper API and bounds.
