#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace finals_math_extra {
using namespace std;
using i64 = long long;
// BOOK: Divisors
// Positive divisors in increasing order; O(sqrt(n)) scanning.
// Trial division is not for adversarial large 64-bit
// semiprimes.
inline vector<i64> positive_divisors(i64 n) {
	if (n < 1) throw invalid_argument("positive divisor input");
	vector<i64> lo, hi;
	for (i64 d = 1; d <= n / d; ++d) {
		if (n % d) continue;
		lo.push_back(d);
		if (d != n / d) hi.push_back(n / d);
	}
	reverse(hi.begin(), hi.end());
	lo.insert(lo.end(), hi.begin(), hi.end());
	return lo;
}
// BOOK: PrimeExponents
// Multiplier exponent vector making n a kth power.
// f has distinct PRIME bases and positive exponents;
// unchecked. k positive; nullopt means product exceeds signed
// 64-bit.
inline optional<i64>
kth_power_multiplier(const vector<pair<i64, int>> &f, int k) {
	if (k <= 0) throw invalid_argument("positive power");
	i64 ans = 1;
	for (auto [p, e] : f) {
		if (p < 2 || e <= 0)
			throw invalid_argument("prime exponent");
		int add = (k - e % k) % k;
		while (add--) {
			if (ans > numeric_limits<i64>::max() / p) return nullopt;
			ans *= p;
		}
	}
	return ans;
}
// Legendre formula; p must be prime and n>=0.
inline i64 factorial_valuation(i64 n, i64 p) {
	if (n < 0 || p < 2)
		throw invalid_argument("valuation input");
	i64 total = 0;
	while (n) {
		n /= p;
		total += n;
	}
	return total;
}
// BOOK: GCDPairs
// Exact gcd counts for unordered distinct-index pairs.
// Values positive and <=V; N<=INT_MAX; O(V log V + N).
inline vector<i64> gcd_pair_histogram(const vector<int> &a,
																			int V) {
	if (V < 1) throw invalid_argument("positive value bound");
	vector<i64> freq(size_t(V) + 1), exact(size_t(V) + 1);
	for (int x : a) {
		if (x < 1 || x > V)
			throw invalid_argument("gcd value range");
		++freq[x];
	}
	for (int d = V; d >= 1; --d) {
		i64 count = 0;
		for (i64 m = d; m <= V; m += d) count += freq[m];
		exact[d] = count * (count - 1) / 2;
		for (i64 m = 2LL * d; m <= V; m += d) exact[d] -= exact[m];
	}
	return exact;
}
// Vacuously true for fewer than two entries.
// Positive inputs, affordable V; same bounds as histogram.
inline bool same_pair_gcd(const vector<int> &a, int V) {
	auto h = gcd_pair_histogram(a, V);
	i64 n = a.size();
	if (n < 2) return true;
	int g = 0;
	for (int x : a) g = gcd(g, x);
	return h[g] == n * (n - 1) / 2;
}
// BOOK: GCDOrbits
// Addition by step on Z/nZ: gcd(n,step) cycles of this length.
// n>0; step may be any signed 64-bit integer.
inline pair<i64, i64> gcd_orbit_shape(i64 n, i64 step) {
	if (n < 1) throw invalid_argument("positive circle size");
	step %= n;
	if (step < 0) step += n;
	i64 cycles = gcd(n, step);
	return {cycles, n / cycles};
}
} // namespace finals_math_extra
