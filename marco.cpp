#include <cstdio>
#include <string>
#include <map>
#include "RccGlobal.hpp"
#include "marco.tab.hpp"
using namespace std;
std::map<std::string, std::string> definedMarcos;
FILE * out;
int preBuild(RccGlobal &global){
    // freopen(".LONG_AND_AWARD_NAME.c","w",stdout);
    out=fopen(".LONG_AND_AWARD_NAME.c","w");
    yy::marco marco_parser(global);
    int v = marco_parser.parse();
    fprintf(out,"\n");
    fclose(out);
    freopen(".LONG_AND_AWARD_NAME.c","r",stdin);
    return v;
}
#ifdef MARCO_DEBUG
int main(int argc,char **argv){
    RccGlobal global(argc > 1 ? argv[1] : "");
    if (argc == 2)
        freopen(argv[1], "r", stdin);
    preBuild(global);
}
#endif