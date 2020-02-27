%{
	int wrd=0;
	int ch=0;
	int blank=0;
	int line=0;
%}

%%

[a-zA-Z]+	{wrd++;ch+=yyleng;}
[ ]	{blank++;ch++;}
\n	{line++;}
.	{ch++;}

%%

int yywrap()
{
	return 1;
}

int main(int argc, char *argv[])
{	
	yylex();
	printf("word:%d char:%d blank:%d line:%d\n",wrd,ch,blank,line);
	return 0;
}