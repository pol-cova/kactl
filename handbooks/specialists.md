# Specialist lookup: choose the model before the template

This appendix is a source guide for the implementation handbook. Source snapshot: `7532cfe6c1eda7279d6e182b83d41e47e7760658` of [pol-cova/kactl](https://github.com/pol-cova/kactl). Paths below start at `content/`. **P** means imported by the default chapter; **S** means present in source but omitted from that chapter. This describes the source configuration, not a claim that any previously built PDF contains it. Use the main handbook's basic graphs, arithmetic, and DP first.

KACTL headers assume the contest prelude (`ll`, `vi`, `pii`, `rep`, `all`, `sz`, standard includes). They are snippets, not independent libraries. Copy only what is needed and its dependencies. Global names such as `dfs`, `Time`, `mod`, `P`, and `conv` can collide. Use separate namespaces or adapt names when combining snippets. Source comments saying stress-tested describe upstream validation; the present handbook does not claim independent exhaustive verification of every header.

## Fast lookup

| Need | Header and exact entry point | Cost and main condition |
|---|---|---|
| All exact pattern occurrences | `strings/KMP.h`: `match(text, pattern)` | O(n+m); nonempty pattern; separator absent from inputs |
| Prefix borders | `strings/KMP.h`: `pi(s)` | O(n) |
| Prefix match at every suffix | `strings/Zfunc.h`: `Z(s)` | O(n); `z[0]=0` |
| Many fixed patterns | `strings/AhoCorasick.h`: `AhoCorasick ac(patterns)` | O(26L) build; default alphabet A–Z |
| Substring fingerprints | `strings/Hashing.h`: `HashInterval h(s)` | O(n) build, O(1) query; collisions possible |
| Palindrome radii | `strings/Manacher.h`: `manacher(s)` | O(n); separate even/odd conventions |
| Smallest cyclic rotation | `strings/MinRotation.h`: `minRotation(s)` | O(n); returns starting index |
| Sorted suffixes / adjacent LCP | `strings/SuffixArray.h`: `SuffixArray sa(s)` | O(n log n); includes empty suffix |
| Strong components of directed graph | `graph/SCC.h`: `scc(g, callback)` | O(V+E); recursive |
| Boolean pairwise clauses | `graph/2sat.h`: `TwoSat ts(n)` | O(V+E); literal negation is `~x` |
| Undirected biconnected edge blocks | `graph/BiconnectedComponents.h`: `bicomps(callback)` | O(V+E); edge-ID adjacency |
| Bipartite maximum matching | `graph/HopcroftKarp.h`: `hopcroftKarp(g,r)` | O(E sqrt V); disjoint left/right indices |
| Bipartite minimum vertex cover | `graph/MinimumVertexCover.h`: `cover(g,n,m)` | matching-dependent; returns combined indices |
| Capacity flow / min cut | `graph/Dinic.h`: `Dinic f(n)` **S** | Header bound O(VE log U); residual capacities |
| Upward tree jumps / LCA | `graph/BinaryLifting.h`: `treeJump(parent)` | O(n log n) build, O(log n) query |
| Many static LCA queries | `graph/LCA.h`: `LCA lca(g)` | O(n log n) build, O(1) query; root 0 |
| Tree path update / maximum | `graph/HLD.h`: `HLD<false> h(g)` | O(log² n); connected tree, root 0 |
| Point predicates | `geometry/Point.h`: `Point<ll>` | O(1); bound intermediate products |
| Convex hull | `geometry/ConvexHull.h`: `convexHull(points)` | O(n log n); excludes intermediate collinear points |
| Segment intersection | `geometry/SegmentIntersection.h`: `segInter(a,b,c,d)` | O(1); integer output can truncate |
| General polygon containment | `geometry/InsidePolygon.h`: `inPolygon(poly,p,strict)` | O(n); simple polygon |
| Signed doubled polygon area | `geometry/PolygonArea.h`: `polygonArea2(poly)` | O(n); nonempty polygon |
| Convex polygon containment | `geometry/PointInsideHull.h`: `inHull(h,p,strict)` | O(log n); nonempty CCW hull, no collinear vertices |
| Farthest points | `geometry/HullDiameter.h`: `hullDiameter(h)` | O(n) after hull; nonempty hull |
| Closest points | `geometry/ClosestPair.h`: `closest(points)` | O(n log n); at least two points |
| Prefix count in a sweep | `data-structures/FenwickTree.h`: `FT f(n)` | O(log n); zero indexed, half-open prefix |
| Offline 2D count | `data-structures/FenwickTree2d.h`: `FT2 f(nx)` | O(log² n); register updates before init |
| Offline interval queries | `data-structures/MoQueries.h`: `mo(queries)` | O(n sqrt q) moves; callbacks must be written |
| Max over lines / DP optimization | `data-structures/LineContainer.h`: `LineContainer lc` | O(log n) amortized insert/query; integer maxima |
| Fixed-size matrix power | `data-structures/Matrix.h`: `Matrix<T,N>` | O(N³ log exponent); no automatic modulus |
| Polynomial convolution | `numerical/FastFourierTransform.h`: `conv(a,b)` | O(N log N); precision bound below |
| Modular convolution | `numerical/NumberTheoreticTransform.h`: `conv(a,b)` | O(N log N); fixed supported prime and length |
| Huge index linear recurrence | `numerical/LinearRecurrence.h`: `linearRec(S,tr,k)` | O(d² log k); configure modulus explicitly |

All rows are P unless marked S. For a header's transitive dependencies, follow its actual `#include` statements. The sections below explain the decisions that a lookup table cannot make.

## Strings: structure before hashing

**KMP and Z.** `auto positions = match(text, pattern);` returns zero-based starts, including overlaps. `pi(s)[i]` is the longest proper prefix ending at i. A candidate period is `p=n-pi(s).back()`; full repetition requires `n%p==0`. A border alone does not imply an integral number of repeats. `auto z=Z(s);` gives the common-prefix length of `s` and `s.substr(i)`, with z[0]=0. For matching via `pattern + separator + text`, choose a character absent from both alphabets. KMP's built-in `match` uses nul; ordinary token strings satisfy this, arbitrary byte buffers may not. Treat empty patterns separately. Both use O(n) extra memory.

**Trie / Aho–Corasick.** There is no separate general-purpose `Trie.h` in this snapshot. Aho–Corasick builds a trie, then fills missing transitions through failure links; afterwards its `next` array is an automaton, not a sparse trie whose absent edges remain -1. Default `first='A'`, `alpha=26`: lowercase input without adaptation indexes outside the array. Construct from a mutable vector of nonempty patterns:

```cpp
vector<string> patterns = {"AB", "BAB"};
AhoCorasick ac(patterns);
auto endings = ac.find("ABAB");
auto starts = ac.findAll(patterns, "ABAB");
```

`find` returns one pattern index per text position, the longest ending there or -1. `findAll` returns all matching pattern indices grouped by start position, including duplicates. Do not mistake `find` for all matches. Build O(alpha·total pattern length), scan O(text length); enumerating all matches additionally costs the output size, which can be quadratic. The source header gives a coarse O(NM) bound for `findAll`.

**Hashing.** `HashInterval h(s); auto fingerprint=h.hashInterval(l,r);` hashes [l,r) in O(1), preprocessing and memory O(n). Compare lengths as well as hashes. This header uses arithmetic modulo 2^64−1, not the simpler overflow hash modulo 2^64. Hash equality is evidence, never a deterministic proof of equality. Prefer KMP/Z for exact matching; use hashes when many arbitrary substring comparisons make their convenience worthwhile and collision risk is acceptable. Do not mix hash objects produced with different bases or alphabets. `Hashing-codeforces.h` is a separate source variant, not the chapter import.

**Other string APIs.** `auto p=manacher(s)` gives even radius p[0][i] and odd half-length p[1][i]; lengths are `2*p[0][i]` and `2*p[1][i]+1`. Test centers on "abba" and "aba" before using indices. `int i=minRotation(s)` selects the lexicographically smallest rotation; rotate using `rotate(s.begin(),s.begin()+i,s.end())`. `SuffixArray a(s)` contains n+1 suffixes, with `a.sa[0]=n`, the empty suffix. `a.lcp[i]` compares suffixes sa[i] and sa[i−1]. No nul bytes; ordinary nonnegative ASCII avoids signed-char alphabet issues. Arbitrary two-suffix LCP requires inverse ranks plus RMQ on the appropriate LCP interval, not direct indexing by text position.

## Directed constraints and undirected connectivity

**SCC.** With `vector<vi> g`, call `scc(g,[&](vi& block){ /* consume vertices */ });`. Afterwards `comp[v]` and `ncomps` are globals. Components are emitted in reverse topological order: every inter-component edge goes from a larger component ID to a smaller one. Build the condensation DAG only after all IDs exist; deduplicate edges if multiplicity affects later counting. O(V+E) time and memory. Recursive DFS can exhaust the process stack on a long chain; stack feasibility must be addressed for large constraints. The callback's vector is reused; copy it if storing blocks. `dfs`, `Time`, and other globals collide with other snippets if blindly pasted.

**2-SAT.** A variable is its zero-based integer x; its negation is `~x`, never `!x` or `-x`.

```cpp
TwoSat ts(3);
ts.either(0, ~1);       // x0 OR NOT x1
// x1 => x2 is NOT x1 OR x2:
ts.either(~1, 2);
ts.setValue(0);
if (ts.solve()) { int x2 = ts.values[2]; }
```

Clauses must reduce to ORs of at most two literals. A requirement that exactly one of k>2 arbitrary variables holds is not generally captured by `atMostOne` alone: that method supplies only the upper bound. `addVar()` introduces auxiliaries; values includes them. Solve O(number of variables + implication edges). It has its own SCC implementation; it does not include `SCC.h`. Reconstruct the actual modeling choice from `values`, and check every original constraint on a tiny example.

**Biconnected blocks and bridges.** Set global `ed` to n empty adjacency lists, insert each undirected edge twice with one shared unique edge ID, then call `bicomps(callback)`. The callback receives edge IDs belonging to a non-bridge biconnected block. A vertex can belong to multiple blocks; a bridge is detected in the explicit `/* e is a bridge */` branch and is not automatically returned. To collect bridges, edit that branch. Self-loops are not a supported block-reporting convention here; handle them separately if allowed. Parallel edges are distinguished correctly only if each physical edge has its own ID. Skipping the parent vertex instead of the parent edge would break this property. Articulation points need additional bookkeeping, including the root-child rule. Do not interpret these blocks as SCCs or as a vertex partition.

## Matching and capacity models

**Hopcroft–Karp.** `g` has one list per left vertex; entries are right-side indices [0,m). Initialize `vi rightMatch(m,-1); int size=hopcroftKarp(g,rightMatch);`. It returns cardinality and fills rightMatch[right]=left, or -1. The left match can be reconstructed by scanning this array. O(E sqrt V), O(V+E) memory. It solves unweighted bipartite matching, not general graphs, weights, or capacity>1 directly. Bipartiteness belongs to the model: separate object types do not automatically imply all constraints are pairwise compatibility edges. `cover(g,n,m)` from `MinimumVertexCover.h` returns left IDs [0,n) and right IDs shifted by n. Its size equals maximum matching cardinality; it includes and uses DFSMatching, not HopcroftKarp.

**Dinic.** Source-only by default: explicitly include or print it if selecting it for contest use.

```cpp
Dinic flow(n);
flow.addEdge(s, a, 3); // directed capacity 3, default reverse capacity 0
flow.addEdge(a, t, 2);
ll sent=flow.calc(s,t);
bool sourceSide=flow.leftOfMinCut(a); // after completed calc
```

Capacities must be nonnegative integers fitting `ll`; source and sink must differ. `calc` mutates residual capacities and returns additional flow sent by that call. `Edge.c` is remaining capacity, `oc` original capacity, `flow()` reports nonnegative net usage for that directed edge. Keep an adjacency index when you need to inspect a specific modeled edge. `leftOfMinCut(v)` reports final residual reachability after max flow; it is not valid before calc. The header advertises O(VE log U), and faster unit-capacity/bipartite bounds. Memory O(V+E). DFS is recursive.

Matching model: source→each left vertex capacity1; allowed left→right capacity1; right→sink capacity1. Vertex capacity: split v into v_in→v_out and route incident edges through the split. Selection with penalties: derive which cut direction charges each forbidden choice before coding. "Infinite" means greater than every feasible finite cut, with a proved safe total bound; LLONG_MAX edges can make total flow overflow. `rcap` supplies initial reverse capacity, not a second independent labeled object. Avoid self-loops in this header's addEdge unless adapting reverse-index bookkeeping. Parallel non-self edges are fine. Flow does not solve min-cost optimization; that needs the separate MinCostMaxFlow API and its cost assumptions.

## Trees: path conventions are part of the algorithm

**Binary lifting.** Supply parent[root]=root and valid parent indices, with depths computed separately. `auto up=treeJump(parent); int a=jmp(up,v,k); int w=lca(up,depth,u,v);`. O(n log n) build/storage, O(log n) query. These tables are for upward ancestors, not arbitrary shortest paths. Ancestor steps beyond represented bits are not checked; restrict k to the supported range or enlarge the table. The RMQ-based `LCA tree(g); tree.lca(u,v);` instead roots at0 and gives O(1) queries after O(n log n) preprocessing; requires its RMQ dependency and recursive traversal.

**HLD.** `HLD<false> h(g)` stores vertex values; `HLD<true>` stores each edge at its deeper endpoint. Input must be a nonempty connected undirected tree rooted at0. The class copies adjacency and deletes parents from its private copy. `modifyPath(u,v,x)` adds x along the path; `queryPath(u,v)` returns its maximum. `querySubtree(v)` uses the contiguous Euler interval [pos[v],pos[v]+siz[v]), excluding v in edge mode. O(log² n) path operations, O(log n) subtree query with its lazy tree. The source includes `LazySegmentTree.h`, whose initial values are its negative default, not zero: initialize each relevant position before arithmetic updates, for example `h.tree->set(h.pos[v],h.pos[v]+1,initial[v])`. Path and subtree ranges are half open. For edge mode, u=v is an empty path and should have a deliberately chosen answer. Combining path pieces in this order assumes a commutative aggregate; string concatenation or matrix products require direction-aware processing. Change both the underlying segment tree and the HLD aggregation when switching max to sum/min.

## Geometry: distinguish predicates from constructed coordinates

Use `Point<ll>` for exact integer dot/cross predicates only when bounds fit. For coordinates |x|,|y|≤B, differences reach2B and a difference cross product can reach8B². Casting the final result is too late: widen before multiplication if using `__int128`. `Point<double>` is appropriate for constructed fractional coordinates; unit vectors and rotation should not use integer points because division truncates. A single arbitrary epsilon does not fix every geometric comparison.

`a.cross(b,c)` is (b−a)×(c−a), positive for a counterclockwise turn; `u.dot(v)` distinguishes acute/obtuse directions. Use squared distances for ordering when safe. `onSegment(a,b,p)` includes endpoints. Test a segment touching at an endpoint, overlapping collinearly, degenerate to a point, and disjoint.

`auto h=convexHull(points)` returns a CCW hull, removing intermediate collinear boundary points. Empty or all-collinear input produces a degenerate hull; handle its size before later operations. `hullDiameter(h)` assumes nonempty, CCW, no duplicate/collinear vertices and finds the farthest pair in O(h). `closest(points)` requires at least two points. Their squared distance arithmetic also needs overflow bounds.

`auto inter=segInter(a,b,c,d)` returns zero points for no intersection, one for a point, two endpoints for overlapping segments. Its proper intersection formula uses triple-coordinate products; with `Point<ll>` a nonintegral intersection is truncated. For existence alone, prefer orientation/sign predicates without constructing coordinates. For coordinate output, use an appropriate floating/rational representation and the problem's tolerance. `inPolygon(poly,p,false)` includes boundary, default strict=true excludes it; requires a simple polygon and O(n) per query. `inHull(h,p,false)` needs a nonempty CCW convex hull with collinear interior-edge points removed, O(log h); the implementation dereferences index0 even for an empty hull. `polygonArea2(poly)` returns signed doubled area, positive CCW, negative clockwise; never call on an empty vector. Keep doubled area integral for half-unit answers.

## Sweeps and optimized transitions

**Sort before reaching for a tree.** For offline requests "count points with x≤X and y≤Y", sort by x and coordinate-compress y, update `FT` for active points, answer prefix counts. Handle equality by explicit event order: insert equal-x points before queries for ≤, afterwards for <. `FT.update(i,delta)` adds; `FT.query(r)` returns [0,r). Thus [l,r) is query(r)−query(l). O((n+q)log n). Compression preserves order/equality, not coordinate distances. A DP with strict predecessor coordinates must delay updates for an equal-coordinate batch to avoid using peers from the same layer.

`FT2 f(nx)` is for update points known offline: call fakeUpdate(x,y) for every possible location, then init() once, then update(x,y,delta). `query(X,Y)` sums x<X,y<Y; rectangle sums use four prefixes. Registration is mandatory even if updates occur later. x is already a valid dense index. Memory O(n log n) for n registered points; query/update O(log² n). `FT.lower_bound(target)` is an order-statistic search requiring nonnegative element frequencies / monotone prefix sums; it is not valid after arbitrary negative updates. Its literal initial search bit `1<<25` also limits the directly supported size.

**Mo.** Use for offline static intervals when add/remove can update the answer cheaply but no small associative segment summary exists. `MoQueries.h` deliberately contains `...`: implement add, del, calc first; it is not copy-and-run code. `mo(Q)` uses [l,r), starts empty at [0,0), and returns answers in original query order. Its O(n sqrt q) movement estimate assumes suitable block size and O(1) callbacks; source blk=350 is a tuning constant, not universal. A frequency-of-frequencies example is a good rehearsal; dynamic updates require a different algorithm.

**Line container.** For `dp[i]=max_j(k[j]*x[i]+b[j])+own[i]`, insert a line for each eligible j, then query x[i]. `LineContainer lc; lc.add(k,b); ll best=lc.query(x);`. O(log n) amortized add/query; source handles arbitrary insertion/query order, duplicate slopes, integer breakpoints via floor division. Empty query asserts. For minimum, negate both slope/intercept and negate the answer, with overflow checked. This is valid only after deriving a linear transition. Products k*x and differences of slopes/intercepts must fit ll; enlarging the answer type alone does not fix internal overflow. Time eligibility still matters: insert before or after query according to whether j=i is legal.

## Matrices and transforms: specialist fallback

`Matrix<T,N> A;` is zero-initialized. Fill A.d; `(A^k)*vector` applies k transitions to a column state. If state_next[i]=sum_j A[i][j]*state[j], rows are destinations and columns sources. Cost O(N³ log k), memory O(N²); use only small state dimensions. Ordinary `Matrix<ll,N>` does not reduce modulo anything and readily overflows. Use a verified modular numeric type or adapt multiply/add with safe intermediate products. Exponent0 returns identity. For small k, ordinary DP is simpler.

`linearRec(S,tr,k)` computes S[k] from initial S[0..d−1] and coefficients `S[i]=sum_j S[i-j-1]*tr[j]`, in O(d² log k). Source contains `const ll mod=5` marked `exclude-line`; that is a source/testing placeholder, not the intended contest modulus. Configure mod explicitly when copying, normalize residues, and check multiplication bounds. Berlekamp–Massey can infer a recurrence over a suitable field but does not prove an observed prefix follows it forever.

FFT `conv(vector<double>,vector<double>)` returns coefficients c[k]=sum_i a[i]b[k−i], O(N log N) at padded power-of-two length N. Manual fft requires positive power-of-two length. For rounded integer convolution the source's sufficient bound is `(sum a[i]^2+sum b[i]^2)*log2(N)<9e14`; do not promise exactness above it. It returns doubles: explicit rounding is needed for integer counts. NTT's `conv(vector<ll>,vector<ll>)` instead returns exact coefficients modulo998244353, requires normalized inputs [0,mod), and depends on ModPow.h. The implementation chooses a power of two strictly larger than the requested output size, so require that actual padded N≤2^23, not just output size≤2^23. FFT and NTT define overlapping global symbols: select one or namespace them. Modular convolution is not integer convolution unless you prove coefficients fit the modulus or reconstruct with sufficient independent primes.

## Printed source pack

These are selected original source expansions, not a single file to concatenate indiscriminately. Keep needed dependencies once; put SCC's globals/functions in their own namespace when combining with other DFS snippets. Geometry's repeated `Point.h` includes become one printed definition; `#pragma once` handles source includes. The selected pack needs the following GNU C++17 prelude (SCC/2-SAT use GNU's omitted-middle conditional `a ?: b`):

```cpp
#include <bits/stdc++.h>
using namespace std;
#define rep(i,a,b) for (int i=(a); i<(b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
using ll = long long;
using vi = vector<int>;
using pii = pair<int,int>;
```

For standalone submissions add main, input parsing, output, and problem-specific model construction. Read each header's alphabet, indexing, boundary, and type conditions above before copying.

### Exact one-pattern matching

{{code:../content/strings/KMP.h}}

### Prefix matches

{{code:../content/strings/Zfunc.h}}

### Many-pattern automaton

{{code:../content/strings/AhoCorasick.h}}

### Directed strong components

{{code:../content/graph/SCC.h}}

### Pairwise Boolean constraints

{{code:../content/graph/2sat.h}}

### Capacity flow and residual cut

{{code:../content/graph/Dinic.h}}

### Bipartite cardinality matching

{{code:../content/graph/HopcroftKarp.h}}

### Basic tree ancestors

{{code:../content/graph/BinaryLifting.h}}

### Exact point operations

{{code:../content/geometry/Point.h}}

### Segment membership dependency

{{code:../content/geometry/OnSegment.h}}

### Segment intersection construction

{{code:../content/geometry/SegmentIntersection.h}}

### Convex hull construction

{{code:../content/geometry/ConvexHull.h}}

### Signed doubled area

{{code:../content/geometry/PolygonArea.h}}

### Dynamic linear transition maximum

{{code:../content/data-structures/LineContainer.h}}

### Fixed-state matrix powers

{{code:../content/data-structures/Matrix.h}}
