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
#include "rccGlobal.hpp"
#include "rcc.tab.hpp"
#include "codegen.h"
#define YY_DECL int yylex(struct PrototypeAST **yylval,yy::rcc::location_type *yylloc, rccGlobal &global)
// make location include the current token
# define YY_USER_ACTION yylloc->columns (yyleng);
typedef yy::rcc::token token;
void count();
int check_type();
void comment();
%}

%%
"/*"			{ comment(); }

"auto"			{ count(); return(token::AUTO); }
"break"			{ count(); return(token::BREAK); }
"case"			{ count(); return(token::CASE); }
"char"			{ count(); return(token::CHAR); }
"const"			{ count(); return(token::CONST); }
"continue"		{ count(); return(token::CONTINUE); }
"default"		{ count(); return(token::DEFAULT); }
"do"			{ count(); return(token::DO); }
"double"		{ count(); return(token::DOUBLE); }
"else"			{ count(); return(token::ELSE); }
"enum"			{ count(); return(token::ENUM); }
"extern"		{ count(); return(token::EXTERN); }
"float"			{ count(); return(token::FLOAT); }
"for"			{ count(); return(token::FOR); }
"goto"			{ count(); return(token::GOTO); }
"if"			{ count(); return(token::IF); }
"int"			{ count(); return(token::INT); }
"long"			{ count(); return(token::LONG); }
"register"		{ count(); return(token::REGISTER); }
"return"		{ count(); return(token::RETURN); }
"short"			{ count(); return(token::SHORT); }
"signed"		{ count(); return(token::SIGNED); }
"sizeof"		{ count(); return(token::SIZEOF); }
"static"		{ count(); return(token::STATIC); }
"struct"		{ count(); return(token::STRUCT); }
"switch"		{ count(); return(token::SWITCH); }
"typedef"		{ count(); return(token::TYPEDEF); }
"union"			{ count(); return(token::UNION); }
"unsigned"		{ count(); return(token::UNSIGNED); }
"void"			{ count(); return(token::VOID); }
"volatile"		{ count(); return(token::VOLATILE); }
"while"			{ count(); return(token::WHILE); }

{L}({L}|{D})*	{ 
	count();
	global.buf.type='s';
	strcpy(global.buf.val.u8,yytext);
	printf("In check_type:%s\n",global.buf.val.u8);
	return(check_type());
}

0[xX]{H}+{IS}?		{ count(); return(token::CONSTANT); }
0{D}+{IS}?		{ count(); return(token::CONSTANT); }
{D}+{IS}?		{ count(); return(token::CONSTANT); }
L?'(\\.|[^\\'])+'	{ count(); return(token::CONSTANT); }

{D}+{E}{FS}?		{ count(); return(token::CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ count(); return(token::CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ count(); return(token::CONSTANT); }

L?\"(\\.|[^\\"])*\"	{ count(); return(token::STRING_LITERAL); }

"..."			{ count(); return(token::ELLIPSIS); }
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
";"			{ count(); return(';'); }
("{"|"<%")		{ count(); return('{'); }
("}"|"%>")		{ count(); return('}'); }
","			{ count(); return(','); }
":"			{ count(); return(':'); }
"="			{ count(); return('='); }
"("			{ count(); return('('); }
")"			{ count(); return(')'); }
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

[ \t\v\n\f]		{ count(); }
.			{ /* ignore bad characters */ }

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
/*
* pseudo code --- this is what it should check
*
*	if (yytext == type_name)
*		return(TYPE_NAME);
*
*	return(IDENTIFIER);
*/

/*
*	it actually will only return IDENTIFIER
*/
	/* std::string key(yytext);
	if(bindings.count(key)==0) */
		return(token::IDENTIFIER);
	/* else return token::TYPE_NAME; */
}