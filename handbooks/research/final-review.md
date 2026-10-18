# Independent final review: arithmetic and core implementation companions

Reviewed 2026-10-18 by the specialist-audit agent, independently of the two handbook authors. Read `handbooks/math.md`, `handbooks/include/math.hpp`, `handbooks/implementations.md`, and `handbooks/include/core.hpp`, plus the relevant source APIs previously audited. This review does not edit those authors' files. All literal `content/...h` and `content/...tex` references in the two companions were checked for existence.

## Outcome

No blocking arithmetic formula, helper/API mismatch, or DP semantic error found in the reviewed snapshot. The code and prose agree on important limitations. This is a source review, not a claim of exhaustive validation; the supplied differential tests and root integration build remain complementary evidence.

## High-stakes checks

- **Signed division:** floor/ceil use 128-bit quotient/remainder corrections for either denominator sign, reject zero, and checked-narrow the LLONG_MIN/-1 case. No negation of LLONG_MIN occurs in 64 bits.
- **Fixed decimals:** parsing happens from text, refuses excess fractional places, pads exact trailing zeros, and checks the magnitude before narrowing. The negative signed-64 endpoint is representable; a positive value of the same magnitude is rejected. It deliberately does not round, parse exponent notation, or accept a missing integer part.
- **Modular arithmetic:** residues normalized to a positive modulus; multiplication widens before multiplying. Inverse checks gcd, prime factorial combinations explicitly require prime p and N<p, and modular expectation denominators are required invertible. No unsupported division modulo a composite is prescribed.
- **General CRT:** compatibility uses gcd(m,n) dividing the residue difference; coefficient and residue products widen before arithmetic. Result modulus is lcm, not m*n unconditionally. Modulus1 works: n/g may be1 and remainder modulo1 yields zero. Overflow beyond signed64 lcm is reported rather than silently wrapping. Worked example14 modulo18 meets both input congruences.
- **Decimal periods:** denominator is reduced first; max(v2(q),v5(q)) is the finite prefix, and the remaining coprime denominator uses multiplicative order, not Euler totient directly. a=0 reduces to denominator1; integer-part zero contributes one digit. Single-fraction runtime is not misrepresented as a full solution to the all-pairs task. Full aggregation remains explicitly unbenchmarked.
- **Fuzzy construction:** ten leading digits guarantee m>=1e9 for long canonical inputs. Discarded tail r<10^e gives r/X<1/m<=1e-9. Powers of ten add both prime exponents and merge existing2/5 exponents. Short inputs are factored exactly. The map is explicitly for the valid constructed Y, not X.
- **Combinatorial counts:** contribution multiplicities are correct; repeated-block inversions distinguish within-copy order from all cross-copy pairs and use C(K,2). Counting combinations versus ordered sequences uses the appropriate loop interpretation. Linearity of expectation is not incorrectly restricted to independent events.
- **Geometry:** widening precedes coordinate subtraction/product. Magnitude1e18 is safely below signed128 cross and squared-distance bounds. The prose separately warns that polygon accumulation needs its own bound and that a product of128-bit orientation values may overflow. Predicate exactness is not confused with fractional intersection construction.
- **BFS / shortest paths:** first-discovery parent semantics, multisource initialization, grid indexing and rectangular input, unreachable sentinels, and weighted return types match the implementation. Dijkstra enforces nonnegative weights and distances below INF; no claim that the unconstrained grid helper handles hazards or inventory states.
- **Trees / connectivity:** components are explicitly undirected. Kruskal returns a forest on disconnected input. root_tree order is explicitly not an Euler subtree interval. LCA parent(root)=root, jump beyond depth=-1, weighted-tree distance requires a separate root-distance array.
- **Ranges:** half-open bounds consistent. Difference array has n+1 sentinel; compression preserves order/equality only. Fenwick assignment uses a delta, order statistics require nonnegative entries. Segment tree preserves order even for a noncommutative associative merge.
- **DP:** knapsack is maximum value at weight at most capacity; zero initialization is therefore valid. Descending capacities in01 preserve prior item layer, including zero-weight items once each. Unbounded requires positive weights. LIS uses lower_bound for strictness and reconstructs indices, not the tails array. Mutable one-dimensional parent-pointer reconstruction is correctly warned against.

## Nonblocking improvements for this team's needs

1. Add a small worked remainder-DP pseudocode example with both existence and reconstruction, or a short distinct DP example beyond knapsack/LIS. The prose explains sufficient states well, but the user's difficulty moving from naive search to DP merits one concrete full transition walkthrough. This is a coverage recommendation, not a discovered defect.
2. If adding "exact sum k = atMost(k)-atMost(k-1)" as copyable code, guard k=LLONG_MIN before subtracting1. Current prose already assumes nonnegative input entries and the global requirement to prove intermediate bounds, so there is no existing executed overflow from this identity.
3. Keep the paper/page-rule note as a final printing prerequisite; actual event limits have not been supplied. Broad source coverage should not become an implied prediction of the contest set or a claim that every mapped specialist is printed in full.

## Integration observations

Source expansions may remove original descriptive comments for print density only if the API/preconditions stay in nearby handbook prose. Preserve authors and license information as planned. The specialist appendix distinguishes source presence from default chapter import, especially Dinic, and documents source placeholders such as LinearRecurrence's mod5. Compiling source snippets with the actual GNU prelude is necessary; it does not validate a new contest reduction automatically.
