#include "../include/core.hpp"
#include "../include/math.hpp"
#include "../include/digit_dp.hpp"
#include <cassert>
#include <climits>
#include <iostream>
#include <random>
using finals_math::i128;
void window_boundaries() {
    using finals_core::count_subarrays_at_most;
    assert(count_subarrays_at_most({LLONG_MAX,1},LLONG_MAX)==2);
    assert(count_subarrays_at_most({LLONG_MAX,LLONG_MAX},LLONG_MAX)==2);
    assert(count_subarrays_at_most({0,LLONG_MAX,0},LLONG_MAX)==6);
    std::mt19937_64 random(88018);
    for(int z=0;z<2000;++z) {
        int n=random()%12; std::vector<long long> a(n);
        for(auto& x:a) x=(random()%2)?LLONG_MAX-static_cast<long long>(random()%50):random()%50;
        long long k=(random()%2)?LLONG_MAX-static_cast<long long>(random()%50):random()%50;
        long long expected=0;
        for(int l=0;l<n;++l) {i128 sum=0; for(int r=l;r<n;++r) {sum+=a[r];expected+=sum<=k;}}
        assert(count_subarrays_at_most(a,k)==expected);
    }
}
void arithmetic_boundaries() {
    using namespace finals_math;
    for(i64 a:{LLONG_MIN,LLONG_MIN+1,-1LL,0LL,1LL,LLONG_MAX-1,LLONG_MAX})
    for(i64 b:{LLONG_MIN,-2LL,-1LL,1LL,2LL,LLONG_MAX}) {
        if(a==LLONG_MIN&&b==-1)continue;
        i128 f=floor_div(a,b),c=ceil_div(a,b),x=a,y=b;
        if(y<0) {x=-x;y=-y;}
        assert(f*y<=x && x<(f+1)*y);
        assert((c-1)*y<x && x<=c*y);
    }
    assert(parse_fixed("-9.223372036854775808",18)==LLONG_MIN);
    assert(parse_fixed("9.223372036854775807",18)==LLONG_MAX);
    assert(parse_fixed("000000000000000000000000000000000000001.2",1)==12);
    for(i64 mod:{1LL,2LL,LLONG_MAX-1,LLONG_MAX}) {
        assert(mul_mod(LLONG_MIN,LLONG_MAX,mod)==i64((i128(norm(LLONG_MIN,mod))*norm(LLONG_MAX,mod))%mod));
        assert(pow_mod(LLONG_MIN,0,mod)==1%mod);
    }
    for(i64 mod:{2LL,LLONG_MAX-1,LLONG_MAX}) {
        auto merged=crt({LLONG_MIN,mod},{LLONG_MAX,1});
        assert(merged && merged->residue==norm(LLONG_MIN,mod) && merged->modulus==mod);
        assert(crt({LLONG_MIN,mod},{LLONG_MIN,mod}));
    }
    bool overflow=false; try {crt({0,LLONG_MAX},{0,2});} catch(const std::overflow_error&) {overflow=true;}
    assert(overflow);
    auto shape=decimal_shape(0,LLONG_MAX);assert(shape.integer_digits==1&&shape.nonrepeating==0&&shape.period==0);
    finals_core::Fenwick f(4);f.add(0,LLONG_MAX-1);f.add(3,1);
    assert(f.lower_bound(LLONG_MAX)==3);
    assert(finals_core::Fenwick(0).lower_bound(1)==0);
    assert(finals_core::first_true(0,LLONG_MAX,[](long long x){return x>=LLONG_MAX-1;})==LLONG_MAX-1);
    assert(finals_dp::complete_digits("0",1,false)==std::optional<std::string>("0"));
    assert(!finals_dp::complete_digits("0",1,true));
}
void interval_oracle() {
    std::mt19937 rng(42);
    for (int z=0;z<1000;++z) {
        std::vector<std::pair<int,int>> a;
        for (int i=0,n=int(rng()%20);i<n;++i) {
            int l=int(rng()%21)-10,r=int(rng()%21)-10;
            if(l>r) std::swap(l,r);
            a.push_back({l,r});
        }
        auto merged=finals_core::merge_intervals(a);
        for(int x=-10;x<10;++x) {
            bool want=false,got=false;
            for(auto [l,r]:a) want|=l<=x&&x<r;
            for(auto [l,r]:merged) got|=l<=x&&x<r;
            assert(want==got);
        }
        for(int i=0;i<int(merged.size());++i) {
            assert(merged[i].first<merged[i].second);
            if(i) assert(merged[i-1].second<merged[i].first);
        }
    }
    auto a=finals_core::merge_intervals<double>({{0.5,1.5},{1.5,2.25},{4,4}});
    assert(a.size()==1 && a[0].first==0.5 && a[0].second==2.25);
}
int main(){interval_oracle();arithmetic_boundaries();window_boundaries();std::cout<<"audit: full-width arithmetic, interval and window oracle checks passed\n";}
