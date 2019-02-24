/*
- types.h 
- by Colton Andreson
- Jan 25, 2019
- 
*/	

#ifndef __TYPES_INCLUDED__
#define __TYPES_INCLUDED__


//pp/eval lexemes
extern char *CLOSURE;
extern char *OCLOSURE;
extern char *PROGRAM;
extern char *CALL;
extern char *LIST;
extern char *FDEF;
extern char *JUNK;
extern char *STATEMENT;
extern char *LIB;
extern char *FCLAUSE;
extern char *THIS;
extern char *BUILTIN;
extern char *MAIN;

//environment lexemes
extern char *ENV;
extern char *TAB;
extern char *VAR;
extern char *VAL;
extern char *JOIN;
extern char *VOID;

extern char *BAD;
extern char *UNKNOWN;

//keywords - fuctions/objects
extern char *DEFINE;
extern char *OBJECT;
extern char *FUNCTION;
extern char *AS;
extern char *END;

//keywords - builtin functions
extern char *FP;
extern char *OPEN;
extern char *CLOSE;
extern char *READINT;
extern char *ATEND;

//keywords - print
extern char *PRINT;
extern char *PRINTLN;
extern char *STRLEN;

//keyword - command lines
extern char *CMD;
extern char *CMDC;

//keywords - arrays
extern char *ARRAY;
extern char *NEW;
extern char *GET;
extern char *SET;
extern char *LEN;
extern char *LAMBDA;

//keywords - block statements
extern char *IF;
extern char *ELSE;
extern char *THEN;
extern char *DO;
extern char *WHILE;

//literals
extern char *VARIABLE;
extern char *INTEGER;
extern char *REAL;
extern char *STRING;

//symbols
extern char *SEMI;
extern char *COLON;
extern char *OBRACKET;
extern char *CBRACKET;
extern char *OPAREN;
extern char *CPAREN;
extern char *ASSIGN;
extern char *EQUALSEQUALS;
extern char *PLUS;
extern char *PLUSPLUS;
extern char *PLUSEQUALS;
extern char *MINUS;
extern char *MINUSMINUS;
extern char *MINUSEQUALS;
extern char *BANG;
extern char *NOTEQUAL;
extern char *GREATERTHAN;
extern char *GREATERTHANEQAULTO;
extern char *LESSTHAN;
extern char *LESSTHANEQAULTO;
extern char *TIMES;
extern char *DIVIDE;
extern char *DOT;
extern char *AND;
extern char *OR;
extern char *QUOTE;
extern char *NEWLINE;
extern char *MODULUS;
extern char *AMPERSAND;
extern char *BAR;
extern char *COMMA;
extern char *ENDOFFILE;
#endif

