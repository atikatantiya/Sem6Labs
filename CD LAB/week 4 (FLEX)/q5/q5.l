%{
%}

%%

[a-zA-Z ]+(" and "|" but "|" or ")[a-zA-Z ]+	{printf("compound\n");}
[a-zA-Z ]+	{printf("simple\n");}
.	{}

%%

int yywrap()
{
	return 1;
}

int main()
{
	yylex();
	return 0;
}