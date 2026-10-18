#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace finals_math {
using i64 = long long;
using i128 = __int128_t; // GCC/Clang extension supported by our C++17 build.
// BOOK: Checked narrowing and signed division
inline i64 narrow(i128 x) {
    if (x < std::numeric_limits<i64>::min() || x > std::numeric_limits<i64>::max())
        throw std::overflow_error("signed 64-bit result");
    return static_cast<i64>(x);
}
// b != 0; throws when the quotient cannot fit i64 (LLONG_MIN / -1).
inline i64 floor_div(i64 a, i64 b) {
    if (!b) throw std::invalid_argument("zero denominator");
    i128 x=a, y=b, q=x/y, r=x%y;
    if (r && ((r<0)!=(y<0))) --q;
    return narrow(q);
}
inline i64 ceil_div(i64 a, i64 b) {
    if (!b) throw std::invalid_argument("zero denominator");
    i128 x=a, y=b, q=x/y, r=x%y;
    if (r && ((r<0)==(y<0))) ++q;
    return narrow(q);
}
// BOOK: Exact decimals and digit counts
// Exact fixed-point parse: optional sign; digits; optional dot and digits.
// Requires at least one digit before '.', <= places fractional digits, 0<=places<=18.
inline i64 parse_fixed(const std::string& s, int places) {
    if (places<0 || places>18) throw std::invalid_argument("decimal places");
    std::size_t j=0; bool neg=false;
    if (j<s.size() && (s[j]=='+' || s[j]=='-')) neg=s[j++]=='-';
    i128 value=0; int whole=0, frac=0;
    const i128 cap=i128(std::numeric_limits<i64>::max())+1;
    auto digit=[&](char c) {
        if (c<'0'||c>'9') throw std::invalid_argument("decimal digit");
        value=value*10+(c-'0');
        if (value>cap) throw std::overflow_error("decimal value");
    };
    while(j<s.size() && s[j]!='.') { digit(s[j++]); ++whole; }
    if(!whole) throw std::invalid_argument("integer part");
    if(j<s.size()) {
        ++j;
        while(j<s.size()) { digit(s[j++]); ++frac; }
    }
    if(frac>places) throw std::invalid_argument("too many fractional digits");
    while(frac++<places) {
        value*=10;
        if(value>cap) throw std::overflow_error("decimal value");
    }
    return narrow(neg ? -value : value);
}
inline int digits10(i64 x) { // Counts magnitude digits, including zero, excluding sign.
    i128 v=x; if(v<0) v=-v;
    int d=1; while(v>=10) { v/=10; ++d; } return d;
}
// BOOK: Modular arithmetic and binary powers
inline i64 norm(i64 a, i64 m) {
    if(m<=0) throw std::invalid_argument("positive modulus required");
    a%=m; return a<0 ? a+m : a;
}
inline i64 mul_mod(i64 a, i64 b, i64 m) {
    return i64(i128(norm(a,m))*norm(b,m)%m);
}
inline i64 pow_mod(i64 a, std::uint64_t e, i64 m) {
    a=norm(a,m); i64 r=1%m;
    while(e) { if(e&1) r=mul_mod(r,a,m); a=mul_mod(a,a,m); e>>=1; } return r;
}
// BOOK: Extended Euclid, inverses and CRT
// Requires nonnegative a,b. Returns g,x,y with a*x+b*y=g.
inline std::tuple<i64,i128,i128> extended_gcd(i64 a,i64 b) {
    if(a<0||b<0) throw std::invalid_argument("nonnegative gcd inputs");
    i128 r=a,s=b,x=1,y=0,u=0,v=1;
    while(s) {
        i128 q=r/s;
        i128 t=r-q*s; r=s; s=t;
        t=x-q*u; x=u; u=t;
        t=y-q*v; y=v; v=t;
    }
    return {i64(r),x,y};
}
inline std::optional<i64> inverse(i64 a,i64 m) {
    if(m<=1) throw std::invalid_argument("inverse modulus must exceed one");
    auto [g,x,y]=extended_gcd(norm(a,m),m); (void)y;
    if(g!=1) return std::nullopt;
    return i64((x%m+m)%m);
}
struct Congruence { i64 residue, modulus; };
// Merge x=a (mod m), x=b (mod n); nullopt = incompatible.
// Positive moduli. Throws if the merged lcm exceeds LLONG_MAX.
inline std::optional<Congruence> crt(Congruence a,Congruence b) {
    a.residue=norm(a.residue,a.modulus); b.residue=norm(b.residue,b.modulus);
    auto [g,x,y]=extended_gcd(a.modulus,b.modulus); (void)y;
    i128 delta=i128(b.residue)-a.residue;
    if(delta%g) return std::nullopt;
    i64 n=b.modulus/g;
    i128 t=(delta/g*x)%n; if(t<0)t+=n;
    i64 lcm=narrow(i128(a.modulus)*n);
    i128 r=(i128(a.residue)+i128(a.modulus)*t)%lcm;
    return Congruence{i64(r),lcm};
}
// BOOK: Trial factorization, totients and sieve
// Positive n; O(sqrt(n)) worst case. Suitable for n<=1e12 in modest counts.
inline std::vector<std::pair<i64,int>> factor(i64 n) {
    if(n<1) throw std::invalid_argument("positive factor input");
    std::vector<std::pair<i64,int>> f;
    for(i64 p=2;p<=n/p;p+=(p==2?1:2)) if(n%p==0) {
        int e=0; do {n/=p;++e;}while(n%p==0); f.push_back({p,e});
    }
    if(n>1)f.push_back({n,1}); return f;
}
inline i64 phi(i64 n) {
    i64 ans=n; for(auto [p,e]:factor(n)) { (void)e; ans=ans/p*(p-1); } return ans;
}
struct Sieve {
    std::vector<int> spf, primes;
    explicit Sieve(int n) {
        if(n<0) throw std::invalid_argument("nonnegative sieve limit");
        spf.resize(std::size_t(n)+1,0);
        for(int i=2;i<=n;++i) {
            if(!spf[i]) { spf[i]=i; primes.push_back(i); }
            for(int p:primes) { if(p>spf[i] || p>n/i)break; spf[p*i]=p; }
        }
    }
    std::vector<std::pair<i64,int>> factors(int x) const {
        if(x<1 || x>=int(spf.size())) throw std::invalid_argument("sieve range");
        std::vector<std::pair<i64,int>> f;
        while(x>1) {int p=spf[x],e=0;do{x/=p;++e;}while(x>1&&spf[x]==p);f.push_back({p,e});}
        return f;
    }
};
// BOOK: Multiplicative order and decimal shape
inline i64 order_mod(i64 a,i64 m) { // m>=2, gcd(a,m)=1; trial division cost applies.
    if(m<2 || std::gcd(norm(a,m),m)!=1) throw std::invalid_argument("order requires a unit");
    i64 k=phi(m);
    for(auto [p,e]:factor(k)) { (void)e; while(k%p==0&&pow_mod(a,k/p,m)==1)k/=p; }
    return k;
}
struct DecimalShape { int integer_digits, nonrepeating; i64 period; };
// a>=0, b>=1; period computation may be costly.
inline DecimalShape decimal_shape(i64 a,i64 b) {
    if(a<0||b<1) throw std::invalid_argument("nonnegative fraction");
    i64 q=b/std::gcd(a,b); int u=0,v=0;
    while(q%2==0) {q/=2;++u;} while(q%5==0) {q/=5;++v;}
    return {digits10(a/b),std::max(u,v),q==1?0:order_mod(10,q)};
}
// BOOK: Combinations under a prime modulus
// Prime p is a CALLER precondition; n<p. Constructor does not test primality.
struct PrimeCombinations {
    i64 p; std::vector<i64> fac,invfac;
    PrimeCombinations(int n,i64 prime):p(prime) {
        if(n<0||p<=1||n>=p)throw std::invalid_argument("need 0<=n<prime");
        fac.resize(n+1,1); invfac.resize(n+1,1);
        for(int i=1;i<=n;++i)fac[i]=mul_mod(fac[i-1],i,p);
        invfac[n]=pow_mod(fac[n],p-2,p);
        for(int i=n;i>0;--i)invfac[i-1]=mul_mod(invfac[i],i,p);
    }
    i64 choose(int n,int k)const {
        if(n<0||n>=int(fac.size()))throw std::invalid_argument("choose range");
        if(k<0||k>n)return 0;
        return mul_mod(fac[n],mul_mod(invfac[k],invfac[n-k],p),p);
    }
};
// BOOK: Strict inversions by compressed Fenwick tree
// Generic comparisons: counts i<j with a[i]>a[j], O(n log n), preserves input.
inline i64 inversions(std::vector<i64> a) {
    auto vals=a;
    std::sort(vals.begin(),vals.end());
    vals.erase(std::unique(vals.begin(),vals.end()),vals.end());
    std::vector<i64> bit(vals.size()+1); i64 answer=0,seen=0;
    for(i64 x:a) {
        int k=int(std::lower_bound(vals.begin(),vals.end(),x)-vals.begin())+1;
        i64 le=0; for(int j=k;j;j-=j&-j)le+=bit[j];
        answer=narrow(i128(answer)+seen-le); ++seen;
        for(int j=k;j<int(bit.size());j+=j&-j)++bit[j];
    }
    return answer;
}
// BOOK: Fuzzy factorization construction
// Positive canonical decimal input (no leading zero); factor the ten-digit prefix,
// then account for trailing powers of ten. Empty factors means Y=1.
inline std::map<i64,int> fuzzy_factors(const std::string& s) {
    if(s.empty()||s[0]=='0')throw std::invalid_argument("positive canonical decimal");
    for(char c:s)if(c<'0'||c>'9')throw std::invalid_argument("decimal digits");
    i64 m=0; int keep=int(std::min<std::size_t>(10,s.size()));
    for(int i=0;i<keep;++i)m=m*10+s[i]-'0';
    int e=int(s.size())-keep; std::map<i64,int> f;
    for(auto [p,k]:factor(m))f[p]=k;
    if(e) {f[2]+=e;f[5]+=e;} return f;
}
// BOOK: Exact integer geometry
struct Point { i64 x,y; };
// Coordinate magnitudes <=1e18; products/sums fit signed 128 bits under this bound.
inline i128 orient(Point a,Point b,Point c) {
    return (i128(b.x)-a.x)*(i128(c.y)-a.y)-(i128(b.y)-a.y)*(i128(c.x)-a.x);
}
inline i128 dot(Point a,Point b) {return i128(a.x)*b.x+i128(a.y)*b.y;}
inline i128 dist2(Point a,Point b) {i128 x=i128(a.x)-b.x,y=i128(a.y)-b.y;return x*x+y*y;}
} // namespace finals_math
