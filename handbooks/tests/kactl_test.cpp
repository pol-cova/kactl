#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <climits>
#include <cstring>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;
#define rep(i,a,b) for(int i=(a); i<(b); ++i)
#define all(x) begin(x),end(x)
#define sz(x) int((x).size())
using ll=long long; using vi=vector<int>; using pii=pair<int,int>;
#include "../../content/strings/KMP.h"
#include "../../content/strings/Zfunc.h"
#include "../../content/strings/AhoCorasick.h"
#include "../../content/graph/BinaryLifting.h"
#include "../../content/data-structures/LineContainer.h"
#include "../../content/data-structures/Matrix.h"
#include "../../content/graph/SCC.h"
#include "../../content/graph/2sat.h"
#include "../../content/graph/Dinic.h"
#include "../../content/graph/HopcroftKarp.h"
#include "../../content/geometry/SegmentIntersection.h"
#include "../../content/geometry/ConvexHull.h"
#include "../../content/geometry/PolygonArea.h"
int main() {
    mt19937 rng(18102026);
    for(int it=0;it<600;++it) {
        string s,p;int n=int(rng()%30),m=1+int(rng()%8);
        for(int i=0;i<n;++i)s+=char('a'+rng()%3);
        for(int i=0;i<m;++i)p+=char('a'+rng()%3);
        vi expect;for(int i=0;i+m<=n;++i)if(s.substr(i,m)==p)expect.push_back(i);
        assert(match(s,p)==expect);
        auto zvalues=Z(s);
        for(int i=1;i<n;++i){int j=0;while(i+j<n&&s[j]==s[i+j])++j;assert(zvalues[i]==j);}
    }
    for(int it=0;it<350;++it) {
        int n=1+int(rng()%8); vector<vi> g(n);
        vector<vi> reach(n,vi(n));
        for(int i=0;i<n;++i){reach[i][i]=1;for(int j=0;j<n;++j)if(rng()%4==0){g[i].push_back(j);reach[i][j]=1;}}
        for(int k=0;k<n;++k)for(int i=0;i<n;++i)for(int j=0;j<n;++j)reach[i][j]|=reach[i][k]&&reach[k][j];
        int got=0;scc(g,[&](vi& block){got+=sz(block);});assert(got==n);
        for(int i=0;i<n;++i)for(int j=0;j<n;++j)assert((comp[i]==comp[j])==(reach[i][j]&&reach[j][i]));
    }
    for(int it=0;it<350;++it) {
        int n=1+int(rng()%7);TwoSat ts(n);vector<pii> clauses;
        auto lit=[&](){int v=int(rng()%n);return rng()%2?v:~v;};
        for(int j=0;j<14;++j){int a=lit(),b=lit();clauses.push_back({a,b});ts.either(a,b);}
        auto eval=[](int l,int mask){return l>=0?bool(mask&(1<<l)):!bool(mask&(1<<(~l)));};
        bool possible=false;for(int mask=0;mask<(1<<n);++mask){bool ok=true;for(auto [a,b]:clauses)ok&=eval(a,mask)||eval(b,mask);possible|=ok;}
        assert(ts.solve()==possible);
        if(possible){int mask=0;for(int j=0;j<n;++j)mask|=ts.values[j]<<j;for(auto [a,b]:clauses)assert(eval(a,mask)||eval(b,mask));}
    }
    for(int it=0;it<350;++it) {
        int n=2+int(rng()%6),s=0,t=n-1;Dinic f(n);vector<tuple<int,int,ll>> edges;
        for(int i=0;i<n;++i)for(int j=0;j<n;++j)if(i!=j&&rng()%4==0){ll c=rng()%12;edges.push_back({i,j,c});f.addEdge(i,j,c);}
        ll best=LLONG_MAX;for(int mask=0;mask<(1<<n);++mask)if((mask&1)&&!(mask&(1<<t))){ll cut=0;for(auto [a,b,c]:edges)if((mask&(1<<a))&&!(mask&(1<<b)))cut+=c;best=min(best,cut);}
        assert(f.calc(s,t)==best);ll cut=0;for(auto [a,b,c]:edges)if(f.leftOfMinCut(a)&&!f.leftOfMinCut(b))cut+=c;assert(cut==best);
    }
    for(int it=0;it<350;++it) {
        int n=int(rng()%7),m=int(rng()%7);vector<vi> g(n);for(int i=0;i<n;++i)for(int j=0;j<m;++j)if(rng()%2)g[i].push_back(j);
        function<int(int,int)> brute=[&](int u,int mask){if(u==n)return 0;int ans=brute(u+1,mask);for(int v:g[u])if(!(mask&(1<<v)))ans=max(ans,1+brute(u+1,mask|(1<<v)));return ans;};
        vi right(m,-1);assert(hopcroftKarp(g,right)==brute(0,0));
    }
    vector<P> square{P{0,0},P{2,0},P{2,2},P{0,2},P{1,1},P{2,0}};
    auto hull=convexHull(square);assert(hull.size()==4&&polygonArea2(hull)==8);
    assert(convexHull({}).empty());assert(convexHull({P{1,1},P{1,1}}).size()==1);
    using D=Point<double>;
    auto meet=segInter(D{0,0},D{1,1},D{0,1},D{1,0});
    assert(meet.size()==1&&abs(meet[0].x-.5)<1e-12);
    auto overlap=segInter(P{0,0},P{3,0},P{1,0},P{2,0});assert(overlap.size()==2);
    assert(segInter(P{0,0},P{1,0},P{2,0},P{3,0}).empty());
    vector<string> patterns{"AB","BAB","AB"};
    AhoCorasick aho(patterns); auto matches=aho.findAll(patterns,"ABAB");
    for(int i=0;i<4;++i) {
        vi expected;for(int j=0;j<3;++j)if(string("ABAB").substr(i,patterns[j].size())==patterns[j])expected.push_back(j);
        sort(all(matches[i]));assert(matches[i]==expected);
    }
    vi parent{0,0,0,1,1},depth{0,1,1,2,2};auto table=treeJump(parent);
    assert(jmp(table,4,2)==0&&lca(table,depth,3,4)==1&&lca(table,depth,2,4)==0);
    LineContainer lc;vector<pair<ll,ll>> lines;
    for(int i=0;i<150;++i) {
        ll k=int(rng()%101)-50,b=int(rng()%101)-50;lines.push_back({k,b});lc.add(k,b);
        for(ll x=-25;x<=25;++x) {ll want=LLONG_MIN;for(auto [kk,bb]:lines)want=max(want,kk*x+bb);assert(lc.query(x)==want);}
    }
    Matrix<ll,2> fib;fib.d[0][0]=fib.d[0][1]=fib.d[1][0]=1;
    array<ll,2> state{1,0};ll a=1,b=0;
    for(int i=0;i<40;++i){assert(((fib^i)*state)==(array<ll,2>{a,b}));ll c=a+b;b=a;a=c;}
    cout<<"KACTL selection: 2000 randomized oracle cases plus geometry boundaries passed\n";
}
