#include "../include/number_queries.hpp"
#include <iostream>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    vector<int> values(q);
    int maximum = 0;
    for (int& x : values) {
        cin >> x;
        maximum = max(maximum, x);
    }
    finals_number::DivisorTable table(maximum);
    for (int x : values)
        cout << table.count[x] << ' ' << table.sum[x] << '\n';
}
