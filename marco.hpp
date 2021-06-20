#include <string>
#include <map>
#include <cstdio>

#ifdef MARCO_DEBUG
#define LOG(format, ...) printf(format, ##__VA_ARGS__)
#else 
#define LOG(format, ...) 
#endif

extern FILE *out;
extern std::map<std::string,std::string> definedMarcos;

static bool mute=false;

inline void warning(std::string s){
    printf("%s\n",s.data());
}
inline int REPLACE(std::string *p){
    if(!mute)
        if(definedMarcos.find(*p)!=definedMarcos.end()){
            fprintf(out,"%s",definedMarcos[*p].data());
            return 1;
        }
        else fprintf(out,p->data());
    return 0;
}
inline int MARCO_DEFINED(std::string *p){
    return definedMarcos.find(*p)!=definedMarcos.end();
}
inline int INSERT_MARCO(std::string *p,std::string *value=NULL){
    int ret=0;
    if(definedMarcos.find(*p)!=definedMarcos.end()){
        warning("marco redefined, the last one ignored");
        ret=1;
    }
    else if(value){
        definedMarcos.insert(make_pair(*p,*value));
        delete value;
    }
    else definedMarcos.insert(make_pair(*p,""));
    delete p;
    return ret;
}