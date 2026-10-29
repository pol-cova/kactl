#ifdef CASE_NATIVE
#include "../../content/finals-practical/LinkedSort.h"
#include "../../content/finals-practical/AtomicInventory.h"
#include "../../content/finals-practical/OfflineThreshold.h"
#else
#include "../include/practical.hpp"
#endif
#include <cassert>
#include <climits>
#include <random>
using namespace finals_practical;
int main() {
    std::mt19937 rng(9812);
    for (int t=0;t<1000;++t) {
        int n=rng()%30;
        vector<int> a(n), b(n), c(n);
        vector<pair<int,int>> expected;
        for(int i=0;i<n;++i) {
            a[i]=rng()%7;b[i]=i;c[i]=-i;
            expected.push_back({a[i],i});
        }
        sort(expected.begin(),expected.end());
        auto p=sorted_indices(a);
        auto x=reordered(a,p),y=reordered(b,p),z=reordered(c,p);
        for(int i=0;i<n;++i) {
            assert(x[i]==expected[i].first);
            assert(y[i]==expected[i].second);
            assert(z[i]==-y[i]);
        }
    }
    for(int t=0;t<500;++t) {
        int n=1+rng()%8, m=1+rng()%8;
        vector<ll> a(n);
        for(auto& v:a) v=rng()%30;
        Inventory inv(a);
        vector<Recipe> recipes(m);
        for(auto& r:recipes)
            for(int j=0,k=rng()%8;j<k;++j)
                r.push_back({rng()%n,rng()%6});
        for(int round=0;round<20;++round) {
            vector<int> order(rng()%8);
            vector<ll> req(n);
            for(auto& d:order) {
                d=rng()%m;
                for(auto [i,q]:recipes[d]) req[i]+=q;
            }
            bool ok=true;
            for(int i=0;i<n;++i) if(req[i]>a[i])ok=false;
            if(ok)for(int i=0;i<n;++i)a[i]-=req[i];
            assert(inv.consume(recipes,order)==ok);
            assert(inv.stock==a);
            for(auto q:inv.need)assert(q==0);
            assert(inv.touched.empty());
        }
    }
    for(int t=0;t<300;++t) {
        int n=rng()%30;
        vector<ll> a(n);
        for(auto& x:a)x=int(rng()%20)-10;
        vector<ThresholdQuery> qs;
        vector<int> want;
        for(int j=0;j<30;++j) {
            int l=rng()%(n+1),r=rng()%(n+1);
            if(l>r)swap(l,r);
            ll v=int(rng()%24)-12;
            qs.push_back({l,r,v});
            int count=0;
            for(int i=l;i<r;++i)count+=a[i]>=v;
            want.push_back(count);
        }
        assert(threshold_counts(a,qs)==want);
    }
    Inventory inv({LLONG_MAX});
    vector<Recipe> recipes={{{0,LLONG_MAX}},{{0,1}}};
    assert(!inv.consume(recipes,{0,1}));
    assert(inv.stock[0]==LLONG_MAX);
    assert(inv.consume(recipes,{0}));
    assert(inv.stock[0]==0);
}
