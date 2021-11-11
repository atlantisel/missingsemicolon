#include <algorithm>    // equal, find
#include <string>
#include <vector>
#include "Functions.h"

using namespace std;

bool match(string str, vector<string> v) {
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
                    return tolower(a), tolower(b);
                 });
}