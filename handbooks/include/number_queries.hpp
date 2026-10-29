#pragma once
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>
namespace finals_number {
using namespace std;
using ll = long long;

// BOOK: Prime flags, list and inclusive range counts
struct PrimeTable {
	vector<char> prime;
	vector<int> prefix, primes;
	// 0<=limit<=10^7. O(V log log V) time, O(V) space.
	explicit PrimeTable(int limit) {
		if (limit < 0 || limit > 10000000)
			throw invalid_argument("prime table limit");
		prime.assign(limit + 1, true);
		prefix.assign(limit + 1, 0);
		prime[0] = false;
		if (limit >= 1) prime[1] = false;
		for (int p = 2; p <= limit / p; ++p)
			if (prime[p])
				for (ll x = 1LL * p * p; x <= limit; x += p)
					prime[x] = false;
		for (int x = 1; x <= limit; ++x) {
			prefix[x] = prefix[x - 1] + prime[x];
			if (prime[x]) primes.push_back(x);
		}
	}
	// Requires 0<=x<=limit; O(1).
	bool is_prime(int x) const { return prime.at(x); }
	// Inclusive [l,r], 0<=l<=r<=limit. O(1).
	int count(int l, int r) const {
		if (l < 0 || r < l || r >= int(prime.size()))
			throw invalid_argument("prime query range");
		return prefix[r] - (l ? prefix[l - 1] : 0);
	}
	// Same bounds; O(log pi(V) + output size).
	vector<int> list(int l, int r) const {
		count(l, r); // check bounds
		auto a = lower_bound(primes.begin(), primes.end(), l);
		auto b = upper_bound(primes.begin(), primes.end(), r);
		return vector<int>(a, b);
	}
};

// BOOK: Enumerate primes in a high narrow interval
// Inclusive [L,R]; 0<=L<=R<=10^12; width<=10^7.
// Base sieve to sqrt(R), then mark multiples within the
// window. Time O(B log log B + W log log B + pi(B) + W),
// B=sqrt(R). Space O(B+W), including returned primes.
inline vector<ll> segmented_primes(ll L, ll R) {
	if (L < 0 || R < L || R > 1000000000000LL ||
			R - L + 1 > 10000000)
		throw invalid_argument("segment bounds");
	ll B = sqrt((long double)R);
	while ((B + 1) * (B + 1) <= R) ++B;
	while (B * B > R) --B;
	PrimeTable base{int(B)};
	vector<char> good(size_t(R - L + 1), true);
	for (ll p : base.primes) {
		ll first = max(p * p, ((L + p - 1) / p) * p);
		for (ll x = first; x <= R; x += p) good[x - L] = false;
	}
	vector<ll> answer;
	for (ll x = L; x <= R; ++x)
		if (x >= 2 && good[x - L]) answer.push_back(x);
	return answer;
}

// BOOK: Number and sum of divisors for every bounded value
struct DivisorTable {
	vector<int> count;
	vector<ll> sum;
	// 0<=limit<=2*10^6. O(V log V) time, O(V) space.
	// Query only positive x<=limit. Slot zero is unused.
	explicit DivisorTable(int limit) {
		if (limit < 0 || limit > 2000000)
			throw invalid_argument("divisor table limit");
		count.assign(limit + 1, 0);
		sum.assign(limit + 1, 0);
		for (int d = 1; d <= limit; ++d)
			for (int x = d; x <= limit; x += d) {
				++count[x];
				sum[x] += d;
			}
	}
};
} // namespace finals_number
