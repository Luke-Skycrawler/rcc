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
static bool mute = false, ifdef=false, shut=false;
std::vector<bool> mute_restore,shut_restore;
inline void warning(std::string s)
{
    printf("%s\n", s.data());
}
inline int ECHO(const char *str){
    if(!mute && !shut)fprintf(out,"%s",str);
    LOG("%s\n",str);
    return 0;
}
inline int INSERT_PARAM_MACRO(std::string *p,std::string *_param=NULL){
    auto v=new std::vector<std::string>;
    parameterizedMacros[*p]=v;
    param=_param;
    current_macro=p;
    LOG("parameter: %s\n",param->data());
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
    if(param)
        LOG("current macro %s,param: %s,received %s\n",current_macro->data(),param->data(),p->data());
    if(param && *param==*p){
        if(_constant){
            parameterizedMacros[*current_macro]->push_back(*_constant);
            LOG("pushing %s\n",_constant->data());
        }
        parameterizedMacros[*current_macro]->push_back(""); // placeholder ""
        LOG("INSERTED PARAM\n");
        return NULL;    // constant string in macro with length 0 
    }
    else return CAT(p,_constant);
}

inline std::string * REPLACE_PARAM(std::string *p,std::string *_param){
    LOG("macro: %s, param:%s\n",p->data(),_param?_param->data():"none");
    shut=shut_restore.back();
    shut_restore.pop_back();
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
        LOG("assembling ,param= %s result = %s\n",_param->data(),unfolded->data());
        return unfolded;
    }
    if(_param){
        *p=*p+"( "+*_param+" )";
        //delete _param;
    }
    return p;   // keep the similar interface
}

inline std::string* ECHO_OR_REPLACE(std::string *p)
{
    LOG("echo_or_replace: %s\n",p->data());
    shut_restore.push_back(shut);
    if (!mute){
        if(parameterizedMacros.find(*p)!=parameterizedMacros.end()){
            shut=true;  // shut the '(' output
            return p;   // process later
        }
        if (definedMacros.find(*p) != definedMacros.end())
        {
            ECHO(definedMacros[*p].data());
            *p=definedMacros[*p];
        }
        else ECHO(p->data());
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