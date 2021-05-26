#ifndef _RCC_GLOBAL_HPP
#define _RCC_GLOBAL_HPP
#include <string>

class RccGlobal {
    public:
    RccGlobal(std::string filename):in(filename) {}
    RccGlobal():in("") {}
    std::string in;
};

#endif