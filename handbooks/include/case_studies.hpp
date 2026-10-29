#pragma once
#include "../../content/finals-implementations/BinarySearch.h"
#include "../../content/finals-implementations/SegmentTree.h"
#include <array>
#include <map>
#include <optional>
#include <string>
namespace finals_cases {
using namespace std;
using ll = long long;
// BOOK: Strict two-dimensional donation chains
struct Donor {
	ll wealth, beauty, donation;
};
struct Maximum {
	ll operator()(ll a, ll b) const { return max(a, b); }
};
// Donations >=0; totals fit ll. Equal pairs contribute
// together.
inline ll max_donation_chain(vector<Donor> a) {
	sort(a.begin(), a.end(), [](Donor x, Donor y) {
		return pair{x.wealth, x.beauty} < pair{y.wealth, y.beauty};
	});
	vector<Donor> b;
	for (auto p : a) {
		assert(p.donation >= 0);
		if (!b.empty() && b.back().wealth == p.wealth &&
				b.back().beauty == p.beauty)
			b.back().donation += p.donation;
		else
			b.push_back(p);
	}
	vector<ll> ys;
	for (auto p : b) ys.push_back(p.beauty);
	sort(ys.begin(), ys.end());
	ys.erase(unique(ys.begin(), ys.end()), ys.end());
	finals_core::SegTree<ll, Maximum> tree(
			vector<ll>(ys.size(), 0), 0);
	ll answer = 0;
	for (int l = 0; l < int(b.size());) {
		int r = l;
		while (r < int(b.size()) && b[r].wealth == b[l].wealth)
			++r;
		vector<pair<int, ll>> pending;
		for (int i = l; i < r; ++i) {
			int y =
					int(lower_bound(ys.begin(), ys.end(), b[i].beauty) -
							ys.begin());
			ll value = tree.query(0, y) + b[i].donation;
			pending.push_back({y, value});
			answer = max(answer, value);
		}
		for (auto [y, value] : pending)
			tree.set(y, max(tree.query(y, y + 1), value));
		l = r;
	}
	return answer;
}
// BOOK: BFS with hostile arrival deadlines
// Exactly one A; rectangular . # A M grid; flattened size fits
// int.
inline optional<string>
escape_monsters(const vector<string> &a) {
	if (a.empty() || a[0].empty()) return nullopt;
	int h = int(a.size()), w = int(a[0].size()), n = h * w;
	for (auto &row : a) assert(int(row.size()) == w);
	const int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
	const string moves = "UDLR";
	vector<int> danger(n, -1), dist(n, -1), parent(n, -1),
			via(n);
	queue<int> q;
	int start = -1, starts = 0;
	for (int r = 0; r < h; ++r)
		for (int c = 0; c < w; ++c) {
			int u = r * w + c;
			if (a[r][c] == 'M') {
				danger[u] = 0;
				q.push(u);
			}
			if (a[r][c] == 'A') {
				start = u;
				++starts;
			}
		}
	assert(starts == 1);
	auto neighbor = [&](int u, int d) {
		int r = u / w + dr[d], c = u % w + dc[d];
		if (r < 0 || r >= h || c < 0 || c >= w || a[r][c] == '#')
			return -1;
		return r * w + c;
	};
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		for (int d = 0; d < 4; ++d) {
			int v = neighbor(u, d);
			if (v < 0 || danger[v] != -1) continue;
			danger[v] = danger[u] + 1;
			q.push(v);
		}
	}
	dist[start] = 0;
	q.push(start);
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		int r = u / w, c = u % w;
		if (r == 0 || r == h - 1 || c == 0 || c == w - 1) {
			string path;
			for (int v = u; v != start; v = parent[v])
				path += moves[via[v]];
			reverse(path.begin(), path.end());
			return path;
		}
		for (int d = 0; d < 4; ++d) {
			int v = neighbor(u, d), t = dist[u] + 1;
			if (v < 0 || dist[v] != -1) continue;
			if (danger[v] != -1 && t >= danger[v]) continue;
			dist[v] = t;
			parent[v] = u;
			via[v] = d;
			q.push(v);
		}
	}
	return nullopt;
}
// BOOK: Binary choices with commuting toggles
// k<=20 binary effects, represented in unsigned bits.
inline int min_toggle_count(const vector<unsigned> &effects,
														unsigned start, unsigned target) {
	int k = int(effects.size()), answer = k + 1;
	assert(k <= 20);
	for (unsigned s = 0; s < (1u << k); ++s) {
		unsigned state = start;
		for (int i = 0; i < k; ++i)
			if (s >> i & 1u) state ^= effects[i];
		if (state == target)
			answer = min(answer, __builtin_popcount(s));
	}
	return answer == k + 1 ? -1 : answer;
}
// BOOK: Minimum capacity for contiguous nonnegative jobs
// At most groups nonempty blocks; sum(a) fits ll; groups>0.
inline ll min_contiguous_capacity(const vector<ll> &a,
																	int groups) {
	assert(groups > 0);
	if (a.empty()) return 0;
	ll lo = 0, hi = 0;
	for (ll x : a) {
		assert(x >= 0);
		lo = max(lo, x);
		hi += x;
	}
	auto fits = [&](ll capacity) {
		int used = 1;
		ll load = 0;
		for (ll x : a) {
			if (x > capacity) return false;
			if (load > capacity - x) {
				++used;
				load = 0;
			}
			if (used > groups) return false;
			load += x;
		}
		return true;
	};
	// hi is proven feasible even though first_true never tests
	// it.
	return finals_core::first_true(lo, hi, fits);
}
// BOOK: Count exact subarray sums with signed entries
// Prefix sums, prefix-target, counts and answer must fit ll.
inline ll count_exact_sum(const vector<ll> &a, ll target) {
	map<ll, ll> freq;
	freq[0] = 1;
	ll prefix = 0, answer = 0;
	for (ll x : a) {
		prefix += x;
		auto it = freq.find(prefix - target);
		if (it != freq.end()) answer += it->second;
		++freq[prefix];
	}
	return answer;
}
} // namespace finals_cases
