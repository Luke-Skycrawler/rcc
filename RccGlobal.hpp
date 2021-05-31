#ifndef _GLOBAL_HPP
#define _GLOBAL_HPP
#include <string>
struct Buffer{
    char type;
    union{
        int i32;
        float f32;
        double f64;
        char u8[32];
        void *p;
    } val;
};
class RccGlobal{
    public:
    RccGlobal(std::string filename):in(filename){}
    std::string in;
    Buffer buf;
};
#endif