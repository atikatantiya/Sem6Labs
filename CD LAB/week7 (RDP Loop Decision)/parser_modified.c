#include "lex.h"

// Program -> main() { declarations statement-list }
// data-type -> int | char
// declarations -> data-type identifier-list; declarations | (epsilon)
// identifier-list -> id identifier-list'
// identifier-list' -> , identifier-list | [number] identifier-list'' | (epsilon)
// identifier-list'' -> (epsilon) | , identifier-list
// statement-list -> statement statement-list | (epsilon)
// statement -> assign_stat; | decision_stat | loop_stat
// assign_stat -> id = expr
// expr -> simple_expr eprime
// eprime -> relop simple_expr | (epsilon)
// simple_expr -> term seprime
// seprime -> addop term seprime | (epsilon)
// term -> factor tprime
// tprime -> mulop factor tprime | (epsilon)
// factor -> id | num
// relop -> == | != | <= | >= | > | <
// addop -> + | -
// mulop -> * | / | %

//decision_stat -> if (expr) { statement-list } dprime
//dprime -> else { statement-list } | (epsilon)
//loop_stat -> while (expr) { statement-list } | for (assign_stat; expr; assign_stat) { statement-list } 

token lookahead;

void program(); 		//done
void declarations(); 	//done
void datatype(); 		//done
void identifierList(); 	//done
void idListPrime();		//done
void idListPrime2(); 	//done
void statementList(); 	//done
void statement(); 		//done
void assignStat(); 		//done
void decisionStat();	//done
void loopStat();		//done
void dprime();			//done
void expr(); 			//done
void eprime(); 			//done
void simpleExpr(); 		//done
void seprime(); 		//done
void term(); 			//done
void tprime(); 			//done
void factor(); 			//done
void addop(); 			//done
void mulop(); 			//done
void relop(); 			//done

void retract() {
	int len = (-1) * strlen(lookahead.lex);
	fseek(fd,len,SEEK_CUR);
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

// Program -> main() { declarations statement-list }
void program() {
	printf("Program with %s\n",lookahead.lex);
	if(strcmp(lookahead.lex,"main") == 0) {
		lookahead = getNextToken();
		printf("Program with %s\n",lookahead.lex);
		if(strcmp(lookahead.lex,"(") == 0) {
			lookahead = getNextToken();
			printf("Program with %s\n",lookahead.lex);
			if(strcmp(lookahead.lex,")") == 0) {
				lookahead = getNextToken();
				printf("Program with %s\n",lookahead.lex);
				if(strcmp(lookahead.lex,"{") == 0) {
					declarations();
					statementList();
					lookahead = getNextToken();
					printf("Program with %s\n",lookahead.lex);
					if(strcmp(lookahead.lex,"}") == 0) {
						return;
					}
					else
						error();
				}
				else
					error();
			}
			else
				error();
		}
		else
			error();
	}
	else
		error();
}

// declarations -> data-type identifier-list; declarations | (epsilon)
void declarations() {
	lookahead = getNextToken();
	printf("Declarations with %s\n",lookahead.lex);
	if(strcmp(lookahead.lex,"int") == 0 || strcmp(lookahead.lex,"char") == 0) {
		retract();
		datatype();
		identifierList();
		lookahead = getNextToken();
		printf("Declarations with %s\n",lookahead.lex);
		if(strcmp(lookahead.lex,";") == 0) {
			declarations();
		}
		else
			error();
	}
	else
		retract();
}

// data-type -> int | char
void datatype() {
	lookahead = getNextToken();
	printf("Datatype with %s\n",lookahead.lex);
	if(strcmp(lookahead.lex,"int") == 0 || strcmp(lookahead.lex,"char") == 0)
		return;
	else
		error();
}

// statement-list -> statement statement-list | (epsilon)
void statementList() {
	lookahead = getNextToken();
	printf("StatementList with %s\n",lookahead.lex);
	if(strcmp(strType[lookahead.type],"IDENTIFIER") == 0 || strcmp(lookahead.lex,"if") == 0 || strcmp(lookahead.lex,"while") == 0 || strcmp(lookahead.lex,"for") == 0) {
		retract();
		statement();
		statementList();
	}
	else
		retract();
}

// identifier-list -> id identifier-list'
void identifierList() {
	lookahead = getNextToken();
	printf("IdentifierList with %s\n",lookahead.lex);
	if(strcmp(strType[lookahead.type],"IDENTIFIER") == 0) {
		idListPrime();
	}
	else
		error();
}

// identifier-list' -> , identifier-list | [number] identifier-list'' | (epsilon)
void idListPrime() {
	lookahead = getNextToken();
	printf("idListPrime with %s\n",lookahead.lex);
	if(strcmp(lookahead.lex,",") == 0) {
		identifierList();
	}
	else if (strcmp(lookahead.lex,"[") == 0) {
		lookahead = getNextToken();
		printf("idListPrime with %s\n",lookahead.lex);
		if (strcmp(strType[lookahead.type],"NUMBER") == 0) {
			lookahead = getNextToken();
			printf("idListPrime with %s\n",lookahead.lex);
			if (strcmp(lookahead.lex,"]") == 0) {
				idListPrime2();
			}
			else
				error();
		}
		else
			error();		
	}
	else
		retract();
}

// identifier-list'' -> (epsilon) | , identifier-list
void idListPrime2() {
	lookahead = getNextToken();
	printf("idListPrime2 with %s\n",lookahead.lex);
	if(strcmp(lookahead.lex,",") == 0) {
		identifierList();
	}
	else
		retract();
}

// statement -> assign_stat; | decision_stat | loop_stat
void statement() {
	lookahead = getNextToken();
	if(strcmp(strType[lookahead.type],"IDENTIFIER") == 0) {
		retract();
		assignStat();
		lookahead = getNextToken();
		if(strcmp(lookahead.lex,";") == 0) {
			return;
		}
		else
			error();
	}
	else if(strcmp(lookahead.lex,"if") == 0) {
		retract();
		decisionStat();
	}
	else if(strcmp(lookahead.lex,"while") == 0 || strcmp(lookahead.lex,"for") == 0) {
		retract();
		loopStat();
	}
	else
		error();
}

// assign_stat -> id = expr
void assignStat() {
	lookahead = getNextToken();
	printf("assignStat with %s\n",lookahead.lex);
	if(strcmp(strType[lookahead.type],"IDENTIFIER") == 0) {
		lookahead = getNextToken();
		printf("assignStat with %s\n",lookahead.lex);
		if(strcmp(lookahead.lex,"=") == 0) {
			expr();
		}
		else
			error();
	}
	else
		error();
}

//decision_stat -> if (expr) { statement-list } dprime
void decisionStat() {
	lookahead = getNextToken();
	if(strcmp(lookahead.lex,"if") == 0) {
		lookahead = getNextToken();
		if(strcmp(lookahead.lex,"(") == 0) {
			expr();
			lookahead = getNextToken();
			if(strcmp(lookahead.lex,")") == 0) {
				lookahead = getNextToken();
				if(strcmp(lookahead.lex,"{") == 0) {
					statementList();
					lookahead = getNextToken();
					if(strcmp(lookahead.lex,"}") == 0) {
						dprime();
					}
					else
						error();
				}
				else
					error();
			}
			else
				error();
		}
		else
			error();
	}
	else
		error();
}

//dprime -> else { statement-list } | (epsilon)
void dprime() {
	lookahead = getNextToken();
	if(strcmp(lookahead.lex,"else") == 0) {
		lookahead = getNextToken();
		if(strcmp(lookahead.lex,"{") == 0) {
			statementList();
			lookahead = getNextToken();
			if(strcmp(lookahead.lex,"}") == 0) {
				return;
			}
			else
				error();
		}
		else
			error();
	}
	else
		retract();
}

//loop_stat -> while (expr) { statement-list } | for (assign_stat; expr; assign_stat) { statement-list } 
void loopStat() {
	lookahead = getNextToken();
	if(strcmp(lookahead.lex,"while") == 0) {
		lookahead = getNextToken();
		if(strcmp(lookahead.lex,"(") == 0) {
			expr();
			lookahead = getNextToken();
			if(strcmp(lookahead.lex,")") == 0) {
				lookahead = getNextToken();
				if(strcmp(lookahead.lex,"{") == 0) {
					statementList();
					lookahead = getNextToken();
					if(strcmp(lookahead.lex,"}") == 0) {
						return;
					}
					else
						error();
				}
				else
					error();
			}
			else
				error();
		}
		else
			error();
	}
	else if(strcmp(lookahead.lex,"for") == 0) {
		lookahead = getNextToken();
		if(strcmp(lookahead.lex,"(") == 0) {
			assignStat();
			lookahead = getNextToken();
			if(strcmp(lookahead.lex,";") == 0) {
				expr();
				lookahead = getNextToken();
				if(strcmp(lookahead.lex,";") == 0) {
					assignStat();
					lookahead = getNextToken();
					if(strcmp(lookahead.lex,")") == 0) {
						lookahead = getNextToken();
						if(strcmp(lookahead.lex,"{") == 0) {
							statementList();
							lookahead = getNextToken();
							if(strcmp(lookahead.lex,"}") == 0) {
								return;
							}
							else
								error();
						}
						else
							error();
					}
					else
						error();
				}
				else
					error();
			}
			else
				error();
		}
		else
			error();
	}
	else
		error();
}

// expr -> simple_expr eprime
void expr() {
	printf("expr with %s\n",lookahead.lex);
	simpleExpr();
	eprime();
}

// simple_expr -> term seprime
void simpleExpr() {
	printf("simpleExpr with %s\n",lookahead.lex);
	term();
	seprime();
}

// eprime -> relop simple_expr | (epsilon)
void eprime() {
	lookahead = getNextToken();
	printf("eprime with %s\n",lookahead.lex);
	if(lookahead.lex[0] == '=' || lookahead.lex[0] == '!' || lookahead.lex[0] == '>' || lookahead.lex[0] == '<') {
		retract();
		relop();
		simpleExpr();
	}
	else
		retract();
}

// term -> factor tprime
void term() {
	printf("term with %s\n",lookahead.lex);
	factor();
	tprime();
}

// seprime -> addop term seprime | (epsilon)
void seprime() {
	lookahead = getNextToken();
	printf("seprime with %s\n",lookahead.lex);
	if(strcmp(lookahead.lex,"+") == 0 || strcmp(lookahead.lex,"-") == 0) {
		retract();
		addop();
		term();
		seprime();
	}
	else
		retract();
}

// relop -> == | != | <= | >= | > | <
void relop() {
	lookahead = getNextToken();
	printf("relop with %s\n",lookahead.lex);
	if(strcmp(strType[lookahead.type],"RELATIONAL_OPERATOR") == 0)
		return;
	else
		error();

}

// factor -> id | num
void factor() {
	lookahead = getNextToken();
	printf("factor with %s\n",lookahead.lex);
	if(strcmp(strType[lookahead.type],"IDENTIFIER") == 0 || strcmp(strType[lookahead.type],"NUMBER") == 0)
		return;
	else
		error();
}

// tprime -> mulop factor tprime | (epsilon)
void tprime() {
	lookahead = getNextToken();
	printf("tprime with %s\n",lookahead.lex);
	if(strcmp(lookahead.lex,"*") == 0 || strcmp(lookahead.lex,"/") == 0 || strcmp(lookahead.lex,"%") == 0) {
		retract();
		mulop();
		factor();
		tprime();
	}
	else
		retract();
}

// addop -> + | -
void addop() {
	lookahead = getNextToken();
	printf("addop with %s\n",lookahead.lex);
	if(strcmp(lookahead.lex,"+") == 0 || strcmp(lookahead.lex,"-") == 0) {
		return;
	}
	else
		error();
}

// mulop -> * | / | %
void mulop() {
	lookahead = getNextToken();
	printf("mulop with %s\n",lookahead.lex);
	if(strcmp(lookahead.lex,"*") == 0 || strcmp(lookahead.lex,"/") == 0 || strcmp(lookahead.lex,"%") == 0) {
		return;
	}
	else
		error();
}

int main() {
	beginn();
	lookahead = getNextToken();
	program();
	success();
}
