#include "../include/math.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <set>
using namespace finals_math;
int main() {
    for(i64 a=-100;a<=100;++a)for(i64 b=-20;b<=20;++b)if(b) {
        assert(floor_div(a,b)==i64(std::floor((long double)a/b)));
        assert(ceil_div(a,b)==i64(std::ceil((long double)a/b)));
    }
    assert(floor_div(std::numeric_limits<i64>::min(),1)==std::numeric_limits<i64>::min());
    bool threw=false;try{floor_div(std::numeric_limits<i64>::min(),-1);}catch(const std::overflow_error&){threw=true;}assert(threw);
    assert(parse_fixed("12.05",2)==1205);assert(parse_fixed("-0.07",2)==-7);
    assert(parse_fixed("3",2)==300);assert(parse_fixed("+3.1",2)==310);
    assert(parse_fixed("-9223372036854775808",0)==std::numeric_limits<i64>::min());
    for(std::string s:{"12.005","1e2",".5","","+"}){
        threw=false;try{parse_fixed(s,2);}catch(const std::exception&){threw=true;}assert(threw);
    }
    threw=false;try{parse_fixed("9223372036854775808",0);}catch(const std::overflow_error&){threw=true;}assert(threw);
    assert(digits10(0)==1&&digits10(-1000)==4);
    assert(digits10(std::numeric_limits<i64>::min())==19);
    for(i64 m=2;m<=80;++m)for(i64 a=-50;a<=50;++a){
        auto inv=inverse(a,m);assert(bool(inv)==(std::gcd(norm(a,m),m)==1));
        if(inv)assert(mul_mod(a,*inv,m)==1);
        i64 r=1%m;
        for(int e=0;e<30;++e){assert(pow_mod(a,e,m)==r);r=(r*norm(a,m))%m;}
    }
    auto [g,x,y]=extended_gcd(6,9);assert(g==3&&6*x+9*y==g);
    for(i64 m=1;m<=20;++m)for(i64 n=1;n<=20;++n)
    for(i64 a=-2;a<m;++a)for(i64 b=-2;b<n;++b){
        auto r=crt({a,m},{b,n});i64 l=std::lcm(m,n),expected=-1;
        for(i64 k=0;k<l;++k)if(k%m==norm(a,m)&&k%n==norm(b,n)){expected=k;break;}
        assert(bool(r)==(expected>=0));if(r)assert(r->residue==expected&&r->modulus==l);
    }
    Sieve sieve(10000);
    for(i64 n=1;n<=10000;++n){
        auto f=factor(n);assert(f==sieve.factors(int(n)));i64 product=1;
        for(auto [p,e]:f){for(i64 d=2;d<=p/d;++d)assert(p%d);while(e--)product*=p;}
        assert(product==n);
        if(n<=1000){i64 coprime=0;for(i64 k=1;k<=n;++k)coprime+=std::gcd(k,n)==1;assert(phi(n)==coprime);}
    }
    for(i64 a=0;a<=120;++a)for(i64 b=1;b<=120;++b){
        auto shape=decimal_shape(a,b);std::map<i64,int> seen;i64 r=a%b;int steps=0;
        while(r&&!seen.count(r)){seen[r]=steps++;r=r*10%b;}
        int tail=r?seen[r]:steps,period=r?steps-seen[r]:0;
        assert(shape.nonrepeating==tail&&shape.period==period);
        assert(shape.integer_digits==int(std::to_string(a/b).size()));
    }
    for(i64 p:{7,101,1009}){
        int n=int(std::min<i64>(p-1,50));PrimeCombinations c(n,p);
        std::vector<i64> row(n+1);row[0]=1;
        for(int i=0;i<=n;++i){for(int k=0;k<=i;++k)assert(c.choose(i,k)==row[k]);for(int k=i+1;k>0;--k)if(k<=n)row[k]=(row[k]+row[k-1])%p;}
    }
    std::mt19937_64 rng(781);
    for(int t=0;t<3000;++t){std::vector<i64>a(rng()%30);for(auto&v:a)v=i64(rng()%11)-5;i64 brute=0;
        for(std::size_t i=0;i<a.size();++i)for(std::size_t j=i+1;j<a.size();++j)brute+=a[i]>a[j];
        assert(inversions(a)==brute);
    }
    for(int t=0;t<2000;++t){i64 X=1+i64(rng()%1000000000000000000ULL);auto f=fuzzy_factors(std::to_string(X));i128 Y=1;
        for(auto [p,e]:f)while(e--)Y*=p;
        assert(Y<=X&&(i128(X)-Y)*1000000000<=X);
    }
    std::string huge="1234567890"+std::string(991,'9');
    auto hugef=fuzzy_factors(huge);auto small=fuzzy_factors("1234567890");
    assert(hugef[2]==small[2]+991&&hugef[5]==small[5]+991);
    for(int t=0;t<3000;++t){Point a{i64(rng()%1000)-500,i64(rng()%1000)-500};Point b{i64(rng()%1000)-500,i64(rng()%1000)-500};Point c{i64(rng()%1000)-500,i64(rng()%1000)-500};
        i64 cross=(b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
        assert(orient(a,b,c)==cross);assert(orient(a,c,b)==-cross);
        assert(dist2(a,b)==(b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y));
    }
    assert(orient({-1000000000000000000LL,-1000000000000000000LL},
                  {1000000000000000000LL,-1000000000000000000LL},
                  {-1000000000000000000LL,1000000000000000000LL})==
                  i128(4000000000000000000LL)*1000000000000000000LL);
    std::cout<<"math helpers: exhaustive and randomized checks passed\n";
}
