#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

#include "schedwork.h"
using namespace std;

static const Worker_T INVALID_ID = (unsigned int)-1;

bool go(
    const AvailabilityMatrix& a,
    size_t d,
    size_t m,
    DailySchedule& s,
    vector<int>& u,
    int day,
    int slot
)
{
    int n = a.size();
    int k = a[0].size();

    if(day >= n) return true;

    if(slot >= (int)d) {
        return go(a,d,m,s,u,day+1,0);
    }

    for(int w = 0; w < k; w++){
        int t1 = w; 
        int t2 = u[w]; 

        if(a[day][t1] == false) continue;

        if(t2 >= (int)m) continue;

        bool bad = false;
        for(size_t pp = 0; pp < s[day].size(); pp++){
            if(s[day][pp] == t1) {
                bad = true;
            }
        }
        if(bad) continue;

        int before = u[t1];
        s[day][slot] = t1;
        u[t1] = before + 1;

        if(go(a,d,m,s,u,day,slot+1)) return true;

        u[t1] = before;
        s[day][slot] = INVALID_ID;
    }

    return false;
}

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0) return false;

    int n = avail.size();
    int k = avail[0].size();

    sched.clear();
    sched.resize(n);

    for(int i = 0; i < n; i++){
        sched[i].resize(dailyNeed);
        for(size_t j = 0; j < dailyNeed; j++){
            sched[i][j] = INVALID_ID;
        }
    }

    vector<int> used(k);
    for(int i = 0; i < k; i++){
        used[i] = 0;
    }

    bool ok = go(avail, dailyNeed, maxShifts, sched, used, 0, 0);

    return ok;
}

