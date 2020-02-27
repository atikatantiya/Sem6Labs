%{
	#include<stdio.h>
%}

%%

[\t\n ]+	;
is |
was |
had |
eat |
take	{printf("%s is a verb\n",yytext);}
[a-zA-Z]+	{printf("%s is not a verb\n",yytext);}

%%

int yywrap() {
	return 1;
}

int main(int argc, char *argv[]) {
	yyin = fopen(argv[1],"r");
	yylex();
	return 0;
}
