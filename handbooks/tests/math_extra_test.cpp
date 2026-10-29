#ifdef CASE_NATIVE
#include "../../content/finals-math-extra/Divisors.h"
#include "../../content/finals-math-extra/PrimeExponents.h"
#include "../../content/finals-math-extra/GCDPairs.h"
#include "../../content/finals-math-extra/GCDOrbits.h"
#else
#include "../include/math_extra.hpp"
#endif
#include "../../content/finals-math/FactorSieve.h"
#include <cassert>
#include <numeric>
#include <random>
using namespace finals_math_extra;
int main() {
	assert((positive_divisors(72) == vector<i64>{1,2,3,4,6,8,9,12,18,24,36,72}));
	for (int n = 1; n <= 300; ++n) {
		vector<i64> want;
		for (int d=1; d<=n; ++d) if (n%d==0) want.push_back(d);
		assert(positive_divisors(n) == want);
	}
	assert(*kth_power_multiplier(finals_math::factor(72), 2) == 2);
	assert(*kth_power_multiplier(finals_math::factor(64), 3) == 1);
	assert(factorial_valuation(10, 2) == 8);
	assert(factorial_valuation(10, 5) == 2);
	for(int n=1;n<=80;++n)for(int k=1;k<=4;++k) {
        auto got=kth_power_multiplier(finals_math::factor(n),k);
        assert(got);
        long long want=0;
        for(long long root=1;root<=n;++root) {
            long long power=1;
            for(int j=0;j<k;++j)power*=root;
            if(power%n==0) { want=power/n;break; }
        }
        assert(*got==want);
    }
    assert(!kth_power_multiplier({{2,1}},64));
    for(int n=0;n<=100;++n)for(int p:{2,3,5,7}) {
        long long want=0;
        for(int i=1;i<=n;++i) {
            int x=i;
            while(x%p==0){++want;x/=p;}
        }
        assert(factorial_valuation(n,p)==want);
    }
    std::mt19937 rng(8721);
	for (int t=0; t<500; ++t) {
		int V=1+rng()%30, n=rng()%35;
		vector<int> a(n);
		for (int &x:a) x=1+rng()%V;
		auto h=gcd_pair_histogram(a,V);
		vector<i64> brute(V+1);
		for (int i=0;i<n;++i) for(int j=i+1;j<n;++j)
			++brute[std::gcd(a[i],a[j])];
		assert(h==brute);
        bool same=true;
        int first=0;
        for(int i=0;i<n;++i)for(int j=i+1;j<n;++j) {
            int g=std::gcd(a[i],a[j]);
            if(first && first!=g) same=false;
            first=g;
        }
        assert(same_pair_gcd(a,V)==same);
	}
	for (int n=1;n<=80;++n) for(int step=-100;step<=100;++step) {
		auto [cycles,len]=gcd_orbit_shape(n,step);
		assert(cycles*len==n);
		vector<bool> seen(n);
		int actual=0;
		for(int s=0;s<n;++s) if(!seen[s]) {
			++actual; int x=s, count=0;
			do { seen[x]=true; ++count; x=(x+step%n+n)%n; }
			while(x!=s);
			assert(count==len);
		}
		assert(actual==cycles);
	}
}
