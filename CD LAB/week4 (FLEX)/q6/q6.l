%{
/* need this for the call to atof() below */
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

%}

DIGIT	[0-9]
LETTER	[a-zA-Z]
C	[//]
S	[ ]
L	[\n]
ID	({LETTER}|_)({LETTER}|{DIGIT}|_)*
DOUBLE	[+-]?{DIGIT}+(\.{DIGIT}+)?([Ee][+-]?{DIGIT}+)?
hex	(0[xX])({DIGIT}|[a-fA-F])*
AR_OP	("+"|"-"|"*"|"/"|"%")
String	(\"(.)*\")
Sym	[~@#$^&()_!|><'{}.,;=]
L_OP	("&&"|"||"|"!")
Comment2	({C}+(.)*{L})
MULTILINEC	([/][\*](.|[\n])*[\*][/])
%%
{DIGIT}+	{
			printf("An integer: %s\n",yytext);
		}
{DOUBLE}	{
			printf("A double: %s\n",yytext);
		}

{hex}		{
			printf("A hexadecimal: %s\n",yytext);
		}
{String}	{
			printf("A String: %s\n",yytext);
		}
{AR_OP}	{printf("An arithmetic op: %s\n",yytext);}
{L_OP}	{printf("A logical op: %s\n",yytext);}
{Comment2}	{printf("single line comment %s\n",yytext);}

{MULTILINEC}	{printf("multiline comment %s\n",yytext);}

void|int|double|while	{
			printf("A keyword: %s\n",yytext); 
		}
{ID}		{
			printf("An identifier: %s\n",yytext);
		}
{Sym}	{printf("A special symbol: %s\n",yytext);}
%%

int main()
{
	yyin=fopen("temp.c","r");
	yylex();
}

int yywrap() {
	return 1;
}
