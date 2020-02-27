#include<stdio.h>
int main()
{
	FILE *fa,*fb;
	fa=fopen("in1.txt","r");
	fb=fopen("out1.txt","w");
	int ca;
	ca=getc(fa);
	while(ca!=EOF){
		if(ca!=' ' && ca!='\t')
		{
			putc(ca,fb);
			ca=getc(fa);
		}
		else
		{
			putc(' ',fb);
			while(ca==' ' || ca=='\t')
				ca=getc(fa);
		}
	}
}