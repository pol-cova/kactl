#pragma once
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
namespace finals_dp {
using namespace std;
// BOOK: Remainder DP with lexicographic reconstruction
// Nonempty pattern of digits and '?', positive modulus;
// indices fit int. smallest valid completion divisible by
// modulus, or nullopt. With forbid_leading_zero, even a
// one-digit zero is forbidden. Time O(length * modulus * 10),
// memory O(length * modulus).
inline optional<string>
complete_digits(const string &pattern, int modulus,
								bool forbid_leading_zero = true) {
	if (pattern.empty() || modulus <= 0)
		throw invalid_argument(
				"nonempty pattern and positive modulus required");
	for (char c : pattern)
		if (c != '?' && (c < '0' || c > '9'))
			throw invalid_argument(
					"pattern must contain digits or question marks");
	int n = int(pattern.size());
	vector<vector<unsigned char>> can(
			n + 1, vector<unsigned char>(modulus, 0));
	can[n][0] = 1;
	auto allowed = [&](int pos, int digit) {
		return !(forbid_leading_zero && pos == 0 && digit == 0) &&
					 (pattern[pos] == '?' ||
						pattern[pos] == char('0' + digit));
	};
	for (int pos = n - 1; pos >= 0; --pos)
		for (int rem = 0; rem < modulus; ++rem)
			for (int d = 0; d <= 9; ++d)
				if (allowed(pos, d)) {
					int next = int((10LL * rem + d) % modulus);
					if (can[pos + 1][next]) {
						can[pos][rem] = 1;
						break;
					}
				}
	if (!can[0][0]) return nullopt;
	string answer;
	int rem = 0;
	for (int pos = 0; pos < n; ++pos)
		for (int d = 0; d <= 9; ++d)
			if (allowed(pos, d)) {
				int next = int((10LL * rem + d) % modulus);
				if (can[pos + 1][next]) {
					answer += char('0' + d);
					rem = next;
					break;
				}
			}
	return answer;
}
} // namespace finals_dp
