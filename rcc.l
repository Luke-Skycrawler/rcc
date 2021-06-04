%option noyywrap
D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include <cstdio>
#include <cstdlib>
#include <string>
#include "AST.hpp"
#include "RccGlobal.hpp"
#include "rcc.tab.hpp"
#define YY_DECL int yylex(yy::rcc::semantic_type *yylval, yy::rcc::location_type *yylloc, RccGlobal &global)
// make location include the current token
# define YY_USER_ACTION yylloc->columns(yyleng);
typedef yy::rcc::token token;
void count();
int check_type();
void comment();
%}
%%
%{
	yylloc->step();	
%}
	
"/*".*"*/"		{ count(); }
"//".*\n		{ yylloc->lines(1); }

"char"			{ count(); return(token::CHAR); }
"double"		{ count(); return(token::DOUBLE); }
"int"			{ count(); return(token::INT); }
"sizeof"		{ count(); return(token::SIZEOF); }
"struct"		{ count(); return(token::STRUCT); }
"if"			{ count(); return(token::IF); }
"else"			{ count(); return(token::ELSE); }
"for"			{ count(); return(token::FOR); }
"incto"			{ count(); return(token::INCTO); }
"decto"			{ count(); return(token::DECTO); }
"while"			{ count(); return(token::WHILE); }
"return"		{ count(); return(token::RETURN); }

\'.\'			{ count(); yylval->constant = new Nconstant("char", yytext[1]); return(token::CHAR_CONSTANT); }
[0-9]+			{ count(); yylval->constant = new Nconstant("int", atoi(yytext)); return(token::INT_CONSTANT); }
[0-9]+\.[0-9]+	{ count(); yylval->constant = new Nconstant("double", atof(yytext)); return(token::DOUBLE_CONSTANT); }   
[a-zA-Z_][a-zA-Z0-9_]*    { count(); std::string tmp(yytext); yylval->identifier = new Nidentifier(tmp); return(token::IDENTIFIER); }
\"(\\.|[^\\\"])*\"	{ count(); yylval->constant = new Nconstant("string_literal", yytext + 1); return(token::STRING_LITERAL); }

">>="			{ count(); return(token::RIGHT_ASSIGN); }
"<<="			{ count(); return(token::LEFT_ASSIGN); }
"+="			{ count(); return(token::ADD_ASSIGN); }
"-="			{ count(); return(token::SUB_ASSIGN); }
"*="			{ count(); return(token::MUL_ASSIGN); }
"/="			{ count(); return(token::DIV_ASSIGN); }
"%="			{ count(); return(token::MOD_ASSIGN); }
"&="			{ count(); return(token::AND_ASSIGN); }
"^="			{ count(); return(token::XOR_ASSIGN); }
"|="			{ count(); return(token::OR_ASSIGN); }
">>"			{ count(); return(token::RIGHT_OP); }
"<<"			{ count(); return(token::LEFT_OP); }
"++"			{ count(); return(token::INC_OP); }
"--"			{ count(); return(token::DEC_OP); }
"->"			{ count(); return(token::PTR_OP); }
"&&"			{ count(); return(token::AND_OP); }
"||"			{ count(); return(token::OR_OP); }
"<="			{ count(); return(token::LE_OP); }
">="			{ count(); return(token::GE_OP); }
"=="			{ count(); return(token::EQ_OP); }
"!="			{ count(); return(token::NE_OP); }
";"				{ count(); return(';'); }
("{"|"<%")		{ count(); return('{'); }
("}"|"%>")		{ count(); return('}'); }
","				{ count(); return(','); }
":"				{ count(); return(':'); }
"="				{ count(); return('='); }
"("				{ count(); return('('); }
")"				{ count(); return(')'); }
("["|"<:")		{ count(); return('['); }
("]"|":>")		{ count(); return(']'); }
"."			{ count(); return('.'); }
"&"			{ count(); return('&'); }
"!"			{ count(); return('!'); }
"~"			{ count(); return('~'); }
"-"			{ count(); return('-'); }
"+"			{ count(); return('+'); }
"*"			{ count(); return('*'); }
"/"			{ count(); return('/'); }
"%"			{ count(); return('%'); }
"<"			{ count(); return('<'); }
">"			{ count(); return('>'); }
"^"			{ count(); return('^'); }
"|"			{ count(); return('|'); }
"?"			{ count(); return('?'); }

[\t ]		{yylloc->step();}
[\n]		{yylloc->lines(1);}
.			{printf("unknown character %c, skipped",*yytext); }

%%
void comment()
{
	char c, c1;

loop:
	while ((c = getchar()) != '*' && c != 0)
		putchar(c);

	if ((c1 = getchar()) != '/' && c != 0)
	{
		ungetc(c1,stdin);
		goto loop;
	}

	if (c != 0)
		putchar(c1);
}


int column = 0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	ECHO;
}


int check_type()
{

	return(token::IDENTIFIER);
}

