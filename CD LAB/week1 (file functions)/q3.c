#include<stdio.h>
#include<string.h>
#include<ctype.h>
int main()
{
	FILE *fa,*fb;
	fa=fopen("q3.c","r");
	fb=fopen("out3.txt","w");
	char *arr[]={"auto"	,"break" ,"case" ,"char","const" ,"continue" ,"default" ,"do","double" ,"else" ,"enum" ,"extern","float" ,"for" ,"goto" ,"if","int" ,"long" ,"register" ,"return","short" ,"signed" ,"sizeof","static","struct" ,"switch" ,"typedef" ,"union","unsigned" ,"void" ,"volatile" ,"while"};
	char arrp[400]="";
	int ca;
	int col=1,row=1;
	int j=0;
	fprintf(fb,"Word\tRow\tColumn\n");
	ca=getc(fa);
	while(ca!=EOF){
		if(ca=='"')
		{
			do
			{
				ca=getc(fa);
				col++;
			}while(ca!='"');
			ca=getc(fa);
			col++;
		}
		else if(isalpha(ca))
		{
			j=0;
			memset(arrp,0, sizeof(arrp));
			while(ca!='\n' && ca!=' ' && ca!='(' && ca!='<' && ca!='\t')
			{
				arrp[j++]=ca;
				ca=getc(fa);
				col++;
			}
			for(int i=0;i<32;i++)
			{
				if(strcmp(arrp,arr[i])==0)
				{
					for(int i=0;i<strlen(arrp);i++)
						arrp[i]=toupper(arrp[i]);
					fprintf(fb,"%s: \t%d \t%ld\n",arrp,row,col-strlen(arrp));
				}
			}
		}
		if(ca=='\n')
		{
			row++;
			col=0;
		}
		ca=getc(fa);
		col++;
	}
}