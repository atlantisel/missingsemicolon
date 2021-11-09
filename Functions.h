#ifndef Functions_h
#define Functions_h

#include <string>
#include <vector>

bool isMember(std::string, std::vector<std::string>);

// constexpr functions are implicitly inline
constexpr unsigned int hash(const char *s, int off = 0) {
    return !s[off] ? 5381 : (::hash(s, off + 1) * 33) ^ s[off];
}

#endif