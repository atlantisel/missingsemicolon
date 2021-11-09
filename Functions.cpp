#include <algorithm>    // find
#include <string>
#include <vector>
#include "Functions.h"

using namespace std;

bool isMember(string str, vector<string> vct) {
    return find(vct.begin(), vct.end(), str) != vct.end();
}

