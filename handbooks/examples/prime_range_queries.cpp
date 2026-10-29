#include "../include/number_queries.hpp"
#include <iostream>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int maximum, queries;
    cin >> maximum >> queries;
    finals_number::PrimeTable table(maximum);
    while (queries--) {
        int l, r;
        cin >> l >> r;
        cout << table.count(l, r) << '\n';
    }
}
