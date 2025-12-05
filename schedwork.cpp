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

bool backtrackHelp(
    const AvailabilityMatrix& avail,
    size_t dNeed,
    size_t mShifts,
    DailySchedule& sched,
    vector<int>& used,
    int day,
    int slot
)
{
    int n = avail.size();
    int k = avail[0].size();

    
    if(day == n) {
        return true;
    }

    
    if(slot == (int)dNeed) {
        return backtrackHelp(avail, dNeed, mShifts, sched, used, day+1, 0);
    }

    
    for(int w = 0; w < k; w++) {

        
        if(!avail[day][w]) {
            continue; 
        }

        
        if(used[w] >= (int)mShifts) {
            continue;
        }

       
        bool already = false;
        for(int x = 0; x < (int)sched[day].size(); x++) {
            if(sched[day][x] == (Worker_T)w) {
                already = true;
                break;
            }
        }
        if(already) continue;

        
        sched[day][slot] = w;
        used[w]++;

       
        if(backtrackHelp(avail, dNeed, mShifts, sched, used, day, slot+1)) {
            return true;
        }

        
        used[w]--;
        sched[day][slot] = INVALID_ID;
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
    if(avail.size() == 0U) {
        return false; 
    }

    int n = avail.size();
    int k = avail[0].size();

    
    sched.clear();
    sched.resize(n);

    
    for(int i = 0; i < n; i++) {
        sched[i].resize(dailyNeed);
        for(int j = 0; j < (int)dailyNeed; j++) {
            sched[i][j] = INVALID_ID;
        }
    }

    
    vector<int> used(k, 0);

    
    bool ok = backtrackHelp(avail, dailyNeed, maxShifts, sched, used, 0, 0);

    return ok;
}
