#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define prev fseek(fd,-1,SEEK_CUR)

#define sschk (ca == '[' || ca == ']' || ca == '(' || ca == ')' || ca == '{' || ca == '}' || ca == ',' || ca == ';' || ca == ':' || ca == '?')
#define opchk (ca == '=' || ca == '+' || ca == '-' || ca == '*' || ca == '/' || ca == '%' || ca == '<' || ca == '>' || ca == '!' || ca == '&' || ca == '|' || ca == '^' || ca == '~' || ca == '.')

//enumeration for tokens
enum tokenType {
	KEYWORD,
	IDENTIFIER,
	CHARACTER,
	STRINGS,
	SS,
	AR_OP,
	LG_OP,
	RL_OP,
	AS_OP,
	OP,
	NUMBER,
	EOFF
};

char strType[12][100] = {
	"KEYWORD",
	"IDENTIFIER",
	"CHARACTER",
	"STRING",
	"SPECIAL_SYMBOL",
	"ARITHMETIC_OPERATOR",
	"LOGICAL_OPERATOR",
	"RELATIONAL_OPERATOR",
	"ASSIGNMENT_OPERATOR",
	"OTHER_OPERATOR",
	"NUMBER",
	"EOFF"
};

//sructure for each token
typedef struct token {
	char lex[100];
	int row,col;
	enum tokenType type;
}token;

FILE *fd;
int col = 1, row = 1;

void beginn() {
	printf("Enter name of input file: \n");
	char *filename = (char *)malloc(sizeof(char)*100);
	fgets(filename,300,stdin);
	int l = strlen(filename);
	filename[l-1] = '\0';
	fd = fopen(filename,"r");
	if(!fd) {
		printf("Error opening file\n");
		exit(0);
	}
}

token getNextToken () {
	//declare keywords list
	char arr[32][300] = { 
		"auto", 
		"break", 
		"case", 
		"char", 
		"const" , 
		"continue", 
		"default", 
		"do", 
		"double", 
		"else", 
		"enum", 
		"extern", 
		"float", 
		"for", 
		"goto", 
		"if", 
		"int", 
		"long", 
		"register", 
		"return", 
		"short", 
		"signed", 
		"sizeof", 
		"static", 
		"struct", 
		"switch", 
		"typedef", 
		"union", 
		"unsigned", 
		"void", 
		"volatile", 
		"while" 
	};

	int ca;
	int j = 0;
	token var;

	ca = getc(fd);
	col++;
	memset(var.lex,0,sizeof(var.lex));
	
	if(ca != EOF) {
		switch(ca) {
			case '\n':  row++;
					    col = 0;
						return getNextToken();
						break;
			case '"':   j = 0;
						ca = getc(fd);
						col++;
						var.row = row;
						var.col = col;
						var.type = 3;
						while(ca != '"') {
							var.lex[j++] = ca;
							ca = getc(fd);
							col++;
						}
						return var;
						break;
			case '/':	ca = getc(fd);
						col++;
						switch(ca) {
							case '/':	while(ca != '\n') {
											ca = getc(fd);
										}
										row++;
										col = 0;
										return getNextToken();
										break;
							case '*':	while(1) {
											while(ca != '*') {
												ca = getc(fd);
												col++;
												if(ca == '\n') {
													row++;
													col = 0;
												}
											}
											ca = getc(fd);
											col++;
											if(ca == '/')
												break;
										}
										return getNextToken();
										break;
							default: 	var.row = row;
										var.col = col;
										var.lex[0] = '/';
										var.type = 5;
										if(ca == '=') {			
											var.type = 8;
											var.lex[1] = ca;
										}
										else {
											prev;
											col--;
										}
										return var;
										break;
						}
						//return var;
						break;
			case '\'':  ca = getc(fd);
						col++;
						var.row = row;
						var.col = col;
						var.lex[0] = ca;
						var.type = 2;
						if(ca == '\\') {
							ca = getc(fd);
							col++;
							var.lex[1] = ca;
						}
						ca = getc(fd);
						col++;
						return var;
						break;
			case '[':	
			case ']': 	
			case '(': 	
			case ')': 	
			case '{': 	
			case '}': 	
			case ',': 	
			case ';': 	
			case ':': 	
			case '?':	var.row = row;
						var.col = col;
						var.type = 4;
						var.lex[0] = ca;
						return var;
						break;
			case '0':	
			case '1':	
			case '2':	
			case '3':	
			case '4':	
			case '5':	
			case '6':	
			case '7':	
			case '8':	
			case '9':	j = 0;
						while(!sschk && !opchk && ca != ' ' && ca != '\n' && ca != '\t' && ca != '\r') {
							var.lex[j++] = ca;
							ca = getc(fd);
							col++;
						}
						var.row = row;
						var.col = col;
						var.type = 10;
						prev;
						col--;
						return var;
						break;
			default :	//remaining are keywords,identifiers, operators
						if(isalpha(ca) || ca == '_') {
							var.col = col;
							var.row = row;
							while(!sschk && !opchk && ca != ' ' && ca != '\n' && ca != '\t' && ca != '\r') {
								var.lex[j++] = ca;
								ca = getc(fd);
								col++;
							}
							var.type = 1;
							prev;
							col--;
							for(int i = 0; i < 32; i++) {
								if(strcmp(var.lex,arr[i]) == 0) {
									var.type = 0;
									break;
								}
							}
							return var;							
						}
						else {
							var.row = row;
							var.col = col;
							var.lex[j++] = ca;
							switch(ca) {
								case '=':	
								case '!': 	ca = getc(fd);
											col++;
											var.type = 6;
											if(ca == '=') {
												var.type = 7;
												var.lex[j++] = ca;
											}
											else {
												prev;
												col--;
											}
											return var;
											break;
								case '<': 	ca = getc(fd);
											col++;
											var.type = 7;
											if(ca == '=') {
												var.type = 7;
												var.lex[j++] = ca;
											}
											else if(ca == '<') {
												var.type = 9;
												var.lex[j++] = ca;
											}
											else {
												prev;
												col--;
											}
											return var;
											break;
								case '>':	ca = getc(fd);
											col++;
											var.type = 7;
											if(ca == '=') {
												var.type = 7;
												var.lex[j++] = ca;
											}
											else if(ca == '>') {
												var.type = 9;
												var.lex[j++] = ca;
											}
											else {
												prev;
												col--;
											}
											return var;
											break;
								case '*':	
								case '%':	ca = getc(fd);
											col++;
											var.type = 5;
											if(ca == '=') {
												var.type = 8;
												var.lex[j++] = ca;
											}
											else {
												prev;
												col--;
											}
											return var;
											break;
								case '+':	ca = getc(fd);
											col++;
											var.type = 5;
											if(ca == '=') {
												var.type = 8;
												var.lex[j++] = ca;
											}
											else if(ca == '+') {
												var.type = 9;
												var.lex[j++] = ca;
											}
											else {
												prev;
												col--;
											}
											return var;
											break;
								case '-':	ca = getc(fd);
											col++;
											var.type = 5;
											if(ca == '=') {
												var.type = 8;
												var.lex[j++] = ca;
											}
											else if(ca == '-' || ca == '>') {
												var.type = 9;
												var.lex[j++] = ca;
											}
											else {
												prev;
												col--;
											}
											return var;
											break;
								case '|':	ca = getc(fd);
											col++;
											var.type = 9;
											if(ca == '!') {
												var.type = 6;
												var.lex[j++] = ca;
											}						
											else {
												prev;
												col--;
											}
											return var;
											break;
								case '&':	ca = getc(fd);
											col++;
											var.type = 9;
											if(ca == '&') {
												var.type = 6;
												var.lex[j++] = ca;
											}						
											else {
												prev;
												col--;
											}
											return var;
											break;
								case '^':
								case '~':
								case '.':	var.type = 9;
											return var;
											break;
								default: 	memset(var.lex,0,sizeof(var.lex));
											return getNextToken();
											break;
							}
							//return var;
						}
		}
	}
	else {
		// no more tokens remaining
		var.type = 11;
		return var;
	}

}