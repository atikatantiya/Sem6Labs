#include "../lex.h"

token lookahead;

void success();
void error();

void retract() {
	int len = (-1) * strlen(lookahead.lex);
	fseek(fd,len,SEEK_CUR);
}

/*
S -> a | > | (T)
T -> ST'
T'-> ,ST' | $
*/

void procS();
void procT();
void procT1 ();

//S -> a | > | (T)
void procS () {
	lookahead = getNextToken();
	printf ("S %s\n", lookahead.lex);
	if(strcmp(lookahead.lex,"a") == 0 || strcmp(lookahead.lex,">") == 0) {
		return;
	}
	else if(strcmp(lookahead.lex,"(") == 0) {
		procT();
		lookahead = getNextToken();
		if(strcmp(lookahead.lex,")") == 0) {
			return;
		}
		else
			error();
	}
	else
		error();
}

//T -> ST'
void procT () {
	printf ("T %s\n", lookahead.lex);
	procS ();
	procT1();
}

//T'-> ,ST' | $
void procT1() {
	lookahead = getNextToken();
	printf ("T1 %s\n", lookahead.lex);
	if (strcmp(lookahead.lex,",") == 0) {
		procS();
		procT1();
	}
	else
		retract();
}

void success () {
	lookahead = getNextToken();
	if (strcmp(strType[lookahead.type],"EOFF") == 0)
		printf ("%s\n", "SUCCESS");
	else
		printf("ERROR\n");
}

void error () {
	printf ("ERROR\n");
	exit(1);
}

int main () {
	beginn();
	procS();
	success();
}