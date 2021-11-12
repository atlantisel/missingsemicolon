#ifndef Functions_h
#define Functions_h

#include <string>
#include <vector>

bool inside(std::string, std::vector<std::string>);

// case-insensitive comparison
bool ciequal(std::string, std::string);

bool isnumstr(std::string);

// constexpr functions are implicitly inline
constexpr unsigned int hash(const char *str, int off = 0) {
    return !str[off] ? 5381 : (::hash(str, off + 1) * 33) ^ str[off];
}

#endif