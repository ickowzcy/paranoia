#ifndef PARANOIA_CONCAT_H
#define PARANOIA_CONCAT_H

#include <sstream>

template <typename T>
std::string concat(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <typename T, typename ...Args>
std::string concat(T first, Args... args) {
    std::ostringstream oss;
    oss << first;
    oss << concat(args...);
    return oss.str();
}

#endif //PARANOIA_CONCAT_H
