/*
- lexeme.c 
- by Colton Andreson
- Jan 25, 2019
- 
*/	

#include "lexeme.h"

struct lexeme
{
	char *type;
	char *typeName;
	int lineNo;
	char *string;
	char *varName;
	int integer;
	double real;
	char character;

	Lexeme *(*fval)(Lexeme *);
	Lexeme **aval;
	int alen;

	FILE *fp;

	Lexeme *left;
	Lexeme *right;
};

//environment functions
Lexeme *
cons(char *t, Lexeme *l, Lexeme * r)
{

	Lexeme *temp = (Lexeme *)malloc(sizeof(Lexeme));

	assert(temp != 0);

	temp->type = t;

	temp->typeName = t;
	temp->lineNo = 0;
	temp->string = "";
	temp->varName = "";
	temp->integer = 0;
	temp->real = 0.0;

	temp->fval = 0;
	temp->aval = 0;
	temp->alen = 0;

	temp->fp = 0;

	temp->left = l;
	temp->right = r;
	
	return temp;
}

Lexeme *
car(Lexeme *lex) 
{	
	return lex->left;
}

Lexeme *
setcar (Lexeme *lex, Lexeme *left)
{
	lex->left = left;
	return left;
}	

Lexeme *
cdr(Lexeme *lex) 
{
	return lex->right;
}

Lexeme *
setcdr (Lexeme *lex, Lexeme *right)
{
	lex->right = right;
	return right;
}


//lexeme funcitons
// this is used for keyword and symbols
Lexeme *
newLexeme(char *t, char *tn, int ln)
{
	Lexeme *l = (Lexeme *)malloc(sizeof(Lexeme));

	assert(l != 0);

	l->type = t;
	l->typeName = tn;
	l->lineNo = ln;
	l->string = "";
	l->varName = "";
	l->integer = 0;
	l->real = 0.0;

	l->fval = 0;

	if (t == ARRAY) {
		l->aval = malloc(sizeof(struct Lexeme *) * ln);
		l->alen = ln;
	}
	else {
		l->aval = 0;
		l->alen = 0;
	}
	
	l->left = 0;
	l->right = 0;

	return l;
}

Lexeme *
newStringLexeme(char *t, int ln, char *s)
{
	Lexeme *l = (Lexeme *)malloc(sizeof(Lexeme));

	assert(l != 0);

	l->type = t;
	l->typeName = "STRING";
	l->lineNo = ln;
	l->string = s;
	l->varName = "";
	l->integer = 0;
	l->real = 0.0;

	l->fval = 0;
	l->aval = 0;
	l->alen = 0;

	l->fp = 0;

	l->left = 0;
	l->right = 0;
		
	return l;
}

Lexeme *
newVarLexeme(char *t, int ln, char *s)
{
	Lexeme *l = (Lexeme *)malloc(sizeof(Lexeme));

	assert(l != 0);

	l->type = t;
	l->typeName = "VARIABLE";
	l->lineNo = ln;
	l->string = s;
	l->varName = s;
	l->integer = 0;
	l->real = 0.0;

	l->fval = 0;
	l->aval = 0;
	l->alen = 0;

	l->fp = 0;

	l->left = 0;
	l->right = 0;
		
	return l;
}

Lexeme *
newIntegerLexeme(char *t, int ln, int i)
{
	Lexeme *l = (Lexeme *)malloc(sizeof(Lexeme));

	assert(l != 0);

	l->type = t;
	l->typeName = "INTEGER";
	l->lineNo = ln;
	l->string = "integer";
	l->varName = "";
	l->integer = i;
	l->real = 0.0;

	l->fval = 0;
	l->aval = 0;
	l->alen = 0;

	l->fp = 0;

	l->left = 0;
	l->right = 0;
		
	return l;
}

Lexeme *
newRealLexeme(char *t, int ln, double r)
{
	Lexeme *l = (Lexeme *)malloc(sizeof(Lexeme));

	assert(l != 0);

	l->type = t;
	l->typeName = "REAL";
	l->lineNo = ln;
	l->string = "real";
	l->varName = "";
	l->integer = 0;
	l->real = r;

	l->fval = 0;
	l->aval = 0;
	l->alen = 0;

	l->fp = 0;

	l->left = 0;
	l->right = 0;
		
	return l;
}


Lexeme *
newBadLexeme(char *t, int ln, char s)
{
	Lexeme *l = (Lexeme *)malloc(sizeof(Lexeme));

	assert(l != 0);

	l->type = t;
	l->typeName = "BAD";
	l->lineNo = ln;
	l->string = "";
	l->integer = 0;
	l->real = 0.0;
	l->character = s;

	l->fval = 0;
	l->aval = 0;
	l->alen = 0;

	l->fp = 0;

	l->left = 0;
	l->right = 0;
	return l;
}

Lexeme *
newUnknownLexeme(char *t, int ln, char s)
{
	Lexeme *l = (Lexeme *)malloc(sizeof(Lexeme));

	assert(l != 0);

	l->type = t;
	l->typeName = "UNKNOWN";
	l->lineNo = ln;
	l->string = "";
	l->integer = 0;
	l->real = 0.0;
	l->character = s;

	l->fval = 0;
	l->aval = 0;
	l->alen = 0;

	l->fp = 0;

	l->left = 0;
	l->right = 0;
	return l;
}


void
display(Lexeme *l, FILE *fp, int flag) 
{	
	if (flag == 1) 
		fprintf(fp, "%d : %s",l->lineNo, l->typeName);
	else
		fprintf(fp, "%s", l->typeName);

	if(l->type == STRING) {
		fprintf(fp, " %s", l->string);
	}
	else if(l->type == VARIABLE) {
		fprintf(fp, " %s", l->varName);
	}
	else if(l->type == INTEGER) {
		fprintf(fp, " %d", l->integer);
	}
	else if(l->type == REAL) {
		fprintf(fp, " %f", l->real);
	}
	else if(l->type == BAD) {
		fprintf(fp, " %c", l->character);
	}
	else if(l->type == UNKNOWN) {
		fprintf(fp, " %c", l->character);
	}

	fprintf(fp, "\n");
}

char *
getLexemeType(Lexeme *l)
{
	return l->type;
}

char *
getLexemeTypeName(Lexeme *l)
{
	return l->typeName;
}

int
getLexemeLineNo(Lexeme *l)
{
	return l->lineNo;
}

char *
getLexemeVarName(Lexeme *l)
{
	return l->varName;
}

char *
getLexemeString(Lexeme *l)
{
	return l->string;
}

int
getLexemeInt(Lexeme *l)
{
	return l->integer;
}

double
getLexemeReal(Lexeme *l)
{
	return l->real;
}

char
getLexemeErrChar(Lexeme *l)
{
	return l->character;
}

void setfval(Lexeme *l, Lexeme *(*fval)(Lexeme *)){
	l->fval = fval;
}

Lexeme *callfval(Lexeme *l, Lexeme *args) {
	return l->fval(args);
}

void setFP(Lexeme *l, FILE*f) {
	l->fp = f;
}

FILE *getFP(Lexeme *l) {
	return l->fp;
}

Lexeme **setAval(Lexeme *l, Lexeme **aval) {
	Lexeme **temp = l->aval;
	l->aval = aval;
	return temp;
}

int setAlen(Lexeme *l, int alen) {
	int temp = l->alen;
	l->alen = alen;
	return temp;
}

Lexeme **getAval(Lexeme *l) {
	return l->aval;
}

int getAlen(Lexeme *l) {
	return l->alen;
}

Lexeme *getAvalAtIdx(Lexeme *l, int index) {
	if (index >= 0 && index < l->alen) {
		return l->aval[index];
	}
	return newIntegerLexeme(INTEGER, 0, -1);
}	

Lexeme *setAvalAtIdx(Lexeme *l, int index, Lexeme *input) {
	if (index >= 0 && index < l->alen) {
		l->aval[index] = input;
		return getAvalAtIdx(l, index);
	}
	return newIntegerLexeme(INTEGER, 0, -1);
}	