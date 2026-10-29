#ifdef CASE_NATIVE
#include "../../content/finals-dp-extra/Unranking.h"
#include "../../content/finals-dp-extra/SubsetOrder.h"
#include "../../content/finals-dp-extra/TreeMatching.h"
#include "../../content/finals-dp-extra/SubtreeRanges.h"
#else
#include "../include/dp_extra.hpp"
#endif
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
using namespace std;
using namespace finals_dp_extra;

static long long brute_path(const vector<vector<i64>>& c) {
	int n = int(c.size()); vector<int> p(n); iota(p.begin(), p.end(), 0);
	i64 best = numeric_limits<i64>::max();
	do {
		i64 cost = 0;
		for (int i = 1; i < n; ++i) cost += c[p[i - 1]][p[i]];
		best = min(best, cost);
	} while (next_permutation(p.begin(), p.end()));
	return best;
}

static int brute_matching(int n, const vector<pair<int,int>>& edges) {
	int best = 0, m = int(edges.size());
	for (int mask = 0; mask < (1 << m); ++mask) {
		vector<char> seen(n); int count = 0; bool ok = true;
		for (int i = 0; i < m; ++i) if (mask >> i & 1) {
			auto [u,v] = edges[i];
			if (seen[u] || seen[v]) { ok = false; break; }
			seen[u] = seen[v] = true; ++count;
		}
		if (ok) best = max(best, count);
	}
	return best;
}

int main() {
    for(int n=0;n<=12;++n) {
        vector<string> all;
        for(int mask=0;mask<(1<<n);++mask) {
            if(mask & (mask<<1)) continue;
            string s;
            for(int b=n-1;b>=0;--b)s+=char('0'+((mask>>b)&1));
            all.push_back(s);
        }
        for(int k=1;k<=int(all.size());++k)
            assert(unrank_no_adjacent(n,k)==all[k-1]);
        assert(!unrank_no_adjacent(n,all.size()+1));
    }
    assert(unrank_no_adjacent(100,~0ULL).has_value());
	mt19937 rng(701);
	for (int n = 1; n <= 8; ++n) for (int it = 0; it < 50; ++it) {
		vector<vector<i64>> c(n, vector<i64>(n));
		for (auto& row : c) for (auto& x : row) x = rng() % 31;
		assert(hamiltonian_path(c) == brute_path(c));
	}
	for (int n = 1; n <= 12; ++n) for (int it = 0; it < 50; ++it) {
		vector<vector<int>> g(n); vector<pair<int,int>> edges;
		for (int v = 1; v < n; ++v) {
			int p = rng() % v; edges.push_back({p,v});
			g[p].push_back(v); g[v].push_back(p);
		}
		assert(maximum_tree_matching(g) == brute_matching(n, edges));
		auto r = subtree_ranges(g, rng() % n);
		for (int u = 0; u < n; ++u) {
			vector<int> got(r.order.begin() + r.tin[u],
							r.order.begin() + r.tout[u]);
			vector<int> parent(n, -2); int root = r.order[0];
			parent[root] = -1; vector<int> q{root};
			for (int i = 0; i < int(q.size()); ++i)
				for (int v : g[q[i]]) if (v != parent[q[i]])
					parent[v] = q[i], q.push_back(v);
			vector<int> want;
			for (int v = 0; v < n; ++v) {
				int x = v; while (x != -1 && x != u) x = parent[x];
				if (x == u) want.push_back(v);
			}
			sort(got.begin(), got.end()); sort(want.begin(), want.end());
			assert(got == want);
		}
	}
	assert(*hamiltonian_path({{0}}) == 0);
	cout << "Extra DP patterns: brute oracle comparisons passed\n";
}
