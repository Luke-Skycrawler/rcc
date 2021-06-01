#ifndef _GLOBAL_HPP
#define _GLOBAL_HPP
#include <string>
struct Buffer{
    char type,advice;
    // type is passed from yacc to lex, while advice the opposite
    union{
        int i32;
        float f32;
        double f64;
        char u8[32];
        void *p;
    } val;
};
class rccGlobal{
    public:
    rccGlobal(std::string filename):in(filename){}
    std::string in;
    Buffer buf;
};
#endif