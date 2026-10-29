#ifdef CASE_NATIVE
#include "../../content/finals-number/PrimeQueries.h"
#include "../../content/finals-number/SegmentedPrimes.h"
#include "../../content/finals-number/DivisorTable.h"
#else
#include "../include/number_queries.hpp"
#endif
#include <cassert>
#include <random>
using namespace finals_number;
bool trial(ll x) {
    if(x<2)return false;
    for(ll d=2;d<=x/d;++d)if(x%d==0)return false;
    return true;
}
int main() {
    for(int bound:{0,1,2,3,100,10000}) {
        PrimeTable t(bound);
        for(int x=0;x<=bound;++x)assert(t.is_prime(x)==trial(x));
        assert(t.count(0,bound)==int(t.primes.size()));
    }
    PrimeTable table(10000);
    std::mt19937 gen(987);
    for(int it=0;it<1000;++it) {
        int l=gen()%10001,r=gen()%10001;
        if(l>r)std::swap(l,r);
        vector<int> want;
        for(int x=l;x<=r;++x)if(trial(x))want.push_back(x);
        assert(table.count(l,r)==int(want.size()));
        assert(table.list(l,r)==want);
    }
    for(ll l=0;l<=150;++l)for(ll len=0;len<8;++len) {
        vector<ll> want;
        for(ll x=l;x<=l+len;++x)if(trial(x))want.push_back(x);
        assert(segmented_primes(l,l+len)==want);
    }
    for(ll l:{999900LL,1000000000LL,999999999980LL}) {
        vector<ll> want;
        for(ll x=l;x<=l+20;++x)if(trial(x))want.push_back(x);
        assert(segmented_primes(l,l+20)==want);
    }
    DivisorTable dt(1000);
    for(int x=1;x<=1000;++x) {
        int count=0;ll sum=0;
        for(int d=1;d<=x;++d)if(x%d==0){++count;sum+=d;}
        assert(dt.count[x]==count);assert(dt.sum[x]==sum);
    }
    assert(table.count(10,30)==6);
    assert((segmented_primes(10,30)==vector<ll>{11,13,17,19,23,29}));
    assert(dt.count[12]==6 && dt.sum[12]==28);
    bool bad=false;
    try { table.count(-1,3); } catch(const std::invalid_argument&) {bad=true;}
    assert(bad);
    bad=false;
    try { segmented_primes(0,10000000); } catch(const std::invalid_argument&) {bad=true;}
    assert(bad);
}
