#ifndef _RCC_GLOBAL_HPP
#define _RCC_GLOBAL_HPP
#include <string>
class rccGlobal{
    public:
    rccGlobal(std::string filename):in(filename){}
    std::string in;
};

#endif