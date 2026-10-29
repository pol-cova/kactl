#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
namespace finals_dp_extra {
using namespace std;
using i64 = long long;

// BOOK: SubsetOrder
// Minimum-cost Hamiltonian path in a complete directed matrix.
// n <= 18, costs in [0, 10^12]. O(n^2 2^n) time,
// O(n 2^n) memory. A path may start and end anywhere.
inline optional<i64>
hamiltonian_path(const vector<vector<i64>> &c) {
	int n = int(c.size());
	if (n == 0 || n > 18)
		throw invalid_argument("need 1..18 vertices");
	for (const auto &row : c) {
		if (int(row.size()) != n)
			throw invalid_argument("matrix must be square");
		for (i64 x : row)
			if (x < 0 || x > 1000000000000LL)
				throw invalid_argument("cost outside [0, 10^12]");
	}
	int count = 1 << n;
	const i64 inf = numeric_limits<i64>::max() / 4;
	vector<vector<i64>> dp(count, vector<i64>(n, inf));
	for (int v = 0; v < n; ++v) dp[1 << v][v] = 0;
	for (int mask = 1; mask < count; ++mask)
		for (int v = 0; v < n; ++v)
			if (mask >> v & 1) {
				i64 cur = dp[mask][v];
				if (cur == inf) continue;
				for (int w = 0; w < n; ++w)
					if (!(mask >> w & 1)) {
						int next = mask | (1 << w);
						dp[next][w] = min(dp[next][w], cur + c[v][w]);
					}
			}
	return *min_element(dp.back().begin(), dp.back().end());
}

// BOOK: TreeMatching
// Maximum number of pairwise vertex-disjoint edges in a tree.
// Requires symmetric adjacency of a simple undirected tree.
// Checks endpoints, arc count, traversal cycles/connectivity.
// O(n+m) time and memory.
inline int
maximum_tree_matching(const vector<vector<int>> &g) {
	int n = int(g.size());
	if (!n) throw invalid_argument("tree must be nonempty");
	long long arcs = 0;
	for (int u = 0; u < n; ++u) {
		arcs += g[u].size();
		for (int v : g[u])
			if (v < 0 || v >= n || v == u)
				throw invalid_argument("invalid tree edge");
	}
	if (arcs != 2LL * (n - 1))
		throw invalid_argument("not a tree");
	vector<int> par(n, -2), order{0};
	par[0] = -1;
	for (int i = 0; i < int(order.size()); ++i) {
		int u = order[i];
		for (int v : g[u])
			if (v != par[u]) {
				if (par[v] != -2) throw invalid_argument("not a tree");
				par[v] = u;
				order.push_back(v);
			}
	}
	if (int(order.size()) != n)
		throw invalid_argument("tree is disconnected");
	vector<int> free(n), used(n);
	for (int i = n - 1; i >= 0; --i) {
		int u = order[i], base = 0;
		for (int v : g[u])
			if (par[v] == u) base += max(free[v], used[v]);
		free[u] = base;
		used[u] = base;
		for (int v : g[u])
			if (par[v] == u)
				used[u] = max(used[u], base - max(free[v], used[v]) +
																	 free[v] + 1);
	}
	return max(free[0], used[0]);
}

// BOOK: SubtreeRanges
struct SubtreeRanges {
	vector<int> order, tin, tout;
};
// Iterative DFS flattening of a rooted tree at root. Subtree u
// is exactly order[tin[u]..tout[u]). Requires symmetric
// adjacency of a simple tree; checks
// endpoints/count/cycles/connectivity. O(n+m) time and memory;
// range works for Fenwick/segment trees.
inline SubtreeRanges
subtree_ranges(const vector<vector<int>> &g, int root = 0) {
	int n = int(g.size());
	if (!n || root < 0 || root >= n)
		throw invalid_argument("bad root/tree");
	long long arcs = 0;
	for (int u = 0; u < n; ++u) {
		arcs += g[u].size();
		for (int v : g[u])
			if (v < 0 || v >= n || v == u)
				throw invalid_argument("invalid tree edge");
	}
	if (arcs != 2LL * (n - 1))
		throw invalid_argument("not a tree");
	SubtreeRanges r;
	r.tin.resize(n);
	r.tout.resize(n);
	vector<int> par(n, -2), stack{root};
	par[root] = -1;
	while (!stack.empty()) {
		int u = stack.back();
		stack.pop_back();
		r.tin[u] = int(r.order.size());
		r.order.push_back(u);
		for (int v : g[u])
			if (v != par[u]) {
				if (par[v] != -2) throw invalid_argument("not a tree");
				par[v] = u;
				stack.push_back(v);
			}
	}
	if (int(r.order.size()) != n)
		throw invalid_argument("tree is disconnected");
	for (int u = 0; u < n; ++u) r.tout[u] = r.tin[u] + 1;
	for (int i = n - 1; i > 0; --i) {
		int u = r.order[i], p = par[u];
		r.tout[p] = max(r.tout[p], r.tout[u]);
	}
	return r;
}
// BOOK: Unrank binary strings with no adjacent ones
// 1-based rank among length-n strings, lexicographic 0
// before 1. Leading zeros allowed; 0<=n<=1000000, k>0. O(n)
// time/space. nullopt if fewer than k valid strings exist.
inline optional<string>
unrank_no_adjacent(int n, unsigned long long k) {
	if (n < 0 || n > 1000000 || k == 0)
		throw invalid_argument("unrank bounds");
	using U = unsigned long long;
	const U cap = k;
	vector<U> ways(n + 1, 1);
	if (n) ways[1] = min<U>(2, cap);
	for (int len = 2; len <= n; ++len) {
		U a = ways[len - 1], b = ways[len - 2];
		ways[len] = a >= cap - b ? cap : a + b;
	}
	if (ways[n] < k) return nullopt;
	string out;
	bool previous_one = false;
	for (int left = n; left; --left) {
		U zero_block = ways[left - 1];
		if (previous_one || k <= zero_block) {
			out += '0';
			previous_one = false;
		} else {
			k -= zero_block;
			out += '1';
			previous_one = true;
		}
	}
	return out;
}
} // namespace finals_dp_extra
