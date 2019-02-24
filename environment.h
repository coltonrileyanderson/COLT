/*
- environment.c 
- by Colton Andreson
- Feb 10, 2019
- 
*/	
#ifndef __ENVIRONMENT_INCLUDED__
#define __ENVIRONMENT_INCLUDED__

#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include "lexeme.h"

extern Lexeme *create();
extern Lexeme *insert(Lexeme *var, Lexeme *val, Lexeme* env);
extern int same(Lexeme *var1, Lexeme *var2);
extern Lexeme *lookup(Lexeme *var, Lexeme *env);
extern Lexeme *update(Lexeme *var, Lexeme *new, Lexeme *env);
extern Lexeme *extend(Lexeme *vars, Lexeme *vals, Lexeme *env);

extern void displayEnv(Lexeme *env, FILE *fp);
void displayLocal(Lexeme *env, FILE *fp);
extern void FatalNoExit(char *fmt, ...);
#endif