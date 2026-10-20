#include "../include/core.hpp"
#include <iostream>
#include <random>
#include <set>
using namespace finals_core;
std::mt19937 rng(20261018);
int rnd(int l,int r) { return std::uniform_int_distribution<int>(l,r)(rng); }
void shortest_tests() {
    for (int z = 0; z < 350; ++z) {
        int n = rnd(1,10);
        Graph g(n); WGraph weighted(n), binary(n);
        std::vector<std::vector<ll>> d(n,std::vector<ll>(n,INF));
        std::vector<std::vector<ll>> b = d;
        for (int u = 0; u < n; ++u) {
            d[u][u] = b[u][u] = 0;
            for (int v = 0; v < n; ++v) if (rnd(0,3) == 0) {
                ll w = rnd(0,20), bw = rnd(0,1);
                g[u].push_back(v); weighted[u].push_back({v,w});
                binary[u].push_back({v,bw});
                d[u][v] = std::min(d[u][v],w); b[u][v] = std::min(b[u][v],bw);
            }
        }
        for (int k = 0; k < n; ++k) for (int u = 0; u < n; ++u)
            for (int v = 0; v < n; ++v) {
                d[u][v] = std::min(d[u][v],d[u][k]+d[k][v]);
                b[u][v] = std::min(b[u][v],b[u][k]+b[k][v]);
            }
        for (int s = 0; s < n; ++s) {
            assert(dijkstra(weighted,s).dist == d[s]);
            assert(zero_one_bfs(binary,s).dist == b[s]);
            auto a = bfs(g,{s});
            std::vector<int> naive(n,-1); naive[s] = 0;
            for (int k = 0; k < n; ++k) for (int u = 0; u < n; ++u)
                if (naive[u] != -1) for (int v : g[u])
                    if (naive[v] == -1 || naive[v] > naive[u]+1) naive[v] = naive[u]+1;
            assert(a.dist == naive);
            for (int t = 0; t < n; ++t) {
                auto p = restore_path(a,t);
                if (naive[t] == -1) assert(p.empty());
                else {
                    assert(int(p.size()) == naive[t]+1 && p.front() == s && p.back() == t);
                    for (int i = 1; i < int(p.size()); ++i)
                        assert(std::find(g[p[i-1]].begin(),g[p[i-1]].end(),p[i]) != g[p[i-1]].end());
                }
            }
        }
        std::vector<int> sources;
        for (int i = 0; i < n; ++i) if (rnd(0,1)) sources.push_back(i);
        if (!sources.empty()) sources.push_back(sources.back());
        auto a = bfs(g,sources);
        for (int t = 0; t < n; ++t) {
            int best = n+1;
            for (int s : sources) { int dist = bfs(g,{s}).dist[t]; if (dist != -1) best = std::min(best,dist); }
            assert(a.dist[t] == (best == n+1 ? -1 : best));
        }
    }
    WGraph huge(3); huge[0] = {{1,INF-1},{2,INF}}; huge[1] = {{2,100}};
    auto d = dijkstra(huge,0).dist;
    assert(d[1] == INF-1 && d[2] == INF);
    assert(components(Graph{}).count == 0);
    assert(topological_order(Graph{}).empty());
    assert(grid_bfs({},{}).dist.empty());
    for (int z = 0; z < 100; ++z) {
        int h = rnd(1,8), w = rnd(1,8);
        std::vector<std::string> grid(h,std::string(w,'.')); Graph g(h*w);
        for (auto& row : grid) for (char& c : row) if (rnd(0,3)==0) c='#';
        for (int r = 0; r < h; ++r) for (int c = 0; c < w; ++c) if (grid[r][c] != '#')
            for (int nr = 0; nr < h; ++nr) for (int nc = 0; nc < w; ++nc)
                if (std::abs(r-nr)+std::abs(c-nc)==1 && grid[nr][nc] != '#')
                    g[r*w+c].push_back(nr*w+nc);
        int sr = rnd(0,h-1), sc = rnd(0,w-1);
        std::vector<int> src; if (grid[sr][sc] != '#') src.push_back(sr*w+sc);
        assert(grid_bfs(grid,{{sr,sc}}).dist == bfs(g,src).dist);
    }
}
void connectivity_tests() {
    for (int z = 0; z < 200; ++z) {
        int n = rnd(1,8); Graph g(n); DSU ds(n);
        std::vector<std::vector<bool>> reach(n,std::vector<bool>(n));
        for (int u = 0; u < n; ++u) {
            reach[u][u] = true;
            for (int v = 0; v < u; ++v) if (rnd(0,3)==0) {
                g[u].push_back(v); g[v].push_back(u); ds.unite(u,v);
                reach[u][v] = reach[v][u] = true;
            }
        }
        for (int k = 0; k < n; ++k) for (int u = 0; u < n; ++u)
            for (int v = 0; v < n; ++v) reach[u][v] = reach[u][v] || (reach[u][k] && reach[k][v]);
        auto c = components(g); std::set<int> ids;
        for (int u = 0; u < n; ++u) {
            ids.insert(c.id[u]); int size = 0;
            for (int v = 0; v < n; ++v) {
                assert((c.id[u] == c.id[v]) == reach[u][v]);
                assert((ds.find(u) == ds.find(v)) == reach[u][v]);
                size += reach[u][v];
            }
            assert(ds.size(u) == size && c.size[c.id[u]] == size);
        }
        assert(int(ids.size()) == c.count);
        for (int id = 0; id < c.count; ++id) assert(c.id[c.representative[id]] == id);
    }
    for (int z = 0; z < 150; ++z) {
        int n = rnd(1,6), m = rnd(0,9); std::vector<Edge> edges;
        for (int i = 0; i < m; ++i) edges.push_back({rnd(0,n-1),rnd(0,n-1),rnd(-5,15)});
        auto got = kruskal(n,edges); DSU all(n);
        for (auto e : edges) all.unite(e.u,e.v);
        int componentCount = 0;
        for (int u = 0; u < n; ++u) componentCount += all.find(u) == u;
        ll best = INF;
        for (int mask = 0; mask < (1<<m); ++mask) {
            DSU d(n); ll cost = 0; bool good = true; int used = 0;
            for (int i = 0; i < m; ++i) if (mask>>i&1) {
                good &= d.unite(edges[i].u,edges[i].v); cost += edges[i].w; ++used;
            }
            if (good && used == n-componentCount) best = std::min(best,cost);
        }
        assert(got.weight == best && got.components == componentCount);
        assert(int(got.edges.size()) == n-componentCount);
    }
    for (int z = 0; z < 100; ++z) {
        int n = rnd(1,15); Graph g(n);
        for (int u = 0; u < n; ++u) for (int v = u+1; v < n; ++v)
            if (rnd(0,3)==0) g[u].push_back(v);
        auto order = topological_order(g); assert(int(order.size()) == n);
        std::vector<int> pos(n);
        for (int i = 0; i < n; ++i) pos[order[i]] = i;
        for (int u = 0; u < n; ++u) for (int v : g[u]) assert(pos[u] < pos[v]);
        g[0].push_back(0); assert(topological_order(g).empty());
    }
}
void tree_tests() {
    for (int z = 0; z < 200; ++z) {
        int n = rnd(1,60); Graph g(n);
        for (int u = 1; u < n; ++u) { int v = rnd(0,u-1); g[u].push_back(v); g[v].push_back(u); }
        int root = rnd(0,n-1); auto t = root_tree(g,root); LCA a(t);
        assert(t.subtree[root] == n);
        for (int u = 0; u < n; ++u) {
            int size = 0;
            for (int v = 0; v < n; ++v) {
                int x = v;
                while (x != root && x != u) x = t.parent[x];
                size += x == u;
                int p = u, q = v;
                while (t.depth[p] > t.depth[q]) p = t.parent[p];
                while (t.depth[q] > t.depth[p]) q = t.parent[q];
                while (p != q) { p = t.parent[p]; q = t.parent[q]; }
                assert(a.query(u,v) == p);
                assert(a.distance(u,v) == bfs(g,{u}).dist[v]);
            }
            assert(t.subtree[u] == size);
            int x = u;
            for (int k = 0; k <= t.depth[u]; ++k) { assert(a.jump(u,k) == x); x = t.parent[x]; }
            assert(a.jump(u,t.depth[u]+1) == -1);
        }
    }
}
struct Concat { std::string operator()(const std::string& a,const std::string& b) const { return a+b; } };
void range_tests() {
    for (int z = 0; z < 250; ++z) {
        int n = rnd(0,35); std::vector<ll> a(n);
        for (ll& x : a) x = rnd(-10,10);
        auto p = prefix_sums(a), vals = compressed_values(a);
        for (int i = 0; i < n; ++i) assert(vals[rank_of(vals,a[i])] == a[i]);
        Fenwick f(n); for (int i = 0; i < n; ++i) f.add(i,a[i]);
        SegTree<ll,Sum> sum(a,0); SegTree<ll,Minimum> mn(a,std::numeric_limits<ll>::max());
        for (int l = 0; l <= n; ++l) for (int r = l; r <= n; ++r) {
            ll total = std::accumulate(a.begin()+l,a.begin()+r,0LL);
            assert(p[r]-p[l] == total && f.sum(l,r) == total && sum.query(l,r) == total);
        }
        for (int it = 0; it < 100; ++it) {
            if (n && rnd(0,1)) {
                int i = rnd(0,n-1); ll v = rnd(-30,30);
                f.add(i,v-a[i]); a[i] = v; sum.set(i,v); mn.set(i,v);
            }
            int l = rnd(0,n), r = rnd(l,n);
            ll total = std::accumulate(a.begin()+l,a.begin()+r,0LL);
            ll smallest = l == r ? std::numeric_limits<ll>::max() : *std::min_element(a.begin()+l,a.begin()+r);
            assert(sum.query(l,r) == total && mn.query(l,r) == smallest && f.sum(l,r) == total);
        }
        std::vector<ll> diff(n+1), naive(n);
        for (int it = 0; it < 60; ++it) {
            int l = rnd(0,n), r = rnd(l,n); ll delta = rnd(-10,10);
            difference_add(diff,l,r,delta);
            for (int i = l; i < r; ++i) naive[i] += delta;
        }
        assert(materialize_difference(diff) == naive);
        for (ll& x : a) x = rnd(0,8);
        ll limit = rnd(-1,30), answer = 0;
        for (int l = 0; l < n; ++l) { ll total = 0; for (int r = l; r < n; ++r) { total += a[r]; answer += total <= limit; } }
        assert(count_subarrays_at_most(a,limit) == answer);
        Fenwick freq(n); ll total = 0;
        for (int i = 0; i < n; ++i) { freq.add(i,a[i]); total += a[i]; }
        for (ll k = 1; k <= total+1; ++k) {
            ll cur = 0; int i = 0;
            while (i < n && cur+a[i] < k) { cur += a[i]; ++i; }
            assert(freq.lower_bound(k) == i);
        }
        for (int boundary = 0; boundary <= n+1; ++boundary)
            assert(first_true(0,n,[&](ll x){return x >= boundary;}) == std::min(boundary,n));
    }
    SegTree<std::string,Concat> cat({"a","b","c","d","e"},std::string{});
    assert(cat.query(1,5) == "bcde"); cat.set(2,"XYZ"); assert(cat.query(0,4) == "abXYZd");
}
void dp_tests() {
    for (int z = 0; z < 300; ++z) {
        int n = rnd(0,12), cap = rnd(0,20); std::vector<int> w(n); std::vector<ll> v(n);
        for (int i = 0; i < n; ++i) { w[i] = rnd(0,9); v[i] = rnd(-5,15); }
        ll best = 0;
        for (int mask = 0; mask < (1<<n); ++mask) {
            int weight = 0; ll value = 0;
            for (int i = 0; i < n; ++i) if (mask>>i&1) { weight += w[i]; value += v[i]; }
            if (weight <= cap) best = std::max(best,value);
        }
        assert(knapsack01(w,v,cap) == best);
        for (int& x : w) ++x;
        // Independent sum-first DP with exact weights, then maximum over all capacities.
        std::vector<ll> exact(cap+1,-INF); exact[0] = 0;
        for (int c = 1; c <= cap; ++c) for (int i = 0; i < n; ++i)
            if (w[i] <= c && exact[c-w[i]] != -INF) exact[c] = std::max(exact[c],exact[c-w[i]]+v[i]);
        assert(knapsack_unbounded(w,v,cap) == *std::max_element(exact.begin(),exact.end()));
        std::vector<int> a(n), dp(n,1); int length = 0;
        for (int i = 0; i < n; ++i) {
            a[i] = rnd(-10,10);
            for (int j = 0; j < i; ++j) if (a[j] < a[i]) dp[i] = std::max(dp[i],dp[j]+1);
            length = std::max(length,dp[i]);
        }
        auto ids = lis_indices(a); assert(int(ids.size()) == length);
        for (int i = 1; i < length; ++i) assert(ids[i-1] < ids[i] && a[ids[i-1]] < a[ids[i]]);
    }
}
int main() {
    shortest_tests(); connectivity_tests(); tree_tests(); range_tests(); dp_tests();
    std::cout << "core: randomized and edge-case oracle tests passed\n";
}
