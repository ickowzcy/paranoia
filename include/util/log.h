#ifndef PARANOIA_LOG_H
#define PARANOIA_LOG_H

#include <iostream>

template <typename T>
void log(T value) {
    std::cerr << value;
}

template <typename T, typename ...Args>
void log(T first, Args... args) {
    std::cerr << first;
    log(args...);
}

#endif //PARANOIA_LOG_H
