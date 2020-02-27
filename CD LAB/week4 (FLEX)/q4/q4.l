%{
	int s=0;
	int p=0;
%}

%%

"scanf"	{fprintf(yyout,"READ");s++;}
"printf"	{fprintf(yyout,"WRITE");p++;}
.	{fprintf(yyout,"%s",yytext);}

%%

int yywrap()
{
	return 1;
}

int main()
{
	yyin = fopen("in.txt","r");
	yyout = fopen("out.txt","w");
	yylex();
	printf("printf:%d \tscanf:%d\n",p,s);
	return 0;
}