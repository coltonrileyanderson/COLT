/*
- lexeme.h 
- by Colton Andreson
- Jan 25, 2019
- 
*/	

#ifndef __LEXEME_INCLUDED__
#define __LEXEME_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "types.h"

typedef struct lexeme Lexeme;

extern Lexeme *newLexeme(char *t, char *tn, int ln);
extern Lexeme *newStringLexeme(char *t, int ln, char *s);
extern Lexeme *newVarLexeme(char *t, int ln, char *s);
extern Lexeme *newIntegerLexeme(char *t, int ln, int i);
extern Lexeme *newRealLexeme(char *t, int ln, double r);

extern Lexeme *newBadLexeme(char *t, int ln, char s);
extern Lexeme *newUnknownLexeme(char *t, int ln, char s);

extern void display(Lexeme *l, FILE *fp, int flag);

extern char *getLexemeTypeName(Lexeme *l);
extern char *getLexemeVarName(Lexeme *l);
extern char *getLexemeType(Lexeme *l);
extern int getLexemeLineNo(Lexeme *l);
extern char *getLexemeString(Lexeme *l);
extern int getLexemeInt(Lexeme *l);
extern double getLexemeReal(Lexeme *l);
extern char getLexemeErrChar(Lexeme *l);

//environments
extern Lexeme *cons(char *t, Lexeme *l, Lexeme * r);
extern Lexeme *car(Lexeme *lex);
extern Lexeme *cdr(Lexeme *lex);
extern Lexeme *setcar(Lexeme *lex, Lexeme *left);
extern Lexeme *setcdr(Lexeme *, Lexeme *right);

extern void setfval(Lexeme *l, Lexeme *(*fval)(Lexeme *));
extern Lexeme *callfval(Lexeme *l, Lexeme *args);

extern void setFP(Lexeme *l, FILE*f);
extern FILE *getFP(Lexeme *l);

extern Lexeme **setAval(Lexeme *l, Lexeme **aval);
extern int setAlen(Lexeme *l, int alen);
extern Lexeme **getAval(Lexeme *l);
extern int getAlen(Lexeme *l);
extern Lexeme *getAvalAtIdx(Lexeme *l, int index);	
extern Lexeme *setAvalAtIdx(Lexeme *l, int index, Lexeme *input);
#endif