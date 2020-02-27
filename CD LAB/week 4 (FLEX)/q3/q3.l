%{
	int pic=0;
	int ni=0;
	int pf=0;
	int nf=0;
%}

%%

[+]?(([0-9]+([\.][0-9]+)([eE][+-]?[0-9]+)?)|([0-9]+([eE][+-]?[0-9]+)))	{pf++;}
[-](([0-9]+([\.][0-9]+)([eE][+-]?[0-9]+)?)|([0-9]+([eE][+-]?[0-9]+)))	{nf++;}
[+]?[0-9]+	{pic++;}
[-][0-9]+	{ni++;}
.	{}

%%

int yywrap()
{
	return 1;
}

int main()
{
	yylex();
	printf("positive int:%d\t negative int:%d\t positive float:%d\t negative float:%d\n",pic,ni,pf,nf);
	return 0;
}