#ifndef Functions_h
#define Functions_h

#include <string>
#include <vector>

namespace util {
    bool contains(std::string, std::vector<std::string>);

    // case-insensitive comparison
    bool iequals(std::string, std::string);

    bool isnumber(std::string);

    // constexpr functions are implicitly inline
    constexpr unsigned int hash(const char *str, int off = 0) {
        return !str[off] ? 5381 : (util::hash(str, off + 1) * 33) ^ str[off];
    }
}

#endif