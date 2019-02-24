/**************************************************************************
 * eval.h
 * Author: Colton Anderson
 * 2/22/2019
 * ************************************************************************/

#ifndef __EVAL_INCLUDED__
#define __EVAL_INCLUDED__

#include "parser.h"

extern void insertFunctions(Lexeme *);

extern int main(int, char **);

extern Lexeme *eval(Lexeme *, Lexeme *);

extern Lexeme *evalProgram(Lexeme *, Lexeme *);
extern Lexeme *evalStatements(Lexeme *, Lexeme *);
extern Lexeme *evalFunctionDef(Lexeme *, Lexeme *);
extern Lexeme *evalObject(Lexeme *, Lexeme *);
extern Lexeme *evalObjConstructor(Lexeme *, Lexeme *);
extern Lexeme *evalDefine(Lexeme *, Lexeme *);
extern Lexeme *evalLib(Lexeme *, Lexeme *);

extern Lexeme *evalBuiltIn(Lexeme *, Lexeme *);

extern Lexeme *evalOpen(Lexeme *); 
extern Lexeme *evalClose(Lexeme *);
extern Lexeme *evalReadInt(Lexeme *);
extern Lexeme *evalAtend(Lexeme *);

extern Lexeme *evalPrint(Lexeme *); 
extern Lexeme *evalPrintln(Lexeme *); 
extern Lexeme *evalStrLen(Lexeme *args);
extern Lexeme *evalCmdc(Lexeme *);
extern Lexeme *evalCmd(Lexeme *); 

extern Lexeme *evalArray(Lexeme *);
extern Lexeme *evalGet(Lexeme *);
extern Lexeme *evalSet(Lexeme *);
extern Lexeme *evalLen(Lexeme *);

extern Lexeme *evalCall(Lexeme *, Lexeme *);
extern Lexeme *evalArgs(Lexeme *, Lexeme *);
extern Lexeme *evalLambda(Lexeme *, Lexeme *);

extern Lexeme *evalIf(Lexeme *, Lexeme *);
extern Lexeme *evalElse(Lexeme *, Lexeme *);
extern Lexeme *evalWhile(Lexeme *, Lexeme *);

extern Lexeme *evalAssign(Lexeme *, Lexeme *);
extern Lexeme *evalDot(Lexeme *, Lexeme *); 

extern int isSimpleOp(Lexeme *);
extern Lexeme *evalSimpleOp(Lexeme *, Lexeme *);
extern Lexeme *evalPlus(Lexeme *, Lexeme *); 
extern Lexeme *evalTimes(Lexeme *, Lexeme *);  
extern Lexeme *evalMinus(Lexeme *, Lexeme *); 
extern Lexeme *evalDivide(Lexeme *, Lexeme *);
extern Lexeme *evalModulus(Lexeme *, Lexeme *); 
extern Lexeme *evalGreater(Lexeme *, Lexeme *);  
extern Lexeme *evalGreaterAssign(Lexeme *, Lexeme *);  
extern Lexeme *evalLess(Lexeme *, Lexeme *);  
extern Lexeme *evalLessAssign(Lexeme *, Lexeme *); 
extern Lexeme *evalEqualsEquals(Lexeme *, Lexeme *);  
extern Lexeme *evalPlusEquals(Lexeme *, Lexeme *);  
extern Lexeme *evalMinusEquals(Lexeme *, Lexeme *);  
extern Lexeme *evalNotEqual(Lexeme *, Lexeme *);  
extern Lexeme *evalPlusPlus(Lexeme *, Lexeme *);
extern Lexeme *evalMinusMinus(Lexeme *, Lexeme *);
extern Lexeme *evalBang(Lexeme *, Lexeme *);  
extern Lexeme *evalAnd(Lexeme *, Lexeme *);
extern Lexeme *evalOr(Lexeme *, Lexeme *); 
extern Lexeme *evalAmpersand(Lexeme *, Lexeme *);
extern Lexeme *evalBar(Lexeme *, Lexeme *);

#endif