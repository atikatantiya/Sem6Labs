#include "../lex.h"

token lookahead;

/*
E -> TE'
E'-> +TE' | $
T -> FT'
T'-> *FT' | $
F -> (E) | id
*/

void procE();
void procE1();
void procT();
void procT1();
void procF();
void success();
void error();

void retract() {
	int len = (-1) * strlen(lookahead.lex);
	fseek(fd,len,SEEK_CUR);
}

//E -> TE'
void procE() {
	printf ("E %s\n",lookahead.lex);
	procT();
	procE1();
}

//E'-> +TE' | $
void procE1() {
	lookahead = getNextToken();
	printf ("E1 %s\n",lookahead.lex);
	if (strcmp(lookahead.lex,"+") == 0)	{
		procT();
		procE1();
	}
	else
		retract();
}

//T -> FT'
void procT() {
	printf ("T %s\n",lookahead.lex);
	procF();
	procT1();
}

//T'-> *FT' | $
void procT1() {
	lookahead = getNextToken();
	printf ("T1 %s\n",lookahead.lex);
	if (strcmp(lookahead.lex,"*") == 0) {
		procF();
		procT1();
	}
	else
		retract();
}

//F -> (E) | id
void procF() {
	lookahead = getNextToken();
	printf ("F %s\n",lookahead.lex);
	if (strcmp(lookahead.lex,"(") == 0)	{		
		procE();
		lookahead = getNextToken();
		if (strcmp(lookahead.lex,")") == 0)
			return;
		else
			error();
	}
	else if(strcmp(strType[lookahead.type],"IDENTIFIER") == 0) {
		return;
	}
	else
		error();

}

void success () {
	lookahead = getNextToken();
	if (strcmp(strType[lookahead.type],"EOFF") == 0)
		printf ("SUCCESS\n");
	else
		printf("ERROR\n");
}

void error() {
	printf("Error\n");
	exit(0);
}

int main () {
	beginn();
	//lookahead = getNextToken();
	procE();
	success();
}