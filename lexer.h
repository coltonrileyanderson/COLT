/*
- lexer.h
- by Colton Andreson
- Jan 25, 2019
- 
*/	
#ifndef __LEXER_INCLUDED__
#define __LEXER_INCLUDED__

#include "lexeme.h"

#include <ctype.h>
#include <string.h>

typedef struct lexer Lexer;

extern Lexer *newLexer(FILE *fp);
extern Lexeme *lex(Lexer *l);
extern void skipWhiteSpace(FILE *fp);
extern Lexeme *lexString(FILE *fp);
extern Lexeme *lexVarOrKeyWord(FILE *fp);
extern Lexeme *lexNumber(FILE *fp);
extern char *removeQuotes(char *string);
#endif