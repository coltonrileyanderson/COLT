/*
- parser.h 
- by Colton Andreson
- Feb 1, 2019
- 
*/	

#ifndef __PARSER_INCLUDED__
#define __PARSER_INCLUDED__

#include "lexer.h"
#include "environment.h"

#include <stdarg.h>

extern int main(int argc, char **argv);
extern void Fatal(char *fmt, ...);


// manipulators
extern Lexeme *parse(FILE *fp);
extern void lexInit(FILE *fp);
extern void legal();
extern void illegal();


// helpers
extern Lexeme *match(char *type);
extern int check(char *type);
extern Lexeme *advance();


//non-terminals
extern Lexeme *program(); // done
extern Lexeme *def();
extern Lexeme *fdef(); // done
extern Lexeme *fBlock(); // done
extern Lexeme *lambda();
extern Lexeme *varDef(); 
extern Lexeme *objDef();
extern Lexeme *objBlock();
extern Lexeme *libDef();
extern Lexeme *expr(); // done
extern Lexeme *expr2(); // done
extern Lexeme *expr3(); // done
extern Lexeme *unary(); // done
extern Lexeme *operator(); // done
//extern Lexeme *list();
//extern Lexeme *optList();
//extern Lexeme *varList();
//extern Lexeme *optVarList();
extern Lexeme *exprList(); // done
extern Lexeme *optExprList(); // done
extern Lexeme *list(); // done
extern Lexeme *optlist(); // done
extern Lexeme *statements(); // done
extern Lexeme *statement(); // done
extern Lexeme *ifStatement(); 
extern Lexeme *ifBlock();
extern Lexeme *elseStatement();
extern Lexeme *whileStatement();
extern Lexeme *whileBlock();

//pending
extern int programPending();
extern int defPending();
extern int fDefPending();
extern int varDefPending();
extern int objPending();
extern int libPending();
extern int lambdaPending();
extern int builtinPending(Lexeme *v);
extern int unaryPending();
extern int exprPending();
extern int expr2Pending();
extern int expr3Pending();
extern int varPending();
extern int intPending();
extern int realPending();
extern int strPending();
extern int varIntRealStrPending();
extern int opPending();
extern int statementPending();
extern int ifPending();
extern int elsePending();
extern int forPending();
extern int whilePending();


#endif