/*
- pretty.h 
- by Colton Andreson
- Jan 25, 2019
- 
*/	

#ifndef __PRETTY_INCLUDED__
#define __PRETTY_INCLUDED__

#include "parser.h"

extern void pp(Lexeme *);
extern void pDefine(Lexeme *tree);
extern void pFunction(Lexeme *);
extern void pFblock(Lexeme *tree);
extern void pLambda(Lexeme *tree);
extern void pLblock(Lexeme *tree);
extern void pList(Lexeme *tree);
extern void pObject(Lexeme *tree);
extern void pObject(Lexeme *tree);
extern void pLib(Lexeme *tree);
//extern void pFor(Lexeme *tree);
extern void pWhile(Lexeme *tree);
extern void pIf(Lexeme *tree);
extern void pElse(Lexeme *tree);
extern void pStatements(Lexeme *tree);

#endif