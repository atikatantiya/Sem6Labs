#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define prev fseek(fd,-1,SEEK_CUR); col--
#define empt memset(var.lex,0,sizeof(var.lex))

#define sschk (ca == '[' || ca == ']' || ca == '(' || ca == ')' || ca == '{' || ca == '}' || ca == ',' || ca == ';' || ca == ':' || ca == '?')
#define opchk (ca == '=' || ca == '+' || ca == '-' || ca == '*' || ca == '/' || ca == '%' || ca == '<' || ca == '>' || ca == '!' || ca == '&' || ca == '|' || ca == '^' || ca == '~' || ca == '.')

enum tokenType { DATA, IDENTIFIER, CHARACTER, STRINGS, SS, AR_OP, LG_OP, RL_OP, AS_OP, OP, NUMBER, KEYWORD };

char strType[12][20] = { "DATATYPE", "IDENTIFIER", "CHARACTER", "STRINGS", "SPECIAL", "ARITHMETIC", "LOGICAL", "RELATION", "ASSSIGNMENT", "OTHER OP", "NUMBER", "KEYWORD"};

char darr[30][30] = {"char", "double", "enum", "float", "int", "long", "short", "signed", "struct", "unsigned", "void"};

int dsize[30]={1,8,0,4,4,8,2,4,0,4,0};

char *arr[] = { "auto", "break", "case", "char", "const" , "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while" };

typedef struct token {
	char lex[100];
	int row,col;
	enum tokenType type;
}token;

typedef struct list {
	token tok;
	int index,size,argc,argList[10];
	char scope,type[30],retType[30];
}list;

list table[100];
FILE *fd;
int col = 1, row = 1;
int gIndex = 0, gScope = 0, darrlen = 11;

void prepro() { 

	FILE *fa,*fb;
	printf("Enter name of input file: ");
	char *filename = (char *)malloc(sizeof(char)*300);
	fgets(filename,300,stdin);
	int l = strlen(filename);
    filename[l-1] = '\0';
	fa = fopen(filename,"r");
	fb = fopen("output.c","w");

	int ca;
	ca = getc(fa);

	if(ca == '#') {
		while(ca != '\n' && ca != EOF)
			ca = getc(fa);
	}		
	while(ca != EOF){
		if(ca == '"') {
			putc(ca,fb);
			do {
				ca = getc(fa);
				putc(ca,fb);
			} while(ca != '"');
			ca = getc(fa);
		}
		else if(ca != '\n')	{
			putc(ca,fb);
			ca = getc(fa);
		}
		else if(ca == '\n') {
			putc(ca,fb);
			ca = getc(fa);
			if(ca == '#'){
				while(ca != '\n')
					ca = getc(fa);
			}
		}
	}
	fclose(fa);
	fclose(fb);
}

token getNextToken() {	
	int ca;
	int j = 0;

	token var; 
	ca = getc(fd);
	col++;
	empt;

	if(ca != EOF){
		if(ca == '"') { //string const
			j = 0;
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
		}
		else if(ca == '/') { //comments
			ca = getc(fd);
			col++;
			if(ca == '/') {
				while(ca != '\n')
					ca = getc(fd);
				row++;
				col = 0;
			}
			else if(ca == '*') {
				do {
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
				} while(ca != '/');
			}
			else { //divide op
				prev;
				var.row = row;
				var.lex[0] = '/';
				var.col = col;
				var.type = 5;
				if(ca == '=') {
					col++;
					ca = getc(fd);
					var.type = 8;
					var.lex[1] = ca;
				}
			}
			return var;
		}
		else if(ca == '\'') { //char const
			ca = getc(fd);
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
		}
		else if(sschk) { //Special symbol
			var.row = row;
			var.col = col;
			var.type = 4;
			var.lex[0] = ca;
			return var;
		}
		else if(isdigit(ca)) { //number const
			j = 0;
			var.row = row;
			var.col = col;
			while(!sschk && !opchk && ca != ' ' && ca != '\n' && ca != '\t') {
				var.lex[j++] = ca;
				ca = getc(fd);
				col++;
			}
			var.type = 10;			
			prev;
			return var;
		}
		else if(ca == '\n') {
			row++;
			col = 0;
			return getNextToken();
		}
		else if(isalpha(ca) || ca == '_') { //keyword,identifier
			int flag = 1;
			j = 0;
			var.col = col;
			var.row = row;
			while(!sschk && !opchk && ca != ' ' && ca != '\n' && ca != '\t' && ca != '.' && ca != '\r') {
				var.lex[j++] = ca;
				ca = getc(fd);
				col++;
			}
			prev;
			
			for(int i = 0; i < 32; i++) {
				if(strcmp(var.lex,arr[i]) == 0) {
					flag = 0;
					var.type = 11;
					for(int j = 0; j < 11; j++) {
						if(strcmp(var.lex,darr[j]) == 0)//datatype
							var.type = 0;
					}
					break;
				}
			}
			if(flag) {
				if(strcmp(var.lex,"NULL") == 0)
					var.type = 2;
				else
					var.type = 1;
			}
			return var;
		}
		else { //operators
			j = 0;
			var.row = row;
			var.col = col;
			var.lex[j++] = ca;
			switch(ca) {
				case '=': 	ca = getc(fd);
							col++;
							if(ca == '=') {
								var.type = 7;
								var.lex[j++] = ca;
							}
							else {
								var.type = 8;
								prev;		
							}
							break;
				case '!':	ca = getc(fd);
							col++;
							if(ca == '=') {
								var.type = 7;
								var.lex[j++] = ca;
							}
							else {
								var.type = 8;
								prev;								
							}
							break;
				case '<':	
				case '>':	ca = getc(fd);
							col++;
							if(ca == '=') {
								var.type = 7;
								var.lex[j++] = ca;
							}
							else if(ca == '<' || ca == '>') {
								var.type = 9;
								var.lex[j++] = ca;
							}
							else {
								var.type = 7;
								prev;
							}
							break;
				case '*':
				case '%':	ca = getc(fd);
							col++;
							if(ca == '=') {
								var.type = 8;
								var.lex[j++] = ca;
							}
							else {
								var.type = 5;
								prev;
							}
							break;
				case '+':	ca = getc(fd);
							col++;
							if(ca == '=') {
								var.type = 8;
								var.lex[j++] = ca;
							}
							else if(ca == '+') {
								var.type = 9;
								var.lex[j++] = ca;
							}
							else {
								var.type = 5;
								prev;
							}
							break;
				case '-':	ca = getc(fd);
							col++;
							if(ca == '=') {
								var.type = 8;
								var.lex[j++] = ca;
							}
							else if(ca == '-') {
								var.type = 9;
								var.lex[j++] = ca;
							}
							else if(ca == '>') {
								var.type = 9;
								var.lex[j++] = ca;
							}
							else {
								var.type = 5;
								prev;
							}
							break;
				case '&':
				case '|':	ca = getc(fd);
							col++;
							if(ca == '&' || ca == '|') {
								var.type = 6;
								var.lex[j++] = ca;
							}
							else {
								var.type = 9;
								prev;
							}
							break;
				case '^':
				case '~':
				case '.':	var.type = 9;
							break;
				default:	return getNextToken();
							break;
			}
			return var;
		}
	}
	else {
		var.row = -1;
		return var;
	}
}

int search(token t) {
	for(int i=0; i < gIndex; i++)
		if(strcmp(t.lex,table[i].tok.lex) == 0)
			return i+1;
	return 0;
}

void insert(token t,char dtype[],int arrsize,int star,int param,char rtype[]) {
	if(search(t) && !param)
		return;

	int lsize = -1;

	for(int i = 0; i < darrlen; i++) {
		if(strcmp(dtype,darr[i]) == 0)
			lsize = dsize[i];
	}

	if(arrsize > 0)
		lsize = lsize * arrsize;

	char sc = 'G';
	if(gScope > 0)
		sc = 'L';
	if(param)
		sc = ' ';

	table[gIndex].tok = t;
	table[gIndex].size = lsize;
	table[gIndex].argc = 0;
	table[gIndex].scope = sc;
	strcpy(table[gIndex].type,dtype);
	strcpy(table[gIndex].retType,rtype);

	if(star && strcmp(dtype,"FUNC") != 0) {	
		for(int i = 0; i < star; i++) 
			strcat(table[gIndex].type,"*");

		if(arrsize == 0)
			table[gIndex].size = 8;
	}

	if(strcmp(dtype,"FUNC") == 0) {
		table[gIndex].size = 0;
		for(int i = 0; i < star; i++)
			strcat(table[gIndex].retType,"*");
	}

	table[gIndex++].index = gIndex+1;
}

void display() {
	printf("ID\tName\tType\tSize\tScope\tArgc\tArgs\tRetType\n");
	for(int i = 0; i < gIndex; i++)	{
		if(strcmp(table[i].type,"FUNC") != 0) {
			printf("%d\t%s\t%s\t%d\t%c\n",table[i].index,table[i].tok.lex,table[i].type,table[i].size,table[i].scope);
		}
		else {
			char args[30] = "", temp[30];
			if(table[i].argc == 0)
				strcpy(args,"NULL");
			else {
				for(int j = 0; j < table[i].argc; j++) {
					sprintf(temp,"%d,",table[i].argList[j]);
					strcat(args,temp);
				}
				args[strlen(args)-1] = '\0';
			}
			printf("%d\t%s\t%s\t \t%c\t%d\t%s\t%s\n",table[i].index,table[i].tok.lex,table[i].type,table[i].scope,table[i].argc,args,table[i].retType);
		}
	}
}

void genTable() {
	int loc,flag,arrsize,star,ca,stsize;
	char d_type[30],p_type[30]; 					//datatype,parameter type
	token temp,temp2;

	while(1) {
		flag = 1;
		arrsize = 0;
		star = 0;
		stsize = 0;
		memset(d_type,0,sizeof(d_type));
		memset(p_type,0,sizeof(p_type));

		temp = getNextToken();

		if(strlen(temp.lex) <= 0) {
			if(temp.row==-1) 						//EOF
				return;
			continue;
		}

		if(temp.type == 0 || temp.type == 1) {		//datatype or id for user defined type
			loc = search(temp);

			if(temp.type == 1 && !loc)				//id not in table
				continue;
			
			strcpy(d_type,temp.lex);

			//we now have the datatype
			temp = getNextToken();
			while(temp.type == 5) {			//pointer *
				star++;
				temp = getNextToken();		//to get id
			}

			if(temp.type == 1 && !search(temp))	{		//id not in table, if in table ignore
				while(flag)	{							//till semicolon
					temp2 = getNextToken();

					if(strcmp(temp2.lex,"(") == 0) {	//Function
						int tcol = col;

						while(strcmp(temp2.lex,")") != 0)
							temp2 = getNextToken();

						ca = getc(fd);
						col++;

						while(ca == ' ' || ca == '\t' || ca == 13)	{ //13 is carriage return
							ca = getc(fd);
							col++;
						}

						if(ca == ';')
							break;

						if(ca == '{' || ca == '\n') {				//proper declaration
							fseek(fd,tcol-col,SEEK_CUR);			//go back to opening (
							col = tcol;
							int fid = gIndex;
							insert(temp,"FUNC",arrsize,star,0,d_type);
							temp2 = getNextToken();

							while(strcmp(temp2.lex,")") != 0) {
								star = 0;
								table[fid].argList[table[fid].argc++] = gIndex+1;

								if(strcmp(temp2.lex,"struct") == 0)
									temp2 = getNextToken();			//will be userdefined and in table
								strcpy(p_type,temp2.lex);
								temp2 = getNextToken();

								while(temp2.type == 5) {			//pointer *
									star++;
									temp2 = getNextToken();			//to get id
								}

								insert(temp2,p_type,arrsize,star,1,"");
								while(strcmp(temp2.lex,",") != 0 && strcmp(temp2.lex,")") != 0)
									temp2 = getNextToken();
								if(strcmp(temp2.lex,")") == 0)
									break;
								temp2 = getNextToken();
							}
							flag = 0;
						}
					}
					else if(temp2.type == 8) {		//assignment
						insert(temp,d_type,arrsize,star,0,"");
						while(1) {
							temp2 = getNextToken();

							if(strcmp(temp2.lex,",") == 0) {
								prev;
								break;
							}
							else if(strcmp(temp2.lex,";") == 0)	{
								flag = 0;
								break;
							}
						}
					}
					else if(strcmp(temp2.lex,",") == 0) {		//comma
						insert(temp,d_type,arrsize,star,0,"");
						arrsize = 0;							//reset
						star = 0;								//remove pointer
						temp = getNextToken();
						while(temp.type == 5) {	 				//pointer *
							star++;
							temp = getNextToken(); 				//to get id
						}
					}
					else if(strcmp(temp2.lex,"[") == 0)			//array
					{
						arrsize=1;
						while(1)
						{
							star++;
							temp2=getNextToken();
							arrsize*=atoi(temp2.lex);
							getNextToken();//]
							temp2=getNextToken();
							if(strcmp(temp2.lex,"[")==0)
								continue;
							else if(strcmp(temp2.lex,";")==0 || strcmp(temp2.lex,",")==0)
							{
								prev;
								break;
							}
							else if(strcmp(temp2.lex,"=")==0)//ignore values in initialization
							{
								temp2=getNextToken();
								if(temp2.type!=3) //not string
									while(strcmp(temp2.lex,"}")!=0) //other array type assuming enters in one line
										temp2=getNextToken();
								break;
							}
						}
					}
					else if(strcmp(temp2.lex,";")==0)//done
					{
						insert(temp,d_type,arrsize,star,0,"");
						flag=0;
					}
				}
			}
		}
		else if(strcmp(temp.lex,"{")==0)
			gScope++;
		else if(strcmp(temp.lex,"}")==0)
			gScope--;
	}
}

int main() {
	prepro();
	
	fd = fopen("output.c","r");

	genTable();
	display();
}
