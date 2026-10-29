# Implementation handbook for the Mexico final

Use this book to turn an identified technique into correct C++17 code. The math
companion explains arithmetic, counting, and proofs. This book explains state,
containers, transitions, and the details that usually cost a submission.

The regional research motivates the selection. Graph modeling, range information,
DP, and implementation discipline recur across the analyzed sets. A graph label
can mean a difficult reduction, so a BFS template alone does not solve every
problem in that category. Start by identifying what a vertex and an edge mean.

The code appendix is the authoritative `include/core.hpp`. Copy only the modules
you need, together with their type aliases and standard includes. Every module
lives in `namespace finals_core`. The original KACTL templates remain unchanged.

## One-minute technique lookup

| Need | First choice | Check before coding |
|---|---|---|
| Which vertices are connected? | Components or DSU | Undirected graph? Offline unions only? |
| Fewest moves, every move costs one | BFS | Is one vertex enough to describe a state? |
| Nearest of many starting positions | Multi-source BFS | Put every source at distance zero |
| Movement costs zero or one | 0-1 BFS | Every edge weight must be 0 or 1 |
| Minimum nonnegative total cost | Dijkstra | Negative edges invalidate it |
| Precedence, dependencies, DAG DP | Kahn topological order | Reject cycles before using DAG transitions |
| Cheapest way to connect an undirected graph | Kruskal + DSU | Report disconnected input explicitly |
| Repeated ancestor/path-length queries | Binary lifting LCA | Input must be a tree |
| Immutable interval sums | Prefix sums | Use 64-bit totals |
| Offline interval additions | Difference array | Materialize after all additions |
| Point changes, interval sums/counts | Fenwick tree | Updates are additions, intervals are half-open |
| Point assignment, interval min/max/custom merge | Segment tree | Associative merge and correct identity |
| Contiguous window with monotone feasibility | Two pointers | Negative values can break monotonicity |
| Minimum feasible answer | Binary search on the answer | Prove predicate monotonicity |
| Each item can be used once | 0-1 knapsack | Descend capacities |
| Each item can be reused | Unbounded knapsack | Ascend capacities; positive weights |
| Longest strictly increasing subsequence | LIS + lower_bound | Equal values must not extend strict LIS |

## Before copying code

All vertex and array indices are zero-based. All array ranges are `[l,r)`, so
`r` is excluded. Convert a one-based inclusive input `[L,R]` into `[L-1,R)`.
An empty range has `l == r`. Prefix arrays have length `n+1`.

`ll` means `long long`. `INF = LLONG_MAX / 4` is an unreachable sentinel for
weighted shortest paths. Every true shortest distance must be less than `INF`.
For other sums, including MST weight, prefix sums, DP values, and products, prove
that intermediate values fit. The sentinel does not automatically protect other
arithmetic. When necessary, multiply with `__int128` before reducing or comparing.

Graph endpoints, source indices, and grid dimensions must be valid. Assertions
catch some mistakes during practice; they are not input validation. Most graph
functions assume `n` fits `int`, and the grid version also needs `height*width`
to fit `int`. An empty graph is allowed only where no source/root is required.

A minimal caller is:

```cpp
#include <iostream>
#include "include/core.hpp"
using namespace finals_core;
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    Graph g(n);
    while (m--) {
        int u, v; std::cin >> u >> v; --u; --v;
        g[u].push_back(v);
        g[v].push_back(u); // Delete this line for directed input.
    }
    auto result = bfs(g, {0});
    std::cout << result.dist[n-1] << '\n';
}
```

Do not mix `scanf` and `cin` after disabling stream synchronization. Read the
whole statement before deciding whether test cases have a leading count, end at
EOF, or stop at a sentinel. Reset graph and DP storage between test cases.

## Components and ordinary BFS

### Connected components

`components(g)` traverses an undirected graph without recursion. It returns:

- `count`, the number of components;
- `id[u]`, a component label between zero and `count-1`;
- `representative[c]`, one vertex from component `c`;
- `size[c]`, the number of vertices in component `c`.

Time and extra storage are `O(n+m)` and `O(n)`. Each undirected edge normally
appears twice in the adjacency lists. Isolated vertices count as components.
A loop or parallel edge does not change the component partition.

To connect every component using the minimum number of new roads:

```cpp
auto c = components(g);
std::cout << std::max(0, c.count-1) << '\n';
for (int i = 1; i < c.count; ++i)
    std::cout << c.representative[0]+1 << ' '
              << c.representative[i]+1 << '\n';
```

There must be at least `c.count-1` new edges because one new edge can reduce the
component count by at most one. The construction reaches that bound. If `g` is
directed, this function is neither an SCC algorithm nor a weak-components
algorithm. Build an undirected view for weak components; use KACTL `graph/SCC.h`
for strongly connected components.

### BFS and paths

`bfs(g,{s})` returns `Search{dist,parent}`. `dist[u]` is the fewest edges from
`s` to `u`, or `-1` when unreachable. A parent is assigned when the vertex first
enters the queue. Each source has parent `-1` and distance zero.

The queue processes all distance `d` vertices before distance `d+1` vertices.
That is why the first discovery gives a shortest path. Mark on insertion, not
on removal, or many neighbors may insert the same vertex.

`restore_path(result,t)` returns the vertices along one shortest path, including
its source and `t`. It returns an empty vector if `t` is unreachable. The path
length in edges is `path.size()-1`; a problem may ask for the number of vertices
instead. BFS costs `O(n+m)`, and path restoration costs `O(path length)`.

```cpp
auto s = bfs(g, {start});
auto path = restore_path(s, finish);
if (path.empty()) std::cout << "IMPOSSIBLE\n";
else {
    std::cout << path.size() << '\n';
    for (int u : path) std::cout << u+1 << ' ';
    std::cout << '\n';
}
```

The chosen path depends on adjacency order. It is not necessarily
lexicographically smallest. BFS also works on directed graphs, following only
the stored directions.

### Many sources

`bfs(g,{s1,s2,...})` computes distance to the closest source in one traversal.
Duplicate sources are ignored. An empty source list leaves everything
unreachable. Its cost is `O(n+m+number of supplied sources)`.

Do not run BFS separately from every source when the question is distance to
any source. Insert them all at distance zero. If the question asks for the
nearest red source and nearest blue source separately, run two multi-source
searches with the two different source sets.

For spreading hazards, compute hazard arrival times first. A player may enter
`v` at time `t` only if the problem allows it, often `t < hazard[v]`.
Simultaneous arrival is unsafe in many statements. This needs a constrained
player BFS; calling the unconstrained grid helper does not enforce the rule.

## Grids and implicit state graphs

`grid_bfs(grid,{{row,column},...},'#')` performs four-neighbor BFS. Rows must
have equal lengths. Every character other than `'#'` is traversable, including
`A`, `B`, and dots. A wall source is ignored. Sources must be valid cells.

Cell `(r,c)` has vertex ID `r*width+c`. Recover coordinates using `id/width`
and `id%width`. The returned `Search` has `height*width` entries. Traversal costs
`O(height*width)` time and memory without constructing adjacency lists.

```cpp
auto s = grid_bfs(board, {{sr,sc}});
int target = tr*width+tc;
auto path = restore_path(s,target);
std::string moves;
for (int i = 1; i < int(path.size()); ++i) {
    int dr = path[i]/width-path[i-1]/width;
    int dc = path[i]%width-path[i-1]%width;
    moves += dr == 1 ? 'D' : dr == -1 ? 'U' : dc == 1 ? 'R' : 'L';
}
```

An empty `path` means no route. If start equals target, it contains one cell and
`moves` is empty, which is a valid zero-move path. For eight-neighbor movement,
change the displacement arrays and recheck corner-cutting rules.

The most common modeling error is storing too little state. If legality depends
on key possession, parity, remaining wall breaks, or orientation, `(r,c)` alone
is insufficient. Use `(r,c,keyMask)`, `(r,c,time%period)`, or the required state.
Allocate distances per full state. Reaching the same cell with a different key
set can lead to a different future.

Count the states before writing code. A grid with `h*w` cells and `2^k` key sets
has up to `h*w*2^k` vertices. An implicit graph saves edge storage, not state
count. Keep the original board separate from visited state when revisits with
different state values are meaningful.

## Weighted shortest paths

Both weighted helpers use `WGraph`, an adjacency list of pairs `{neighbor,weight}`.
They return `WeightedSearch{dist,parent}`. Unreachable distance is `INF`, and a
source parent is `-1`. To restore a path, first check `dist[t] != INF`, follow
`parent` back to `-1`, and reverse. Do not pass a `WeightedSearch` directly to
`restore_path`, which accepts the unweighted `Search` type.

### 0-1 BFS

`zero_one_bfs(g,s)` finds minimum total weight when every edge has weight zero
or one. A zero-cost relaxation goes to the deque front; a one-cost relaxation
goes to the back. This preserves distance order. Time is `O(n+m)`, memory is
`O(n+m)` including queued entries.

```cpp
WGraph g(n);
g[u].push_back({v,0}); // Free transition.
g[v].push_back({u,1}); // Paid reverse transition, if the model says so.
auto s = zero_one_bfs(g,start);
```

Unlike ordinary BFS, a discovered vertex can later receive a better distance.
Use relaxation `newDistance < dist[v]`, not one permanent visited flag. A common
application assigns cost one to reversing an edge and zero to following it.
The statement must justify those transitions.

### Dijkstra

`dijkstra(g,s)` accepts nonnegative `ll` edge weights. Time is
`O((n+m) log(n+m+1))` with this lazy heap implementation; memory is `O(n+m)`.
For a simple graph the usual bound is written `O((n+m) log n)`. A stale heap
entry is discarded when its saved distance differs from `dist[u]`.

```cpp
auto s = dijkstra(weighted,start);
if (s.dist[finish] == INF) std::cout << "IMPOSSIBLE\n";
else std::cout << s.dist[finish] << '\n';
```

Negative edges invalidate Dijkstra, even if there is no negative cycle. Use a
suitable negative-edge algorithm, such as KACTL `graph/BellmanFord.h`, after
checking its contract. For all-pairs distances with small `n`, inspect
`graph/FloydWarshall.h`; its cubic time often suits `n` of a few hundred.

The implementation skips candidate distances at least `INF` before addition.
This avoids overflowing that addition, but such paths cannot be represented.
Choose a larger safe sentinel or wider arithmetic if constraints require it.
Zero-weight cycles are fine. Counting shortest paths in their presence needs
extra reasoning; adding a `ways` update to ordinary Dijkstra may be wrong.

## Dependencies, topological order, and DAG DP

`topological_order(g)` uses Kahn's algorithm on a directed graph. It returns a
permutation in which each edge `u -> v` places `u` before `v`. If a cycle exists,
it returns an empty vector. Check `order.size() == g.size()`, which also handles
the valid empty graph. Time and storage are `O(n+m)` and `O(n)`.

Use indegree zero as the initial queue. Reducing indegrees simulates removing
completed tasks. A vertex can enter the queue only after all prerequisites have
been removed. Parallel edges count separately and are removed separately.

A longest path from `source` in an unweighted DAG:

```cpp
auto order = topological_order(g);
if (order.size() != g.size()) { /* Report a cycle; stop DAG DP. */ }
else {
    std::vector<ll> best(g.size(),-INF);
    std::vector<int> parent(g.size(),-1);
    best[source] = 0;
    for (int u : order) if (best[u] != -INF)
        for (int v : g[u]) if (best[u]+1 > best[v]) {
            best[v] = best[u]+1; parent[v] = u;
        }
    // best[target] == -INF means unreachable, not a zero-length path.
}
```

Replace `max` by modular addition for path counts, or by `min` for shortest paths
on a DAG. DAG shortest paths can have negative weights because the topological
order prevents revisiting a dependency. Parallel edges count as distinct paths
unless you explicitly deduplicate them.

A recurrence is not automatically a DAG algorithm. First identify the dependency
order and prove that every transition uses an already computed state.

## Connectivity changes and minimum spanning trees

`DSU(n)` stores `n` initially separate sets. `find(u)` returns a representative,
`unite(u,v)` returns true exactly when it merged two sets, and `size(u)` returns
the size of the current set containing `u`. Path compression and union by size
give amortized `O(alpha(n))` time per operation and `O(n)` memory.

DSU answers connectivity under additions. It does not give a graph path, a
shortest distance, or support arbitrary edge deletions. The representative is an
implementation detail and may change after a union.

`kruskal(n,edges)` sorts undirected `Edge{u,v,w}` values and accepts an edge
whenever it joins different DSU sets. It returns `MST{weight,components,edges}`.
For connected nonempty input, `components == 1` and exactly `n-1` edges are
selected. For disconnected input it returns a minimum spanning forest.

```cpp
std::vector<Edge> edges = {{0,1,4},{1,2,2},{0,2,9}};
auto mst = kruskal(3,edges); // weight 6, two selected edges.
if (mst.components != 1) { /* No spanning tree exists. */ }
```

Kruskal costs `O(m log m)` time plus near-linear DSU work. Negative edge weights,
parallel edges, and self-loops are supported; a self-loop never joins two sets.
Store each undirected edge once in the edge list. A minimum spanning tree
minimizes the sum of selected edge weights, not all source-to-vertex distances.
Dijkstra's shortest-path tree solves a different objective.

For offline deletions, sometimes process operations in reverse and turn deletions
into additions. For rollback, see KACTL `data-structures/UnionFindRollback.h`.
Check whether path compression is compatible with rollback before modifying it.

## Trees without recursive DFS

`root_tree(tree,root)` assumes a nonempty connected undirected tree. It returns
`parent`, `depth`, `order`, and `subtree`. The root is its own parent. `depth`
counts edges from the root. `order` places parents before children; reversing it
places children before parents. `subtree[u]` includes `u` itself.

Time and storage are `O(n)`. No recursive call can overflow the runtime stack.
The function visits vertices only once, but on a graph with cycles it would
produce a spanning-tree order rather than prove that the input is a tree. The
caller must enforce the tree assumption.

A common tree DP asks for a maximum independent set, a largest set with no
adjacent selected vertices:

```cpp
auto info = root_tree(tree,0);
std::vector<ll> take(n,1), skip(n,0);
for (int i = n-1; i > 0; --i) {
    int u = info.order[i], p = info.parent[u];
    take[p] += skip[u];
    skip[p] += std::max(take[u],skip[u]);
}
ll answer = std::max(take[0],skip[0]);
```

The two states mean whether `u` itself is selected. If it is selected, children
must be skipped. If it is skipped, each child chooses its better state. This
works because different child subtrees have no cross edges.

This `order` is not an Euler tour and does not make each subtree a contiguous
interval. For subtree range queries, explicitly compute DFS entry/exit times.
For path range queries and updates, inspect KACTL `graph/HLD.h` after learning
heavy-light decomposition; an ordinary Fenwick tree over the current `order`
does not solve that task.

### Binary lifting LCA

Construct `LCA ancestor(info)`. `jump(u,k)` returns the `k`th ancestor or `-1`
when `k` exceeds `depth[u]`. `query(u,v)` returns the lowest common ancestor.
`distance(u,v)` returns the number of edges on the unique tree path.
Preprocessing costs `O(n log n)` time and storage; each query costs `O(log n)`.

```cpp
auto info = root_tree(tree,root);
LCA ancestor(info);
int common = ancestor.query(u,v);
int edgesOnPath = ancestor.distance(u,v);
```

The table entry `up[k][u]` is the ancestor `2^k` steps above `u`. First lift the
deeper vertex to equal depth. Then lift both from the largest powers down while
their ancestors differ. Their immediate parent is the LCA.

For a weighted tree, compute `rootDistance[u]` separately and use
`rootDistance[u] + rootDistance[v] - 2*rootDistance[common]`. The provided
`distance` method counts edges and does not know edge weights. For forests,
add component IDs and reject cross-component queries before using a suitable
forest-aware implementation.

## Ordered information and interval techniques

### Coordinate compression

`compressed_values(a)` returns the sorted distinct values. `rank_of(values,x)`
returns the lower-bound position of `x`. It is an exact compressed index only
when `x` belongs to `values`; otherwise it is an insertion point, possibly
`values.size()`. Build values from every coordinate that later needs an index.
Sorting costs `O(n log n)`, and each lookup costs `O(log n)`.

Compression preserves order and equality, not numerical distances. Values
`10` and `1000000` can become adjacent ranks. Recover real gaps from the original
coordinate vector when computing lengths or areas.

```cpp
std::vector<ll> a = {40,10,40,90};
auto values = compressed_values(a); // 10,40,90.
Fenwick counts(int(values.size()));
for (ll x : a) counts.add(rank_of(values,x),1);
```

To count inversions, scan left to right. The number of earlier values greater
than `a[i]` is `seen - counts.prefix(rank+1)`. Then add one at `rank`. Querying
before updating avoids counting the current element.

### Prefix sums

`prefix_sums(a)` returns `p[0]=0` and `p[i+1]=p[i]+a[i]`. Thus the sum on `[l,r)`
is `p[r]-p[l]`, including empty ranges. Preprocessing costs `O(n)`, a query costs
`O(1)`, and storage is `O(n)`. Negative entries are supported.

Do not rebuild prefix sums after every update. If updates are online, use a
Fenwick or segment tree. For immutable rectangles, use a two-dimensional prefix
sum and inclusion-exclusion; KACTL `data-structures/SubMatrix.h` is a reference.

### Difference arrays

Allocate `std::vector<ll> diff(n+1)`. `difference_add(diff,l,r,x)` adds `x` to
all positions in `[l,r)`. After all updates,
`materialize_difference(diff)` returns the resulting length-`n` array. To update
an existing array, add the materialized increments to it, or initialize `diff`
with the differences of that array.

```cpp
std::vector<ll> diff(6); // Five actual positions plus one sentinel.
difference_add(diff,1,4,7);
difference_add(diff,3,5,-2);
auto a = materialize_difference(diff); // 0,7,7,5,-2.
```

Each update costs `O(1)`; materialization costs `O(n)`. The sentinel at `diff[n]`
is necessary for updates ending at `n`. A difference array supports offline
updates, followed by queries. Interleaved updates and queries require another
data structure.

### Two pointers and sliding windows

`count_subarrays_at_most(a,limit)` counts nonempty contiguous subarrays whose
sum is at most `limit`, assuming all entries are nonnegative. It costs `O(n)`
time and `O(1)` extra space. Zeros are valid. A negative limit returns zero.

At each right endpoint, remove leftmost entries until the window is feasible.
All starts between `l` and `r` are then feasible, so add `r-l+1`. Each pointer
moves forward at most `n` times.

For exact sum `k` with nonnegative entries, use
`atMost(k)-atMost(k-1)`. Negative entries destroy this window argument. For
arbitrary signed entries and exact sum, count earlier prefix sums equal to
`currentPrefix-k` using a map. Initialize the count of prefix zero to one.
If you need deterministic bounds, choose `std::map` or compressed coordinates
instead of assuming constant-time hashing.

### Binary search on a monotone predicate

`first_true(lo,hi,ok)` searches the nonnegative integer interval `[lo,hi)`.
The predicate must be false and then true as the argument increases. It returns
the first true position, or the original `hi` if none exists. It never evaluates
`ok(hi)`. Its cost is `O(log(hi-lo+1) * predicate cost)`.

```cpp
ll answer = first_true(0,1001,[&](ll x) { return x*x >= 200; });
// answer == 15. Multiplication is safe for this small bound.
```

Choose an excluded upper endpoint that includes every candidate you care about.
When `hi` is a proven feasible sentinel but intentionally excluded, returning
`hi` still gives that answer. Record which interpretation you use.

Before coding, write a sentence such as "if capacity x works, every larger
capacity works." If that sentence is false, answer binary search is invalid.
A greedy feasibility test still needs its own proof. This helper's midpoint is
safe for nonnegative bounds; do not pass negative search intervals to it.

## Fenwick tree

`Fenwick(n)` starts with `n` zeros. `add(i,delta)` adds to index `i`.
`prefix(r)` sums `[0,r)`, and `sum(l,r)` sums `[l,r)`. Updates and queries cost
`O(log n)`; storage is `O(n)`. Constructing and adding `n` initial entries costs
`O(n log n)` with this version.

```cpp
Fenwick fw(n);
for (int i = 0; i < n; ++i) fw.add(i,a[i]);
ll interval = fw.sum(l,r);
// Assignment a[i] = value requires a delta:
fw.add(i,value-a[i]); a[i] = value;
```

Negative values are allowed for sums. They are not allowed for order statistics.
`lower_bound(k)` requires `k > 0` and every represented entry nonnegative.
It returns the zero-based index of the first position where the inclusive
prefix reaches `k`, or `n` if the total is less than `k`.

For frequency counts `[0,2,0,3]`, the first and second elements in sorted order
have index 1; the third through fifth have index 3. Therefore
`lower_bound(1)==1`, `lower_bound(3)==3`, and `lower_bound(6)==4`.
This can remove the `k`th remaining item with `add(index,-1)`, provided its
frequency stays nonnegative.

The internal array is one-based, while the public API is zero-based. Copy the
whole module; mixing conventions is a common off-by-one failure. See KACTL
`data-structures/FenwickTree.h` and `FenwickTree2d.h` for related templates.

## Segment tree

`SegTree<T,Op>` supports point assignment and associative range aggregation.
The constructor takes the initial array and an identity value. `set(i,value)`
assigns one position; `query(l,r)` merges `[l,r)`. Build time and storage are
`O(n)`; each update and query costs `O(log n)`.

```cpp
SegTree<ll,Sum> sums(a,0LL);
SegTree<ll,Minimum> mins(a,std::numeric_limits<ll>::max());
sums.set(2,8);
ll total = sums.query(1,4);
ll smallest = mins.query(1,4); // Independent tree; still has original a[2].
```

The merge must be associative: `op(op(a,b),c) == op(a,op(b,c))`. It need not be
commutative because the query preserves order. The identity must satisfy
`op(identity,x) == op(x,identity) == x`. For sums use zero, minimum use the
largest representable value, maximum use the smallest representable value,
and GCD use zero. Empty queries return the identity.

For maximum subarray sum, each node needs total sum, best prefix, best suffix,
and best subarray. Define exactly whether an empty subarray is allowed, then
choose the node identity and merge rules consistently. A tree of scalar sums
alone cannot answer that query.

This implementation does not support interval updates. Repeating point updates
for every element can time out. For range addition/range maximum, or range
assignment/range sums, use lazy propagation and explicitly define how a pending
operation composes with another. KACTL `data-structures/LazySegmentTree.h` has a
specific contract; read it before adapting. Its update semantics are not implied
by this point-assignment API.

## From a slow solution to DP

A DP state records enough information that the future no longer depends on the
full history. Write these six lines before selecting an array size:

1. What does one state mean, in one exact sentence?
2. Which decisions enter or leave this state?
3. What are the base states and impossible-state values?
4. In what order are all dependencies already known?
5. What is the number of states times transitions per state?
6. Which state contains the answer, and do you need a witness?

For example, brute force tries every subset of items. Many histories reach the
same pair `(items considered,capacity remaining)`. Keep the best value for that
pair; histories with lower value and the same future options can be discarded.
That gives `O(n*capacity)` DP rather than `2^n` enumeration. If capacity is huge,
this is not an efficient solution merely because it is DP. Consider DP by value,
meet in the middle, a smaller mathematical state, or another technique.

For exact reachability or minimum cost, initialize impossible states explicitly.
Examples are `false`, `INF`, or `-INF` depending on the operation. Initializing
every exact-sum state to zero falsely makes unreachable sums reachable. The
provided knapsack functions use an "at most capacity" objective, so zero is
valid everywhere because taking no items is allowed.

### 0-1 knapsack

`knapsack01(weight,value,capacity)` returns the maximum value using each item
at most once and total weight at most capacity. The vectors must have equal
length, weights are nonnegative integers, and capacity is nonnegative. Time is
`O(number of items * capacity)` and space is `O(capacity)`.

```cpp
ll best = knapsack01({2,3,4},{4,5,7},5); // 9, using weights 2 and 3.
```

Descending capacity ensures `dp[c-weight]` still describes the previous item
layer. Ascending would allow the current item to be reused. Zero-weight items
are supported here because every capacity is updated once per item.
Negative values are allowed; choosing no item gives a result of at least zero.

If you need the selected items, a parent pointer stored in the overwritten
one-dimensional array is not automatically safe. Later updates can change a
parent state and create an invalid witness. Store a full two-dimensional decision
table, immutable predecessor records, or use a proven reconstruction method.
Budget the memory before choosing the first option.

### Unbounded knapsack and counting loop order

`knapsack_unbounded` has the same objective but permits unlimited copies of
every item. Weights must be strictly positive. Capacity goes upward so that the
current item can extend an already updated state. A positive-value zero-weight
item would make the optimum unbounded and is rejected by this API.

```cpp
ll best = knapsack_unbounded({2,3},{4,5},6); // 12, three copies of weight 2.
```

Counting is different from maximizing. To count unordered coin combinations,
put the coin loop outside and ascend sums. To count ordered sequences of coin
choices, put the target sum outside and sum over each possible last coin.
For coins `{1,2}` and total 3, combinations are `{1,1,1}` and `{1,2}`, while
ordered sequences are `(1,1,1)`, `(1,2)`, `(2,1)`. Duplicate denominations need
attention; they may be distinct item types or accidental repeated values.

### Longest increasing subsequence

`lis_indices(a)` returns indices of one longest strictly increasing subsequence,
in their original order. It costs `O(n log n)` time and `O(n)` space.
Values may be repeated or negative. Empty input returns an empty vector.

```cpp
std::vector<int> a = {3,1,2,2,4};
auto ids = lis_indices(a); // Length 3, with values 1,2,4.
for (int i : ids) std::cout << a[i] << ' ';
```

`tails[k]` is the smallest possible last value for a strictly increasing
subsequence of length `k+1`. It is not itself necessarily a subsequence from
the input. The separate predecessor indices reconstruct an actual witness.
To obtain a longest nondecreasing subsequence, change `lower_bound` to
`upper_bound` and keep the predecessor logic consistent. See KACTL
`various/LIS.h` for its own indexing and reconstruction contract.

## Worked DP: a huge number becomes a small remainder

Suppose a digit pattern contains fixed digits and '?', and you need the smallest completion divisible by m. Trying every completion costs up to 10^q, where q is the number of unknown digits. Two prefixes at the same position with the same remainder have identical future options. Merge them.

Define `can[pos][r]`: after reading pos digits with remainder r, some permitted suffix can finish with remainder zero. Base: `can[length][0]=true`, every other terminal remainder is false. Transition: try allowed digit d and ask `can[pos+1][(10*r+d)%m]`. Fill positions backwards. Leading-zero restrictions apply at position zero, including fixed digits.

To reconstruct, start at pos=0,r=0. Try digits in increasing order and take the first whose next state is feasible. This is lexicographically smallest because a larger first differing digit can never improve the number, and feasibility guarantees that the chosen prefix can finish. Fixed-length canonical numbers have the same lexicographic and numeric order.

Example `?2`, modulus7: candidates 12,22,32,42,... first succeed at42. Looking only for whether any completion exists would not by itself produce this reconstruction.

The helper `finals_dp::complete_digits("?2",7)` returns an optional string containing `"42"`. `nullopt` means impossible. It uses O(length*m*10) time and O(length*m) bytes plus vector overhead. For length1000,m1000, there are roughly one million stored states and at most ten million tested transitions. Estimate this product before allocating. Counts-only DP can roll two rows, but reconstruction needs feasibility information or a different recovery method. The helper is for decimal pattern completion, not all digit-DP tasks: upper-bound tightness, digit sums, or forbidden substrings require extra state.

### Counting loop order changes the objects

For positive coin values with unlimited reuse, initialize `ways[0]=1` and other entries zero. To count unordered multisets, process each coin type outside and sums ascending inside. To count ordered sequences, process sums ascending outside and last coin inside. With coins1,2 and target3, unordered answers are 1+1+1 and1+2, hence2. Ordered answers also include2+1, hence3. Duplicate denominations count as distinct types unless deduplicated. Reduce additions modulo the requested modulus or prove the count fits.

### Small-subset assignment DP

When n is around20 and the state is the chosen subset, `dp[mask]` can store the minimum cost of assigning the first popcount(mask) workers to the jobs in mask. For every unchosen job j, relax `dp[mask | (1<<j)]` with `dp[mask]+cost[popcount(mask)][j]`. Set dp[0]=0 and others infinity; skip unreachable masks. Time O(n*2^n), memory O(2^n). This state is sufficient because only which jobs remain affects later workers. It is not a general substitute for polynomial matching on larger instances. Check shift widths and memory before computing `1<<n`.

### Copyable remainder DP

This separate header has no dependency on core.hpp.

{{code:include/digit_dp.hpp}}

## Submission checks that catch real failures

Use one tiny normal case, one smallest case, and one case that attacks the
assumption behind the technique. For BFS, include an isolated target and a
cycle. For Dijkstra, include a cheaper indirect route and a zero-weight edge.
For interval code, query `[0,n)`, `[0,0)`, and `[n,n)`. For knapsack, use one
item that fits twice to detect an accidental unbounded update.

Check whether the answer counts edges or vertices, whether the source itself
is an allowed answer, and whether equal values are strict or non-strict.
Disconnected graphs, repeated coordinates, zero values, and multiple test cases
usually expose more bugs than a large random example viewed by eye.

Compile in the judge's language mode. During practice add sanitizers and
warnings when available. The accompanying `tests/core_test.cpp` compares the
modules against small brute-force oracles; it does not prove that a new problem
has been modeled correctly. Write down the reduction and its assumptions before
trusting a familiar template.

## Extension map into the original KACTL

These are references for tasks beyond this companion. Check the original header's
description, dependencies, and indexing before use. Advanced templates require
practice before the contest.

| Need | Original repository file | Added responsibility |
|---|---|---|
| Strongly connected directed components | `content/graph/SCC.h` | Reduce the condensation graph to a DAG |
| Boolean implications | `content/graph/2sat.h` | Encode both implications correctly |
| Euler trail/circuit | `content/graph/EulerWalk.h` | Verify degree and connectivity conditions |
| Bridges and biconnected structure | `content/graph/BiconnectedComponents.h` | Distinguish vertices from edges in the result |
| Bipartite matching | `content/graph/HopcroftKarp.h` | Identify the two partitions and allowed pairs |
| Capacity flow | `content/graph/Dinic.h` | Construct capacities, source, sink, and prove reduction |
| Paths with costs and capacities | `content/graph/MinCostMaxFlow.h` | Read negative-cost and potential assumptions |
| Online subtree/path operations | `content/graph/HLD.h` | Choose correct node/edge intervals |
| Static range minimum | `content/data-structures/RMQ.h` | Updates are not supported |
| Range updates | `content/data-structures/LazySegmentTree.h` | Match lazy composition to operation semantics |
| Dynamic line minimum/maximum | `content/data-structures/LineContainer.h` | Verify slope and arithmetic conventions |
| DP optimization | `content/various/DivideAndConquerDP.h`, `KnuthDP.h` | Prove the required monotonicity first |

Matching, flow, string algorithms, and computational geometry have additional
material in the other companions or original KACTL. Do not interpret this core
as complete coverage of every regional problem.

## Authoritative core C++17 code

The following listing is the tested header used by the examples and randomized
checks. The `BOOK` markers divide it into lookup sections in the printed build.

{{code:include/core.hpp}}
