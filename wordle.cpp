#ifndef RECCHECK
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"

using namespace std;

void makeWords(string pattern, string curr, string floating, int idx, int blanks, const set<string>& dict, set<string>& results)
{
    // finished building the string
    if(idx == (int)pattern.size()) {
        if(floating.size() == 0) {
            // check dictionary
            if(dict.find(curr) != dict.end()) {
                results.insert(curr);
            }
        }
        return;
    }

    // if it's a fixed letter (not '-')
    if(pattern[idx] != '-') {
        curr[idx] = pattern[idx];
        makeWords(pattern, curr, floating, idx+1, blanks, dict, results);
        return;
    }

    // blank '-'
    // if not enough blanks left to place all floating letters, fail early
    if((int)floating.size() > blanks) {
        return;
    }

    int nextBlanks = blanks - 1;

    // try placing floating letters first
    for(int i = 0; i < (int)floating.size(); i++) {
        char c = floating[i];
        curr[idx] = c;

        string newFloating = floating;
        newFloating.erase(i,1);  // remove the letter we used

        makeWords(pattern, curr, newFloating, idx+1, nextBlanks, dict, results);
    }

    // if we can still place letters that aren't floating
    if(blanks > (int)floating.size()) {
        for(char c = 'a'; c <= 'z'; c++) {
            // skip letters that are floating (already tried above)
            bool skip = false;
            for(int j = 0; j < (int)floating.size(); j++) {
                if(floating[j] == c) {
                    skip = true;
                    break;
                }
            }
            if(skip) continue;

            curr[idx] = c;
            makeWords(pattern, curr, floating, idx+1, nextBlanks, dict, results);
        }
    }
}

std::set<std::string> wordle(const std::string& in,
                             const std::string& floating,
                             const std::set<std::string>& dict)
{
    set<string> results;

    string curr = in;

    int blanks = 0;
    for(int i = 0; i < (int)in.size(); i++) {
        if(in[i] == '-') blanks++;
    }

    // call the messy helper
    makeWords(in, curr, floating, 0, blanks, dict, results);

    return results;
}
