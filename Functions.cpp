#include <algorithm>    // any_of, equal
#include <string>
#include <vector>
#include "Functions.h"

using namespace std;

bool inside(string str, vector<string> v) {
    // case-insensitive find function
    return any_of(v.begin(), v.end(),
                  [str](string key) {
                      return ciequal(str, key);
                  });
}

// case-insensitive comparison
bool ciequal(string a, string b) {
    return equal(a.begin(), a.end(),
                 b.begin(), b.end(),
                 [](char a, char b) {
                    return tolower(a) == tolower(b);
                 });
}

bool isnumstr(string str) {
    for (char chr : str) {
        if (!isdigit(chr)) // changed to isdigit due to isnumber is MacOS specific
            return false;
    }
    return true;
}

