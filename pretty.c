/**************************************************************************
 * pretty.c
 * Author: Colton Anderson
 * 2/22/2019
 * ************************************************************************/

#include "pretty.h"

char *fileName = "";

int
main(int argc, char **argv)
{
	if (argc == 1) Fatal("%d arguments! Use the command \"scanner <f>\", where <f> is the file name\n", argc-1);

	fileName = argv[argc-1];
	FILE *inFile = fopen(fileName, "r");

	Lexeme *p;
	p = parse(inFile);
	pp(p);
	fclose(inFile);
	return 0;
}

void pp(Lexeme *tree) {
	if (tree != NULL) {
		if (getLexemeType(tree) == PROGRAM) {
			pp(car(tree)); // go left down
			pp(cdr(tree)); // then go right (should be another PROGRAM or NULL);
		}
		if (getLexemeType(tree) == FUNCTION){
			pFunction(tree);
		}
		else if(getLexemeType(tree) == OBJECT) {
			pObject(tree);
		}
		else if(getLexemeType(tree) == DEFINE) {
			pDefine(tree);
		}
		else if (getLexemeType(tree) == LIB) {
			pLib(tree);
		}
		//
		// TYPES
		//
		else if(getLexemeType(tree) == VARIABLE){
			printf("%s", getLexemeVarName(tree));
		}
		else if(getLexemeType(tree) == INTEGER){
			printf("%d", getLexemeInt(tree));
		}
		else if(getLexemeType(tree) == STRING){
			printf("%s", getLexemeString(tree));
		}
		else if(getLexemeType(tree) == REAL){
			printf("%f", getLexemeReal(tree));
		}
		//
		// BUILT INS
		//
		else if (getLexemeType(tree) == OPEN){
			printf("open");
			if (cdr(tree) != NULL) {
				pp(cdr(tree));
			}
		} 
		else if (getLexemeType(tree) == CLOSE){
			printf("close");
			if (cdr(tree) != NULL) {
				pp(cdr(tree));
			}
		}
		else if (getLexemeType(tree) == READINT){
			printf("readint");
			if (cdr(tree) != NULL) {
				pp(cdr(tree));
			}
		}
		else if (getLexemeType(tree) == PRINT){
			printf("print");
			if (cdr(tree) != NULL) {
				pp(cdr(tree));
			}
		} 
		else if (getLexemeType(tree) == PRINTLN){
			printf("println");
			if (cdr(tree) != NULL) {
				pp(cdr(tree));
			}
		} 
		else if (getLexemeType(tree) == CMD){
			printf("cmd");
			if (cdr(tree) != NULL) {
				pp(cdr(tree));
			}
		}
		else if (getLexemeType(tree) == CMDC){
			printf("cmdc");
			if (cdr(tree) != NULL) {
				pp(cdr(tree));
			}
		} 
		else if (getLexemeType(tree) == ARRAY){
			printf("array");
			if (cdr(tree) != NULL) {
				pp(cdr(tree));
			}
		}
		else if (getLexemeType(tree) == GET){
			printf("get");
			if (cdr(tree) != NULL) {
				pp(cdr(tree));
			}
		}
		else if (getLexemeType(tree) == SET){
			printf("set");
			if (cdr(tree) != NULL) {
				pp(cdr(tree));
			}
		}
		else if (getLexemeType(tree) == LEN){
			printf("len");
			if (cdr(tree) != NULL) {
				pp(cdr(tree));
			}
		}
		//
		// FUNCTION, OBJECTS, LOOPS, IFS
		//
		else if(getLexemeType(tree) == CALL) {
			pp(car(tree)); // funcName
			pp(cdr(tree)); // optparamlist
		}
		else if (getLexemeType(tree) == LAMBDA) {
			pLambda(tree);
		}
		else if(getLexemeType(tree) == IF) {
			pIf(tree);
			printf("end\n");
		}
		else if(getLexemeType(tree) == ELSE) {
			pElse(tree);
		}
		else if(getLexemeType(tree) == WHILE) {
			pWhile(tree);
		}
		//
		// LISTS
		//
		else if(getLexemeType(tree) == LIST) {
			printf("(");
			pList(tree);
			printf(")");
		}
		//
		// OPERATORS
		//
		else if(getLexemeType(tree) == ASSIGN) {
			pp(car(tree));
			printf("=");
			pp(cdr(tree));
		}
		else if (getLexemeType(tree) == PLUS){
			pp(car(tree));
			printf("+");
			pp(cdr(tree));
		} 
		else if (getLexemeType(tree) == TIMES){
			pp(car(tree));
			printf("*");
			pp(cdr(tree));
		}  
		else if (getLexemeType(tree) == MINUS){
			pp(car(tree));
			printf("-");
			pp(cdr(tree));
		} 
		else if (getLexemeType(tree) == DIVIDE){
			pp(car(tree));
			printf("/");
			pp(cdr(tree));
		}
		else if (getLexemeType(tree) == MODULUS){
			pp(car(tree));
			printf("%%");
			pp(cdr(tree));
		} 
		else if (getLexemeType(tree) == GREATERTHAN){
			pp(car(tree));
			printf(">");
			pp(cdr(tree));
		}  
		else if (getLexemeType(tree) == GREATERTHANEQAULTO){
			pp(car(tree));
			printf(">=");
			pp(cdr(tree));
		}  
		else if (getLexemeType(tree) == LESSTHAN){
			pp(car(tree));
			printf("<");
			pp(cdr(tree));
		}  
		else if (getLexemeType(tree) == LESSTHANEQAULTO){
			pp(car(tree));
			printf("<=");
			pp(cdr(tree));
		} 
		else if (getLexemeType(tree) == EQUALSEQUALS){
			pp(car(tree));
			printf("==");
			pp(cdr(tree));
		}  
		else if (getLexemeType(tree) == PLUSEQUALS){
			pp(car(tree));
			printf("+=");
			pp(cdr(tree));
		}  
		else if (getLexemeType(tree) == MINUSEQUALS){
			pp(car(tree));
			printf("-=");
			pp(cdr(tree));
		}  
		else if (getLexemeType(tree) == NOTEQUAL){
			pp(car(tree));
			printf("!=");
			pp(cdr(tree));
		}  
		else if (getLexemeType(tree) == DOT){
			pp(car(tree));
			printf(".");
			pp(cdr(tree));
		}  
		else if (getLexemeType(tree) == PLUSPLUS){
			pp(car(tree));
			printf("++");
			pp(cdr(tree));
		}
		else if (getLexemeType(tree) == MINUSMINUS){
			pp(car(tree));
			printf("--");
			pp(cdr(tree));
		}
		else if (getLexemeType(tree) == BANG){
			pp(car(tree));
			printf("!");
			pp(cdr(tree));
		}  
		else if (getLexemeType(tree) == AND){
			pp(car(tree));
			printf("&&");
			pp(cdr(tree));
		}
		else if (getLexemeType(tree) == OR){
			pp(car(tree));
			printf("||");
			pp(cdr(tree));
		} 
		else if (getLexemeType(tree) == AMPERSAND){
			pp(car(tree));
			printf("&");
			pp(cdr(tree));
		}
		else if (getLexemeType(tree) == BAR){
			pp(car(tree));
			printf("|");
			pp(cdr(tree));
		}
		//
		// NULL
		//
		else if(tree == NULL) {
			printf("NOT IMPLEMENTED YET\n");
		}
	}
}
//
void pDefine(Lexeme *tree) {
	printf("def ");
	pp(car(tree));
	if (cdr(tree)) {
		printf("=");
		pp(cdr(tree));
	}
}
//
void pList(Lexeme *tree) {
	pp(car(tree));
	if (cdr(tree) != NULL) {
		printf(",");
		pList(cdr(tree));
	}
}
//
void pFunction(Lexeme *tree) {
	printf("def function ");
	Lexeme *l, *r;
	l = car(tree); // name
	r = cdr(tree); // optvarlist and block
	pp(l); //name
	printf("(");
	if (r != NULL) {
		if(car(r) != NULL) {
			pList(car(r));
		}
		printf(")");
		pFblock(cdr(r));
	}
}
//
void pFblock(Lexeme *tree) {
	if (tree != NULL) {
		return;
	}
	printf(" as\n");
	if(tree != NULL){
		pStatements(tree);
	}
	
	printf("end\n");
}
//
void pLambda(Lexeme *tree) {
	printf("(function");
	Lexeme *fdef = cdr(tree);
	pp(car(fdef));
	pLblock(cdr(fdef));
	printf(")");
	if (car(tree) != 0) {
		pp(car(tree));
	}
	
}
//
void pLblock(Lexeme *tree) {
	if (tree != NULL) {
		return;
	}
	printf(" as ");
	if(tree != NULL){
		pStatements(tree);
	}
	
	printf("end");
}
//
void pObject(Lexeme *tree) {
	printf("def object ");
	Lexeme *l, *r;
	l = car(tree); // name
	r = cdr(tree); // block/semi
	pp(l);
	printf(" as\n");
	if (r != NULL) {
		pStatements(r);
	}
	printf("end\n");
}
//
void pLib(Lexeme *tree) {
	printf("def ");
	pp(car(tree));
	printf("\n");
}

void pWhile(Lexeme *tree) {
	printf("while ");
	pp(car(tree));
	printf(" do\n");
	if (cdr(tree) != NULL){
		pStatements(cdr(tree));
	}
	printf("end\n");
}
//
void pIf(Lexeme *tree) {
	printf("if ");
	Lexeme *then = car(tree);
	Lexeme *e = car(then);
	Lexeme *b = cdr(then);
	pp(e); // expr
	printf(" then\n");
	if (b != NULL){
		if(getLexemeType(b) != END){
			pStatements(b);
		}
		else {
			pStatements(cdr(b));
		}
	}
	Lexeme *ee = cdr(tree);
	if (ee != NULL) {
		pp(ee);
	}
}
//
void pElse(Lexeme *tree) {
	printf("else\n");
	if (car(tree) != NULL){
		pIf(car(tree));
	}
	if (cdr(tree) != NULL){
		if(getLexemeType(cdr(tree)) == END) {
			Lexeme *en = cdr(tree);
			pStatements(cdr(en));
		}
		
	}
}
//
void pStatements(Lexeme *tree) {
	while (tree != NULL) {
		if(getLexemeType(tree) == STATEMENT) {
			Lexeme *t;
			t = car(tree); //left child of statement, defines what kind of statement

			pp(t);
			tree = cdr(tree); // right child of statement, should be another statement/null
		}	
	}
}