#include<stdio.h>
#define temp 33
int main()
{
	FILE *fa,*fb;
	fa=fopen("q2.c","r");
	fb=fopen("out2.txt","w");
	int ca;
	printf("\n#");
	ca=getc(fa);
	if(ca=='#')
		while(ca!='\n' && ca!=EOF)
			ca=getc(fa);
	while(ca!=EOF){
		if(ca=='"')	//very imp
		{
			putc(ca,fb);
			do
			{
				ca=getc(fa);
				putc(ca,fb);
			}while(ca!='"');
			ca=getc(fa);
		}
		else if(ca!='\n')
		{
			putc(ca,fb);
			ca=getc(fa);
		}
		else if(ca=='\n')
		{
			putc(ca,fb);
			ca=getc(fa);
			if(ca=='#')
				while(ca!='\n')
					ca=getc(fa);
		}
	}
}