#include "../lex.h"

token lookahead;

/*
S -> aAcBe
A -> bA'
A'-> bA' | $
B -> d
*/

void procS();
void procA();
void procA1();
void procB();
void success();
void error();

void retract() {
	int len = (-1) * strlen(lookahead.lex);
	fseek(fd,len,SEEK_CUR);
}

//S -> aAcBe
void procS() {
	lookahead = getNextToken();
	printf ("S %s\n",lookahead.lex);
	if (strcmp(lookahead.lex,"a") == 0)	{
		procA();		
		lookahead = getNextToken();
		printf ("S %s\n",lookahead.lex);

		if (strcmp(lookahead.lex,"c") == 0)	{			
			procB();
			lookahead = getNextToken();
			printf ("S %s\n",lookahead.lex);

			if (strcmp(lookahead.lex,"e") == 0)
				return;
			else
				error();
		}
		else
			error();
	}
	else
		error();
}

//A -> bA'
void procA() {
	lookahead = getNextToken();
	printf ("A %s\n",lookahead.lex);
	if (strcmp(lookahead.lex,"b") == 0)		
		procA1();
	else
		error();
	
}

//A'-> bA' | $
void procA1() {
	lookahead = getNextToken();
	printf ("A1 %s\n",lookahead.lex);
	if (strcmp(lookahead.lex,"b") == 0)	
		procA1();
	else
		retract();
}

//B -> d
void procB() {
	lookahead = getNextToken();
	printf ("B %s\n",lookahead.lex);
	if (strcmp(lookahead.lex,"d") == 0)
		return;
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
	procS();
	success();
}