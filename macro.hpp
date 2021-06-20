#include <string>
#include <map>
#include <cstdio>

#ifdef MACRO_DEBUG
#define LOG(format, ...) printf(format, ##__VA_ARGS__)
#else
#define LOG(format, ...)
#endif

extern FILE *out;
extern std::map<std::string, std::string> definedMacros;

static bool mute = false;

inline void warning(std::string s)
{
    printf("%s\n", s.data());
}
inline int REPLACE(std::string *p)
{
    if (!mute)
        if (definedMacros.find(*p) != definedMacros.end())
        {
            fprintf(out, "%s", definedMacros[*p].data());
            return 1;
        }
        else
            fprintf(out, p->data());
    return 0;
}
inline int MACRO_DEFINED(std::string *p)
{
    return definedMacros.find(*p) != definedMacros.end();
}
inline int INSERT_MACRO(std::string *p, std::string *value = NULL)
{
    int ret = 0;
    if (definedMacros.find(*p) != definedMacros.end())
    {
        warning("macro redefined, the last one ignored");
        ret = 1;
    }
    else if (value)
    {
        definedMacros.insert(make_pair(*p, *value));
        delete value;
    }
    else
        definedMacros.insert(make_pair(*p, ""));
    delete p;
    return ret;
}