#ifndef _GLOBAL_HPP
#define _GLOBAL_HPP
#include <string>
class rccGlobal{
    public:
    rccGlobal(std::string filename):in(filename){}
    std::string in;
};

#endif