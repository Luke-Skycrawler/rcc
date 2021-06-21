#include <string>
#include <map>
#include <cstdio>

#ifndef NO_LOGGING
#define LOG(format, ...) printf(format, ##__VA_ARGS__)
#else
#define LOG(format, ...)
#endif

extern FILE *out;
extern std::map<std::string, std::string> definedMacros;
static std::map<std::string, std::vector<std::string>*> parameterizedMacros;

static std::string *param=NULL,*current_macro=NULL;
static bool mute = false, ifdef=false;
inline void warning(std::string s)
{
    printf("%s\n", s.data());
}
inline int INSERT_PARAM_MACRO(std::string *p,std::string *_param=NULL){
    auto v=new std::vector<std::string>;
    parameterizedMacros[*p]=v;
    param=_param;
    current_macro=p;
    return 0;
    // v->push_back()
}
inline int PARAM(std::string *p){
    if(param)
        if (*param==*p){
            parameterizedMacros[*current_macro]->push_back("");
            LOG("INSERTED PARAM\n");
        }
        else {
            parameterizedMacros[*current_macro]->push_back(*p); 
            LOG("INSERTED CONST\n");
        }
    return 0;
}
inline int REPLACE(std::string *p)
{
    if (!mute){
        if (definedMacros.find(*p) != definedMacros.end())
        {
            fprintf(out, "%s", definedMacros[*p].data());
            return 1;
        }
        else
            fprintf(out, p->data());
        // if (definedMacros)
    }
    
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