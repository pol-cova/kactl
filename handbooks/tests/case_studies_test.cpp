#define sz(x) int((x).size())
#ifdef CASE_NATIVE
#include "../../content/finals-patterns/Fundraising.h"
#include "../../content/finals-patterns/HazardEscape.h"
#include "../../content/finals-patterns/ToggleSubsets.h"
#include "../../content/finals-patterns/CapacitySearch.h"
#include "../../content/finals-patterns/ExactSum.h"
#else
#include "../include/case_studies.hpp"
#endif
#include <climits>
#include <iostream>
#include <random>
using namespace finals_cases;
std::mt19937 rng(20261019);
int rnd(int n){return int(rng()%unsigned(n));}
void donations(){
  for(int z=0;z<500;++z){
    int n=rnd(11);vector<Donor>a(n);
    for(auto &p:a)p={rnd(5),rnd(5),rnd(10)};
    ll want=0;
    for(int s=0;s<(1<<n);++s){
      ll sum=0;bool ok=true;
      for(int i=0;i<n;++i)if(s>>i&1){
        sum+=a[i].donation;
        for(int j=0;j<i;++j)if(s>>j&1){
          auto x=a[i],y=a[j];
          ok&=(x.wealth==y.wealth&&x.beauty==y.beauty)||
              (x.wealth<y.wealth&&x.beauty<y.beauty)||
              (x.wealth>y.wealth&&x.beauty>y.beauty);
        }
      }
      if(ok)want=max(want,sum);
    }
    assert(max_donation_chain(a)==want);
  }
  assert(max_donation_chain({{1,1,4},{1,2,7},{2,3,5}})==12);
}
void capacities(){
  for(int z=0;z<500;++z){
    int n=1+rnd(10),k=1+rnd(n+2);vector<ll>a(n);
    for(auto &x:a)x=rnd(15);
    ll want=LLONG_MAX;
    for(int s=0;s<(1<<(n-1));++s){
      if(1+__builtin_popcount(unsigned(s))>k)continue;
      ll largest=0,sum=0;
      for(int i=0;i<n;++i){
        sum+=a[i];
        if(i==n-1||(s>>i&1)){largest=max(largest,sum);sum=0;}
      }
      want=min(want,largest);
    }
    assert(min_contiguous_capacity(a,k)==want);
  }
  assert(min_contiguous_capacity({},1)==0);
  assert(min_contiguous_capacity({LLONG_MAX},2)==LLONG_MAX);
  assert(min_contiguous_capacity({LLONG_MAX-1,1},2)==LLONG_MAX-1);
  assert(min_contiguous_capacity({4,2,3,5},2)==8);
}
void exact_sums(){
  for(int z=0;z<1000;++z){
    vector<ll>a(rnd(25));for(auto &x:a)x=rnd(21)-10;
    ll target=rnd(31)-15,want=0;
    for(int l=0;l<int(a.size());++l){
      ll sum=0;for(int r=l;r<int(a.size());++r){sum+=a[r];want+=sum==target;}
    }
    assert(count_exact_sum(a,target)==want);
  }
}
vector<int> toggle_bfs(const vector<unsigned>&effects,unsigned start,int states){
  vector<int>d(states,-1);queue<unsigned>q;d[start]=0;q.push(start);
  while(!q.empty()){
    unsigned u=q.front();q.pop();
    for(unsigned e:effects){unsigned v=u^e;if(d[v]<0){d[v]=d[u]+1;q.push(v);}}
  }
  return d;
}
void toggles(){
  for(int z=0;z<150;++z){
    vector<unsigned>e(rnd(10));for(auto &x:e)x=unsigned(rnd(256));
    unsigned s=unsigned(rnd(256)),t=unsigned(rnd(256));
    assert(min_toggle_count(e,s,t)==toggle_bfs(e,s,256)[t]);
  }
  vector<unsigned> e(9);
  for(int r=0;r<3;++r)for(int c=0;c<3;++c)
    for(int y=0;y<3;++y)for(int x=0;x<3;++x)
      if(abs(r-y)+abs(c-x)<=1)e[r*3+c]|=1u<<(y*3+x);
  auto d=toggle_bfs(e,0,512);
  for(unsigned t=0;t<512;++t)assert(min_toggle_count(e,0,t)==d[t]);
}
int hazard_oracle(const vector<string>&a,const optional<string>&answer){
  int h=int(a.size()),w=int(a[0].size()),n=h*w,start=-1;
  vector<int> monster(n),player(n);
  for(int r=0;r<h;++r)for(int c=0;c<w;++c){
    if(a[r][c]=='M')monster[r*w+c]=1;
    if(a[r][c]=='A')start=r*w+c;
  }
  player[start]=1;
  auto adjacent=[&](int u,int v){return abs(u/w-v/w)+abs(u%w-v%w)==1;};
  int pathpos=start;
  for(int time=0;time<=n;++time){
    if(answer && time<=int(answer->size()))assert(!monster[pathpos]);
    bool exit=false;
    for(int u=0;u<n;++u)if(player[u] && (u/w==0||u/w==h-1||u%w==0||u%w==w-1))exit=true;
    if(exit){
      assert(answer && int(answer->size())==time);
      assert(pathpos/w==0||pathpos/w==h-1||pathpos%w==0||pathpos%w==w-1);
      return time;
    }
    vector<int>nextmonster=monster,nextplayer(n);
    for(int u=0;u<n;++u)for(int v=0;v<n;++v)
      if(monster[u]&&a[v/w][v%w]!='#'&&adjacent(u,v))nextmonster[v]=1;
    for(int u=0;u<n;++u)for(int v=0;v<n;++v)
      if(player[u]&&a[v/w][v%w]!='#'&&adjacent(u,v)&&!nextmonster[v])nextplayer[v]=1;
    if(answer && time<int(answer->size())){
      int r=pathpos/w,c=pathpos%w;char move=(*answer)[time];
      if(move=='U')--r;else if(move=='D')++r;else if(move=='L')--c;else if(move=='R')++c;else assert(false);
      assert(r>=0&&r<h&&c>=0&&c<w&&a[r][c]!='#');pathpos=r*w+c;
    }
    monster=nextmonster;player=nextplayer;
  }
  assert(!answer);return -1;
}
void hazards(){
  assert(escape_monsters({"A"})==optional<string>(""));
  assert(!escape_monsters({"###","#A#","###"}));
  for(int z=0;z<300;++z){
    int h=1+rnd(6),w=1+rnd(6);vector<string>a(h,string(w,'.'));
    for(auto &row:a)for(auto &c:row){int x=rnd(6);c=x==0?'#':x==1?'M':'.';}
    a[rnd(h)][rnd(w)]='A';hazard_oracle(a,escape_monsters(a));
  }
  vector<string>a={"#######","#.....#","#M.A..#","#.....#","#####.#"};
  assert(hazard_oracle(a,escape_monsters(a))==4);
}
int main(){donations();capacities();exact_sums();toggles();hazards();std::cout<<"Case studies: exhaustive subset/partition, state BFS and time-layer oracles passed\n";}
