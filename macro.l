%option noyywrap
%{
#include <cstdio>
#include <cstdlib>
#include <string>
#include "RccGlobal.hpp"
#include "macro.tab.hpp"
#define YY_DECL int macrolex(yy::macro::semantic_type *yylval, yy::macro::location_type *yylloc, RccGlobal &global)
// make location include the current token
#define YY_USER_ACTION yylloc->columns(yyleng);
extern FILE *out;
#define ECHO fprintf(out,"%s",yytext)

typedef yy::macro::token token;
%}
%%
%{
	yylloc->step();	
%}

"/*".*"*/"					{ ECHO; }
"//".*\n					{ ECHO; yylloc->lines(1); }
"else"						{ return(token::ELSE); }
"ifdef" 					{ return(token::IFDEF);}
"ifndef"					{ return(token::IFNDEF);}
"endif" 					{ return(token::ENDIF);}
"define" 					{ return(token::DEFINE);}
[a-zA-Z_][a-zA-Z0-9_]*    	{ yylval->macro_name = new std::string(yytext); return(token::IDENTIFIER); }
[\r\t ]						{ ECHO; yylloc->step();}
"\n"						{ ECHO; yylloc->lines(1); return('\n');}
"("							{ return('(');}
")"							{ return(')');}
"#"							{ return('#'); }
\"(\\.|[^\\\"])*\"|\'.\'	{ yylval->macro_name = new std::string(yytext); return(token::OTHER);}
[^#\r\t\n a-zA-Z_"'()]+		{ yylval->macro_name = new std::string(yytext); return(token::OTHER);}
%%

