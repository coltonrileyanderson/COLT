/*
- parser.c 
- by Colton Andreson
- Jan 25, 2019
- 
*/	
#include "parser.h"

//global current lexeme
Lexeme *curr;

//global lexer
Lexer *lexer;

Lexeme *
parse(FILE *fp) {
	lexInit(fp);
	curr = lex(lexer);
	Lexeme *prog;
	prog = program();

	match(ENDOFFILE);
	return prog;
}


void
lexInit(FILE *fp) {
	lexer = newLexer(fp);
}

void
legal() {
	fprintf(stdout, "legal\n");
}

void
illegal() {
	fprintf(stdout, "illegal\n");
	exit(0);
}


//************************************************
//helpers

Lexeme *
match(char *type) {
	int i = check(type);

	if (i != 0) {	
		return advance();
	}
	else {
		Fatal("Syntax error at line %d: Expected %s, Got: %s\n", getLexemeLineNo(curr), type, getLexemeType(curr));
	}
	return 0;
}

int
check(char *type) {
	if (getLexemeType(curr) == type) {
		return 1;
	}
	else {
		return 0;
	}
}

Lexeme *
advance() {
	
	Lexeme *prev = curr;

	//uncomment for testing
	//printf("CURR: ");
	//display(curr, stdout, 1);

	curr = lex(lexer);
	//printf("NEXT: ");
	//display(curr, stdout, 1);

	return prev;
}


//************************************************
//non-terminals

Lexeme *program() {
	Lexeme *prog = NULL;
	Lexeme *d;
	d = def();
	if (programPending()) {
		prog = program();
	}

	return cons(PROGRAM, d, prog);
}

Lexeme *
def() {
	if (defPending()) {
		advance();
		if(fDefPending()) {
			return fdef();
		}
		else if(varDefPending()) {
			return varDef();
		}
		else if(objPending()) {
			return objDef();
		}
		else if(libPending()) {
			return libDef();
		}
		else {
			Fatal("Syntax error at line %d: Incomplete Definition Exiting...\n\n", getLexemeLineNo(curr));
		}
	}
	return NULL;
}

Lexeme *fdef() {
	Lexeme *v;
	Lexeme *ovl = NULL;
	Lexeme *b = NULL;
	match(FUNCTION);
	v = match(VARIABLE);
	match(OPAREN);
	ovl = optlist();

	match(CPAREN);

	//function block checker
	b = fBlock();

	return cons(FUNCTION, v, cons(FDEF, ovl, b));
}

Lexeme *fBlock() {

	//check if theres a semi colon or a function block
	Lexeme *ss = NULL;

	match(AS);

	if (statementPending()) {
		ss = statements();
	}

	match(END);

	if (ss != NULL) {
		return cons(getLexemeType(ss), car(ss), cdr(ss));; // return to program
	}
	else return NULL;
}

Lexeme *lambda() {
	match(FUNCTION);
	match(OPAREN);
	Lexeme *ol = optlist();
	match(CPAREN);
	Lexeme *b = fBlock();

	return cons(FDEF, ol, b);

}
Lexeme *varDef() {
	Lexeme *u;
	Lexeme *e;
	u = unary();

	if (check(ASSIGN)) {
		advance();
	
		e = expr();

		return cons(DEFINE, u, e);
	}
	return cons(DEFINE, u, NULL);
}

Lexeme *objDef() {
	Lexeme *v, *b;
	match(OBJECT);
	v = match(VARIABLE);

	b = objBlock();
	return cons(OBJECT, v, b);
}

Lexeme *objBlock() {
	Lexeme *ss = NULL;

	match(AS);

	if (statementPending()) {
		ss = statements();
	}

	match(END);

	if (ss != NULL) {
		return cons(getLexemeType(ss), car(ss), cdr(ss));; // return to program
	}
	else return NULL;
	
}

Lexeme *libDef() {
	Lexeme *s;
	s = match(STRING);
	return cons(LIB, s, NULL);
}

// assign expressions
Lexeme *expr() {
	Lexeme *e;
	e = expr2();
	while(check(ASSIGN)) {
		advance();
		Lexeme *e2 = expr2();
		e = cons(ASSIGN, e, e2);
	}
	return e;
}

// operator expressions
Lexeme *expr2() {
	Lexeme *e;
	e = expr3();
	while(opPending()) {
		Lexeme *o = operator();
		Lexeme *e3 = expr3();
		e = cons(getLexemeType(o), e, e3);
	}
	return e;
}

//dot expressions
Lexeme *expr3() {
	Lexeme *u;
	u = unary();
	while(check(DOT)) {
		advance();
		Lexeme *uu = unary();
		u = cons(DOT, u, uu);
	}
	if (cdr(u) != NULL && getLexemeType(cdr(u)) == CALL){
		Lexeme *cdru = cdr(u);
		return cons(CALL, cons(DOT, car(u), car(cdru)), cdr(cdru));
	}
	return u;
}

//need to add all of these to pp
Lexeme *unary() {
	Lexeme *v;
	if (check(VARIABLE)) {
		v = advance();

		//function call
		if (check(OPAREN)) {
			Lexeme *oel;
			advance();
			oel = optExprList();
			match(CPAREN);
			return cons(CALL, v, oel);
		}

		else return v;
	}
	else if (check(INTEGER)) {
		v = advance();
		return v;
	}
	else if (check(REAL)) {
		v = advance();
		return v;
	}
	else if (check(STRING)) {
		v = advance();
		return v;
	}
	else if (check(VOID)) {
		v = advance();
		return v;
	}
	else if (check(OPAREN)) {
		Lexeme *e;
		match(OPAREN);
		if(exprPending()){
			e = expr();
		}
		else if (lambdaPending()) {
			Lexeme *l =  lambda();
			match(CPAREN);
			if (check(OPAREN)) {
				match(OPAREN);
				Lexeme *e = optExprList();
				match(CPAREN);
				return cons(LAMBDA, e, l);
			}
			else return cons(LAMBDA, 0, l);
			
		}
		match(CPAREN);
		return e;
	}
	
	else if(check(MINUS)) {
		advance();
		if (check(VARIABLE) || check(INTEGER) || check(REAL)){
			v = advance();
		}
		return cons(MINUS, NULL, v);
	}
	
	else if (check(MINUSMINUS)) {
		advance();
		v = match(VARIABLE);
		return cons(MINUSMINUS, NULL, v);
	}
	else if (check(PLUSPLUS)) {
		advance();
		v = match(VARIABLE);
		return cons(PLUSPLUS, NULL, v);
	}
	else if (check(BANG)) {
		advance();
		if (check(VARIABLE) || check(INTEGER) || check(REAL)){
			v = advance();
		}
		return cons(BANG, NULL, v);
	}
	return NULL;
}


// need to add all of these to pp
Lexeme *operator() {
	if (check(PLUS)) 
		return advance();
	else if (check(TIMES)) 
		return advance(); 
	else if (check(MINUS)) 
		return advance();
	else if (check(DIVIDE))
		return advance();
	else if (check(MODULUS)) 
		return advance();
	else if (check(GREATERTHAN)) 
		return advance(); 
	else if (check(GREATERTHANEQAULTO)) 
		return advance(); 
	else if (check(LESSTHAN))  
		return advance();
	else if (check(LESSTHANEQAULTO)) 
		return advance();
	else if (check(EQUALSEQUALS))  
		return advance();
	else if (check(PLUSEQUALS))  
		return advance();
	else if (check(MINUSEQUALS))  
		return advance();
	else if (check(NOTEQUAL)) 
		return advance(); 
	else if (check(BANG))  
		return advance();
	else if (check(AND))
		return advance();
	else if (check(OR)) 
		return advance();
	else if (check(AMPERSAND))
		return advance();
	else if (check(BAR)){
		return match(BAR);
	}
	else return NULL;
}

Lexeme *statements() {
	Lexeme *s = NULL;;
	Lexeme *ss = NULL;
	s = statement();
	if (statementPending()) {
		ss = statements();
	}

	if (s == NULL && ss == NULL){
		return NULL;	
	}
	return cons(STATEMENT, s, ss);
}

//finish the rest of statement after finishing unary, operator, expr in pp
Lexeme *statement() {
	if (exprPending()) {
		Lexeme *e;
		e = expr();
		return e;
	}
	else if (ifPending()) {
		return ifStatement();
	}
	else if (whilePending()) {
		return whileStatement();
	}
	else if(defPending()) {
		return def();
	}
	return NULL;
}

Lexeme *ifStatement() {
	Lexeme *e;
	Lexeme *b = NULL;
	Lexeme *ifs = NULL;
	Lexeme *elses = NULL;
	match(IF);
	if(exprPending()) {
		
		e = expr();
		match(THEN);
		b = ifBlock();

	}
	if (getLexemeType(b)  != END) {
		match(ELSE);
		if (ifPending()) {
			ifs = ifStatement();
		} 
		else {
			elses = ifBlock();
		}
		return cons(IF, cons(THEN, e, b), cons(ELSE, ifs, elses));
	}
	return cons(IF, cons(THEN, e, b), NULL);
}

Lexeme *ifBlock() {
	Lexeme *ss = NULL;
	
	ss = statements();

	if (check(END)) {
		advance();
		if (ss != NULL) {
			return cons(END, NULL, cons(getLexemeType(ss), car(ss), cdr(ss))); // return to program
		}
		else return NULL;
	}
	else return cons(getLexemeType(ss), car(ss), cdr(ss));
}

Lexeme *whileStatement(){
	Lexeme *e, *b;

	match(WHILE);

	if(exprPending()) {
		
		e = expr();
	}
	b = whileBlock();

	return cons(WHILE, e, b);
}

Lexeme *whileBlock(){
	Lexeme *ss = NULL;

	match(DO);

	if(statementPending()) {
		ss = statements();
	}
	match(END);

	if (ss != NULL) {
		return cons(getLexemeType(ss), car(ss), cdr(ss));; // return to program
	}
	else return NULL;
}

Lexeme *exprList() {
	Lexeme *e, *el;
	e = expr();
	if (check(COMMA)) {
		match(COMMA);

		if (exprPending()) {
			el = exprList();
		}
	}
	else el = NULL;

	return cons(LIST, e, el);
	
}

Lexeme *optExprList() {
	if (exprPending()) {
		return exprList();
	}
	return NULL;
}

Lexeme *list() {
	Lexeme *v, *vl;

	v = unary();
	if (check(COMMA)) {
		match(COMMA);
		if(unaryPending()) {
			vl = list();
		}	
	}
	else vl = NULL;

	return cons(LIST, v, vl);
}

Lexeme *optlist() {
	if (unaryPending()) {
		return list();
	}
	return NULL;
}

//************************************************
//pendings

int programPending() {
	return defPending();
}

int defPending() {
	return check(DEFINE);
}

int fDefPending() {
	return check(FUNCTION);
}

int varDefPending() {
	return check(VARIABLE);
}

int objPending() {
	return check(OBJECT);
}

int libPending() {
	return check(STRING);
}

int lambdaPending() {
	return check(FUNCTION);
}
int builtinPending(Lexeme *v) {
	if (getLexemeVarName(v) == OPEN || getLexemeVarName(v) == CLOSE ||
		getLexemeVarName(v) == READINT || getLexemeVarName(v) == ATEND ||
		getLexemeVarName(v) == PRINT || getLexemeVarName(v) == PRINTLN || 
		getLexemeVarName(v) == CMD || getLexemeVarName(v) == CMDC ||
		getLexemeVarName(v) == ARRAY ||getLexemeVarName(v) == GET || 
		getLexemeVarName(v) == SET || getLexemeVarName(v) == LEN)
	{
		return 1;
	}
	else return 0;
}
int unaryPending() {
	if (check(VARIABLE) || check(INTEGER) || 
		check(REAL) || check(STRING) || check(OPAREN) || 
		check(PLUSPLUS) || check(MINUSMINUS) || 
		check(VOID) || check(MINUS) ||
		check(BANG)) {
		return 1;
	}
	else return 0;
}

int exprPending() {
	return expr2Pending();
}

int expr2Pending() {
	return expr3Pending();
}

int expr3Pending() {
	return unaryPending();
}

int opPending() {
	if (check(PLUS) || 
	check(TIMES) || check(MINUS) || 
	check(DIVIDE) || check(MODULUS) || 
	check(GREATERTHAN) || check(GREATERTHANEQAULTO) || 
	check(LESSTHAN) || check(LESSTHANEQAULTO) || 
	check(EQUALSEQUALS) || check(PLUSEQUALS) || 
	check(MINUSEQUALS) || check(NOTEQUAL) || check(AND) || 
	check(OR) || check(AMPERSAND) || check(BAR)) 
			{
				return 1;
			}
	else return 0;
}

int varPending() {
	return check(VARIABLE);
}

int intPending() {
	return check(INTEGER);
}

int realPending() {
	return check(REAL);
}

int strPending() {
	return check(STRING);
}

int varIntRealStrPending() {
	if (check(VARIABLE) || check(INTEGER) || 
	check(REAL) || check(STRING)) {
		return 1;
	}
	else return 0;
}

int statementPending() {
	if (varPending() || ifPending() || 
	whilePending() || defPending() || exprPending()) {
		return 1;
	}
	else return 0;
}

int ifPending() {
	return check(IF);
}

int elsePending() {
	return check(ELSE);
}

int whilePending() {
	return check(WHILE);
}

void
Fatal(char *fmt, ...)
{
    va_list ap;

    fprintf(stderr,"ERROR: ");

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}
