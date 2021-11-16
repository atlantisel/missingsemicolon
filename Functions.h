#ifndef Functions_h
#define Functions_h

#include <algorithm>    // any_of, equal
#include <cctype>       // idigit
#include <string>
#include <vector>
#include "Classes.h"

namespace util {
    // case-insensitive comparison
    inline bool iequals(std::string a, std::string b) {
        return std::equal(a.begin(), a.end(),
                          b.begin(), b.end(),
                          [](char a, char b) {
                              return tolower(a) == tolower(b);
                          });
    }

    // case-insensitive find function
    // locate member in vector; distinct from "non-contiguous" Sentence::search()
    inline bool contains(std::string str, std::vector<std::string> v) {
        return std::any_of(v.begin(), v.end(),
                           [str](std::string key) {
                               return iequals(str, key);
                           });
    }

    inline bool contains(Item item, std::vector<Item> v) {
        return std::any_of(v.begin(), v.end(),
                           [item](Item key) {
                               return item == key;
                           });
    }

    inline bool isnumber(std::string str) {
        return std::find_if(str.begin(), str.end(),
                            [](char chr) {
                                return !std::isdigit(chr);
                            }) == str.end();
    }

    inline bool hasspaces(std::string str) {
        return std::find_if(str.begin(), str.end(),
                            [](char chr) {
                                return std::isspace(chr);
                            }) != str.end();
    }

    // constexpr functions are implicitly inline
    constexpr unsigned int hash(const char *str, int off = 0) {
        return !str[off] ? 5381 : (util::hash(str, off + 1) * 33) ^ str[off];
    }
}

#endif