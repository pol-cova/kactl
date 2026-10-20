# Model and pattern cases: sources

## Intervals: Jigsaw of Shadows

- Official problem statement, ICPC Latin American Regionals 2024, Problem J: <https://codeforces.com/gym/105505/attachments/download/28244/contest_onesided.pdf>
- Official solution sketches, Problem J: <https://codeforces.com/gym/105505/attachments/download/28286/icpcla24_solution_sketches.pdf>
- Verified statement facts: road on the x-axis, light angle relative to ground, eastward shadows, distinct positions, union length output; input bounds include N <= 1e5, angle 10 to 80 degrees, X <= 3e5, H <= 1000, and absolute/relative error 1e-4. The numeric interval trace in the chapter is explicitly synthetic, not claimed as an official sample.

## Flip

- Official ICPC Masters Mexico 2022 problem book, Problem F, page 9: <https://acm.iteso.mx/share/ICPCMXMasters2022_problems.pdf>
- Verified statement facts: a 3x3 board, click toggles itself and existing orthogonal neighbors, target is reachable, up to 50 cases, and the input alphabet is `*` / `.`. The chapter's board trace is synthetic: top-left press produces bits 0, 1, and 3 under row-major numbering; it is not attributed to the official sample.

## Synthetic cases and implementation APIs

- The matching/team-capacity example and contiguous partition example are teaching constructions, not contest statements.
- Code contract details were checked against `handbooks/include/case_studies.hpp` in this repository: `finals_cases::min_toggle_count` enumerates at most 20 binary effects; `finals_cases::min_contiguous_capacity` accepts nonnegative `long long` durations, a positive group limit, and an empty input (returns 0). These local helpers are the implementations behind the case descriptions.
