#pragma once
#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <limits>
#include <numeric>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace finals_core {
using ll = long long;
using Graph = std::vector<std::vector<int>>;
using WGraph = std::vector<std::vector<std::pair<int, ll>>>;
constexpr ll INF = std::numeric_limits<ll>::max() / 4;

// BOOK: Unweighted BFS and path reconstruction
struct Search {
    std::vector<int> dist, parent;
};
inline Search bfs(const Graph& g, const std::vector<int>& sources) {
    int n = int(g.size());
    Search a{std::vector<int>(n, -1), std::vector<int>(n, -1)};
    std::queue<int> q;
    for (int s : sources) if (a.dist[s] == -1) {
        a.dist[s] = 0; q.push(s);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) if (a.dist[v] == -1) {
            a.dist[v] = a.dist[u] + 1;
            a.parent[v] = u; q.push(v);
        }
    }
    return a;
}
inline std::vector<int> restore_path(const Search& a, int target) {
    if (a.dist[target] == -1) return {};
    std::vector<int> p;
    for (int u = target; u != -1; u = a.parent[u]) p.push_back(u);
    std::reverse(p.begin(), p.end());
    return p;
}
// BOOK: Connected components
struct Components {
    int count = 0;
    std::vector<int> id, representative, size;
};
inline Components components(const Graph& g) {
    Components a;
    a.id.assign(g.size(), -1);
    for (int s = 0; s < int(g.size()); ++s) if (a.id[s] == -1) {
        int id = a.count++;
        a.representative.push_back(s); a.size.push_back(0);
        std::vector<int> st{s}; a.id[s] = id;
        while (!st.empty()) {
            int u = st.back(); st.pop_back(); ++a.size[id];
            for (int v : g[u]) if (a.id[v] == -1) {
                a.id[v] = id; st.push_back(v);
            }
        }
    }
    return a;
}
// BOOK: Grid BFS
// Four-neighbor grid; every character other than wall is traversable.
inline Search grid_bfs(const std::vector<std::string>& grid,
                const std::vector<std::pair<int, int>>& sources,
                char wall = '#') {
    int h = int(grid.size()), w = h ? int(grid[0].size()) : 0;
    Search a{std::vector<int>(h*w, -1), std::vector<int>(h*w, -1)};
    std::queue<int> q;
    for (auto [r,c] : sources) {
        assert(0 <= r && r < h && 0 <= c && c < w);
        if (grid[r][c] != wall && a.dist[r*w+c] == -1) {
            a.dist[r*w+c] = 0; q.push(r*w+c);
        }
    }
    const int dr[] = {1,-1,0,0}, dc[] = {0,0,1,-1};
    while (!q.empty()) {
        int u = q.front(); q.pop();
        int r = u/w, c = u%w;
        for (int k = 0; k < 4; ++k) {
            int nr = r+dr[k], nc = c+dc[k];
            if (nr < 0 || nr >= h || nc < 0 || nc >= w) continue;
            int v = nr*w+nc;
            if (grid[nr][nc] == wall || a.dist[v] != -1) continue;
            a.dist[v] = a.dist[u]+1; a.parent[v] = u; q.push(v);
        }
    }
    return a;
}
// BOOK: Weighted distances: 0-1 BFS and Dijkstra
struct WeightedSearch {
    std::vector<ll> dist;
    std::vector<int> parent;
};
inline WeightedSearch zero_one_bfs(const WGraph& g, int source) {
    int n = int(g.size());
    WeightedSearch a{std::vector<ll>(n, INF), std::vector<int>(n,-1)};
    std::deque<std::pair<ll,int>> q;
    a.dist[source] = 0; q.push_front({0,source});
    while (!q.empty()) {
        auto [du,u] = q.front(); q.pop_front();
        if (du != a.dist[u]) continue;
        for (auto [v,w] : g[u]) {
            assert(w == 0 || w == 1);
            if (du+w < a.dist[v]) {
                a.dist[v] = du+w; a.parent[v] = u;
                if (w == 0) q.push_front({du,v});
                else q.push_back({du+1,v});
            }
        }
    }
    return a;
}
inline WeightedSearch dijkstra(const WGraph& g, int source) {
    int n = int(g.size());
    WeightedSearch a{std::vector<ll>(n, INF), std::vector<int>(n,-1)};
    using Node = std::pair<ll,int>;
    std::priority_queue<Node,std::vector<Node>,std::greater<Node>> q;
    a.dist[source] = 0; q.push({0,source});
    while (!q.empty()) {
        auto [du,u] = q.top(); q.pop();
        if (du != a.dist[u]) continue;
        for (auto [v,w] : g[u]) {
            assert(w >= 0);
            if (w >= INF-du) continue;
            if (du+w < a.dist[v]) {
                a.dist[v] = du+w; a.parent[v] = u; q.push({du+w,v});
            }
        }
    }
    return a;
}
// BOOK: Topological order
// Empty result means a cycle unless the input graph is empty.
inline std::vector<int> topological_order(const Graph& g) {
    int n = int(g.size());
    std::vector<int> deg(n), order;
    for (const auto& adj : g) for (int v : adj) ++deg[v];
    std::queue<int> q;
    for (int u = 0; u < n; ++u) if (!deg[u]) q.push(u);
    while (!q.empty()) {
        int u = q.front(); q.pop(); order.push_back(u);
        for (int v : g[u]) if (--deg[v] == 0) q.push(v);
    }
    if (int(order.size()) != n) return {};
    return order;
}
// BOOK: Disjoint sets and minimum spanning forests
struct DSU {
    std::vector<int> p, sz;
    explicit DSU(int n) : p(n), sz(n,1) { std::iota(p.begin(),p.end(),0); }
    int find(int u) {
        while (u != p[u]) { p[u] = p[p[u]]; u = p[u]; }
        return u;
    }
    bool unite(int u, int v) {
        u = find(u); v = find(v);
        if (u == v) return false;
        if (sz[u] < sz[v]) std::swap(u,v);
        p[v] = u; sz[u] += sz[v]; return true;
    }
    int size(int u) { return sz[find(u)]; }
};
struct Edge { int u,v; ll w; };
struct MST { ll weight = 0; int components = 0; std::vector<Edge> edges; };
inline MST kruskal(int n, std::vector<Edge> edges) {
    std::sort(edges.begin(),edges.end(),[](Edge a,Edge b){ return a.w < b.w; });
    DSU d(n); MST a; a.components = n;
    for (Edge e : edges) if (d.unite(e.u,e.v)) {
        a.weight += e.w; a.edges.push_back(e); --a.components;
    }
    return a;
}
// BOOK: Tree order, subtree sizes, and LCA
struct TreeInfo {
    std::vector<int> parent, depth, order, subtree;
};
inline TreeInfo root_tree(const Graph& tree, int root = 0) {
    int n = int(tree.size());
    assert(n > 0 && 0 <= root && root < n);
    TreeInfo t{std::vector<int>(n,-1),std::vector<int>(n),{},std::vector<int>(n,1)};
    t.parent[root] = root; t.order.push_back(root);
    for (int i = 0; i < int(t.order.size()); ++i) {
        int u = t.order[i];
        for (int v : tree[u]) if (t.parent[v] == -1) {
            t.parent[v] = u; t.depth[v] = t.depth[u]+1; t.order.push_back(v);
        }
    }
    assert(int(t.order.size()) == n);
    for (int i = n-1; i > 0; --i) {
        int u = t.order[i]; t.subtree[t.parent[u]] += t.subtree[u];
    }
    return t;
}
struct LCA {
    std::vector<std::vector<int>> up;
    std::vector<int> depth;
    explicit LCA(const TreeInfo& t) : depth(t.depth) {
        int n = int(depth.size()), levels = 1;
        assert(n > 0);
        while ((1LL << levels) <= n) ++levels;
        up.assign(levels,std::vector<int>(n)); up[0] = t.parent;
        for (int k = 1; k < levels; ++k)
            for (int u = 0; u < n; ++u) up[k][u] = up[k-1][up[k-1][u]];
    }
    int jump(int u, int steps) const {
        if (steps < 0 || steps > depth[u]) return -1;
        for (int k = 0; k < int(up.size()); ++k) if ((steps >> k)&1) u = up[k][u];
        return u;
    }
    int query(int u, int v) const {
        if (depth[u] < depth[v]) std::swap(u,v);
        u = jump(u,depth[u]-depth[v]);
        if (u == v) return u;
        for (int k = int(up.size())-1; k >= 0; --k) if (up[k][u] != up[k][v]) {
            u = up[k][u]; v = up[k][v];
        }
        return up[0][u];
    }
    int distance(int u, int v) const { return depth[u]+depth[v]-2*depth[query(u,v)]; }
};
// BOOK: Coordinate compression and prefix sums
template<class T> std::vector<T> compressed_values(std::vector<T> a) {
    std::sort(a.begin(),a.end()); a.erase(std::unique(a.begin(),a.end()),a.end());
    return a;
}
template<class T> int rank_of(const std::vector<T>& sorted_unique, const T& x) {
    auto it = std::lower_bound(sorted_unique.begin(),sorted_unique.end(),x);
    return int(it-sorted_unique.begin());
}
inline std::vector<ll> prefix_sums(const std::vector<ll>& a) {
    std::vector<ll> p(a.size()+1);
    for (int i = 0; i < int(a.size()); ++i) p[i+1] = p[i]+a[i];
    return p;
}
// BOOK: Difference arrays
inline void difference_add(std::vector<ll>& diff, int l, int r, ll value) {
    assert(0 <= l && l <= r && r < int(diff.size()));
    diff[l] += value; diff[r] -= value;
}
inline std::vector<ll> materialize_difference(const std::vector<ll>& diff) {
    assert(!diff.empty());
    std::vector<ll> a(diff.size()-1); ll cur = 0;
    for (int i = 0; i < int(a.size()); ++i) { cur += diff[i]; a[i] = cur; }
    return a;
}
// BOOK: Sliding window for nonnegative sums
// Number of nonempty subarrays with sum <= limit; all a[i] must be >= 0.
inline ll count_subarrays_at_most(const std::vector<ll>& a, ll limit) {
    if (limit < 0) return 0;
    int l = 0; ll sum = 0, answer = 0;
    for (int r = 0; r < int(a.size()); ++r) {
        assert(a[r] >= 0); sum += a[r];
        while (sum > limit) sum -= a[l++];
        answer += r-l+1;
    }
    return answer;
}
// BOOK: Monotone binary search
// Predicate is false, ..., false, true, ..., true on [lo,hi).
template<class Pred> ll first_true(ll lo, ll hi, Pred ok) {
    assert(0 <= lo && lo <= hi);
    while (lo < hi) {
        ll mid = lo+(hi-lo)/2;
        if (ok(mid)) hi = mid; else lo = mid+1;
    }
    return lo; // Original hi if every tested position is false.
}
// BOOK: Fenwick sums and order statistics
struct Fenwick {
    int n; std::vector<ll> bit;
    explicit Fenwick(int n_) : n(n_), bit(n_+1) {}
    void add(int i, ll delta) {
        assert(0 <= i && i < n);
        for (++i; i <= n; i += i&-i) bit[i] += delta;
    }
    ll prefix(int r) const {
        assert(0 <= r && r <= n);
        ll answer = 0;
        for (; r; r -= r&-r) answer += bit[r];
        return answer;
    }
    ll sum(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        return prefix(r)-prefix(l);
    }
    // Positive, one-based target; all represented values must be nonnegative.
    int lower_bound(ll target) const {
        assert(target > 0);
        int pos = 0, step = 1; ll accumulated = 0;
        while (step <= n/2) step *= 2;
        for (; step; step /= 2) {
            int next = pos+step;
            if (next <= n && bit[next] < target-accumulated) {
                accumulated += bit[next]; pos = next;
            }
        }
        return pos; // n means no prefix reaches target.
    }
};
// BOOK: Iterative segment tree
struct Sum { ll operator()(ll a,ll b) const { return a+b; } };
struct Minimum { ll operator()(ll a,ll b) const { return std::min(a,b); } };
template<class T, class Op> struct SegTree {
    int n, base = 1; T identity; Op op;
    std::vector<T> t;
    SegTree(const std::vector<T>& a, T identity_, Op op_ = Op())
        : n(int(a.size())), identity(identity_), op(op_) {
        while (base < n) base *= 2;
        t.assign(2*base,identity);
        for (int i = 0; i < n; ++i) t[base+i] = a[i];
        for (int i = base-1; i; --i) t[i] = op(t[2*i],t[2*i+1]);
    }
    void set(int i, T value) {
        assert(0 <= i && i < n);
        t[i += base] = value;
        while (i /= 2) t[i] = op(t[2*i],t[2*i+1]);
    }
    T query(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        T left = identity, right = identity;
        for (l += base,r += base; l < r; l /= 2,r /= 2) {
            if (l&1) left = op(left,t[l++]);
            if (r&1) right = op(t[--r],right);
        }
        return op(left,right);
    }
};
// BOOK: Capacity DP: 0-1 and unbounded knapsack
// Max value with total weight <= capacity. Taking no items is allowed.
inline ll knapsack01(const std::vector<int>& weight,
                     const std::vector<ll>& value, int capacity) {
    assert(weight.size() == value.size() && capacity >= 0);
    std::vector<ll> dp(capacity+1);
    for (int i = 0; i < int(weight.size()); ++i) {
        assert(weight[i] >= 0);
        for (int c = capacity; c >= weight[i]; --c)
            dp[c] = std::max(dp[c],dp[c-weight[i]]+value[i]);
    }
    return dp[capacity];
}
inline ll knapsack_unbounded(const std::vector<int>& weight,
                            const std::vector<ll>& value, int capacity) {
    assert(weight.size() == value.size() && capacity >= 0);
    std::vector<ll> dp(capacity+1);
    for (int i = 0; i < int(weight.size()); ++i) {
        assert(weight[i] > 0);
        for (int c = weight[i]; c <= capacity; ++c)
            dp[c] = std::max(dp[c],dp[c-weight[i]]+value[i]);
    }
    return dp[capacity];
}
// BOOK: Strict LIS with reconstruction
template<class T> std::vector<int> lis_indices(const std::vector<T>& a) {
    std::vector<T> tails;
    std::vector<int> ending, parent(a.size(),-1);
    for (int i = 0; i < int(a.size()); ++i) {
        auto it = std::lower_bound(tails.begin(),tails.end(),a[i]);
        int p = int(it-tails.begin());
        if (p > 0) parent[i] = ending[p-1];
        if (p == int(tails.size())) { tails.push_back(a[i]); ending.push_back(i); }
        else { tails[p] = a[i]; ending[p] = i; }
    }
    std::vector<int> result;
    if (!ending.empty())
        for (int i = ending.back(); i != -1; i = parent[i]) result.push_back(i);
    std::reverse(result.begin(),result.end()); return result;
}
} // namespace finals_core
