#include "../lex.h"

token lookahead;
#define isID strcmp(strType[lookahead.type],"IDENTIFIER") == 0
#define isNUM strcmp(strType[lookahead.type],"NUMBER") == 0

void retract() {
	int len = (-1) * strlen(lookahead.lex);
	fseek(fd,len,SEEK_CUR);
}

/*
E 	     -> A | List
A    	 -> id | number
List 	 -> (Lexp_Seq)
Lexp_Seq -> E L'
L' 		 -> E L' | $
*/

void procE(); 			//lexp
void procA(); 			//aterm
void procList(); 		//list
void procLexp_Seq();	//lexp_seq
void procL1(); 
void success();
void error();

//E -> A | List
void procE() {
	lookahead = getNextToken();
	printf ("Lexp %s\n",lookahead.lex);
	if (isNUM || isID)	{
		retract();
		procA();		
	}
	else if(strcmp(lookahead.lex,"(") == 0) {
		retract();
		procList();
	}
	else 
		error();
}

//A -> id | number
void procA() {
	lookahead = getNextToken();
	printf ("Aterm %s\n",lookahead.lex);
	if (isID || isNUM) {
		return;
	}
	else 
		error();
}


//List -> (Lexp_Seq)
void procList() {
	lookahead = getNextToken();
	printf ("List %s\n",lookahead.lex);
	if(strcmp(lookahead.lex,"(") == 0) {		
		procLexp_Seq();
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

//Lexp_Seq -> E L'
void procLexp_Seq() {
	printf ("Lexp_Seq %s\n",lookahead.lex);
	procE();
	procL1();
}

//L' -> E L' | $
void procL1() {
	printf ("L1 %s\n",lookahead.lex);
	lookahead = getNextToken();
	if (isID || isNUM || strcmp(lookahead.lex,"(") == 0) {
		retract();
		procE();
		procL1();
	}
	else
		retract();
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
	procE();
	success();
}