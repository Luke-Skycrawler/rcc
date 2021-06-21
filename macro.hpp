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

static std::string *param=NULL,*current_macro=NULL,*tmp=NULL;
static bool mute = false, ifdef=false;
inline void warning(std::string s)
{
    printf("%s\n", s.data());
}
inline int ECHO(const char *str){
    if(!mute)fprintf(out,"%s",str);
    LOG("%s\n",str);
    return 0;
}
inline int INSERT_PARAM_MACRO(std::string *p,std::string *_param=NULL){
    auto v=new std::vector<std::string>;
    parameterizedMacros[*p]=v;
    param=_param;
    current_macro=p;
    return 0;
    // v->push_back()
}
inline std::string * CAT(std::string *p,std::string *_constant=NULL){
    if(!_constant)_constant=p;
    else {
        *_constant=*_constant+" "+*p;
        //delete p;
    }
    return _constant;
}
inline std::string * PARAM(std::string *p,std::string *_constant=NULL){
    // extract the parameters in macros
    if(param && *param==*p){
        if(_constant)parameterizedMacros[*current_macro]->push_back(*_constant);
        parameterizedMacros[*current_macro]->push_back(""); // placeholder ""
        LOG("INSERTED PARAM\n");
        return NULL;    // constant string in macro with length 0 
    }
    else return CAT(p,_constant);
}

inline std::string * REPLACE_PARAM(std::string *p,std::string *_param){
    LOG("identifier: %s\n",p->data());
    if(!mute && _param && parameterizedMacros.find(*p)!=parameterizedMacros.end()){
        // assemble all the parameters
        auto unfolded= new std::string("");
        for(auto it : *parameterizedMacros[*p])
            if(it==""){
                ECHO(_param->data());
                *unfolded+=*_param;
            }
            else {
                ECHO(it.data());
                *unfolded+=it;
            }
        //delete _param;
        //delete p;
        return unfolded;
    }
    if(_param){
        *p=*p+"( "+*_param+" )";
        //delete _param;
    }
    else *p+="( )";
    return p;   // keep the similar interface
}

inline std::string* ECHO_OR_REPLACE(std::string *p)
{
    LOG("identifier: %s\n",p->data());
    if (!mute){
        if (definedMacros.find(*p) != definedMacros.end())
        {
            fprintf(out, "%s", definedMacros[*p].data());
            *p=definedMacros[*p];
        }
        else
            fprintf(out, "%s",p->data());
    }
    return p;
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
        //delete value;
    }
    else
        definedMacros.insert(make_pair(*p, ""));
    //delete p;
    return ret;
}