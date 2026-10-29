#pragma once
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>
using namespace std;
namespace finals_practical {
using ll = long long;

// BOOK: Sort linked records without losing associations
// Ascending key, original order breaks ties. O(n log n).
template <class T>
vector<int> sorted_indices(const vector<T> &key) {
	vector<int> p(key.size());
	iota(p.begin(), p.end(), 0);
	stable_sort(p.begin(), p.end(),
							[&](int a, int b) { return key[a] < key[b]; });
	return p;
}
// p must be a permutation of [0,a.size()).
template <class T>
vector<T> reordered(const vector<T> &a, const vector<int> &p) {
	vector<T> out;
	out.reserve(p.size());
	for (int i : p) out.push_back(a[i]);
	return out;
}

// BOOK: Validate an entire order before consuming inventory
using Recipe = vector<pair<int, ll>>;
struct Inventory {
	vector<ll> stock, need;
	vector<int> touched;
	// All stock and quantities nonnegative. IDs in range.
	explicit Inventory(vector<ll> initial)
			: stock(std::move(initial)), need(stock.size(), 0) {}
	// Duplicate ingredients/dishes accumulate. Failure is
	// atomic. O(order length + expanded ingredient entries +
	// touched).
	bool consume(const vector<Recipe> &recipes,
							 const vector<int> &order) {
		bool ok = true;
		for (int dish : order) {
			for (auto [id, q] : recipes[dish]) {
				if (q == 0) continue;
				if (need[id] == 0) touched.push_back(id);
				// Subtract first: no overflowing need + q.
				if (q > stock[id] - need[id]) {
					ok = false;
					break;
				}
				need[id] += q;
			}
			if (!ok) break;
		}
		for (int id : touched) {
			if (ok) stock[id] -= need[id];
			need[id] = 0;
		}
		touched.clear();
		return ok;
	}
};
// BOOK: Offline threshold queries on static ranges
struct ThresholdQuery {
	int l, r;
	ll minimum;
};
// Count a[i]>=minimum for l<=i<r; valid half-open ranges.
// O((n+q) log(n+q)) time, O(n+q) space; no updates.
inline vector<int>
threshold_counts(const vector<ll> &a,
								 const vector<ThresholdQuery> &queries) {
	int n = a.size(), q = queries.size();
	auto pos = sorted_indices(a);
	reverse(pos.begin(), pos.end());
	vector<int> ids(q), bit(n + 1), ans(q);
	iota(ids.begin(), ids.end(), 0);
	sort(ids.begin(), ids.end(), [&](int x, int y) {
		return queries[x].minimum > queries[y].minimum;
	});
	auto prefix = [&](int end) {
		int sum = 0;
		for (; end; end -= end & -end) sum += bit[end];
		return sum;
	};
	int active = 0;
	for (int id : ids) {
		auto [l, r, minimum] = queries[id];
		while (active < n && a[pos[active]] >= minimum) {
			for (int i = pos[active++] + 1; i <= n; i += i & -i)
				++bit[i];
		}
		ans[id] = prefix(r) - prefix(l);
	}
	return ans;
}
} // namespace finals_practical
