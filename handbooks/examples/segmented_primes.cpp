#include "../include/number_queries.hpp"
#include <iostream>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long l, r;
    cin >> l >> r;
    auto primes = finals_number::segmented_primes(l, r);
    cout << primes.size() << '\n';
    for (auto p : primes) cout << p << '\n';
}
