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

// i didn't know what to call this helper
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

    // if we filled everything??
    if(day == n) {
        return true;
    }

    // if we filled all d slots for this day, move to the next one
    if(slot == (int)dNeed) {
        return backtrackHelp(avail, dNeed, mShifts, sched, used, day+1, 0);
    }

    // try workers one by one
    for(int w = 0; w < k; w++) {

        // check availability first
        if(!avail[day][w]) {
            continue; // can't work today
        }

        // can't exceed max shifts
        if(used[w] >= (int)mShifts) {
            continue;
        }

        // also can't assign same worker twice in same day
        bool already = false;
        for(int x = 0; x < (int)sched[day].size(); x++) {
            if(sched[day][x] == (Worker_T)w) {
                already = true;
                break;
            }
        }
        if(already) continue;

        // looks ok, so try it
        sched[day][slot] = w;
        used[w]++;

        // recurse again
        if(backtrackHelp(avail, dNeed, mShifts, sched, used, day, slot+1)) {
            return true;
        }

        // undo it
        used[w]--;
        sched[day][slot] = INVALID_ID;
    }

    // nothing worked
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
        return false; // nothing to schedule LOL
    }

    int n = avail.size();
    int k = avail[0].size();

    // i think we need to resize sched first
    sched.clear();
    sched.resize(n);

    // set all slots to invalid so it's easier to track
    for(int i = 0; i < n; i++) {
        sched[i].resize(dailyNeed);
        for(int j = 0; j < (int)dailyNeed; j++) {
            sched[i][j] = INVALID_ID;
        }
    }

    // track how many shifts each worker has done
    vector<int> used(k, 0);

    // call the weird helper function
    bool ok = backtrackHelp(avail, dailyNeed, maxShifts, sched, used, 0, 0);

    return ok;
}
