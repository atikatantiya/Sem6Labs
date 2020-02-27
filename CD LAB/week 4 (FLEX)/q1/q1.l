%{
	int vow=0;
	int con=0;
%}

%%

[aeiouAEIOU]	{vow++;}
[a-zA-Z]	{con++;}

%%

int yywrap()
{
	return 1;
}

int main(int argc, char *argv[])
{
	yylex();
	printf("vow:%d con:%d\n",vow,con);
	return 0;
}
