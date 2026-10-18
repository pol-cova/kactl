#include "../include/digit_dp.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <random>
using namespace std;
int main() {
    mt19937 rng(12345);
    for(int it=0;it<500;++it) {
        int n=1+int(rng()%4),mod=1+int(rng()%40);
        string pattern;for(int i=0;i<n;++i)pattern+=rng()%2?'?':char('0'+rng()%10);
        for(bool forbid:{false,true}) {
            optional<string> want;string s=pattern;
            function<void(int,int)> search=[&](int pos,int rem){
                if(want)return;
                if(pos==n){if(!rem)want=s;return;}
                for(int d=0;d<10;++d)if(!(forbid&&pos==0&&d==0)&&
                    (pattern[pos]=='?'||pattern[pos]==char('0'+d))) {
                    s[pos]=char('0'+d);search(pos+1,(10*rem+d)%mod);
                }
            };
            search(0,0);assert(finals_dp::complete_digits(pattern,mod,forbid)==want);
        }
    }
    assert(*finals_dp::complete_digits("?2",7)=="42");
    assert(!finals_dp::complete_digits("0",1));
    assert(*finals_dp::complete_digits("0",1,false)=="0");
    cout<<"Digit remainder DP: 1000 exhaustive-enumeration comparisons passed\n";
}
