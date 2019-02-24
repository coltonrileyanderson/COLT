/*
- lexer.c 
- by Colton Andreson
- Jan 25, 2019
- 
*/	

#include "lexer.h"

int lineNum = 1;

struct lexer 
{
	FILE *fileName;
};

Lexer *
newLexer(FILE *fp)
{
	Lexer *l = (Lexer *)malloc(sizeof(Lexer));

	assert(l != 0);

	l->fileName = fp;

	return l;
}


//written by John C. Lusth in scanner.c
void
skipWhiteSpace(FILE *fp)
    {
    int ch;

    /* read chars until a non-whitespace character is encountered */
    ch = fgetc(fp);
    while (ch != EOF && (isspace(ch) || ch == '?'))
    {
    	if(ch == '\n') {
    		lineNum++;
    	}
    	if(ch == '?'){
    		//first ? hit, get next char to see if its single line or multiline
    		ch = fgetc(fp);
    		if (ch == '?') {
    			// if next char is ? then it is a multiline
    			// read until the next ?
    			ch = fgetc(fp);
    			while (ch != EOF)
    			{
    				if (ch == '\n'){
    					lineNum++;
    				}
    				if (ch == '?') {
    					// if a ? is hit, then check to see if the next char is ?
    					ch = fgetc(fp);
    					if (ch == '?') {
    						// if ? is the next character, break the loop and 
    						// start looking for next whitespace or char
    						break;
    					}
    					else {
    						// if next char is not ? then its not the end of the 
    						// comment, unget that character to keep adding to 
    						// the comment
    						ungetc(ch, fp);
    					}
    				}
    				// read next character in comment and re enter loop
    				ch = fgetc(fp);
    			}
    		}
    		// the next character was not a ? so its a single line comment
    		// read until the first newline is hit.
    		else {
    			ungetc(ch, fp);

    			while (ch != '\n')
    			{
    				ch = fgetc(fp);
    			}
    			lineNum++;
    		}
    	// end of if for comment checking
    	}

    	// continue checking for whitespace or more comments
        ch = fgetc(fp);
    }
 
    /* a non-space character got us out of the loop, so push it back */

    if (ch != EOF) ungetc(ch,fp);

    }

void
skipCommentSpace(FILE *fp) 
{
	char ch;
	char ch1;

	ch = fgetc(fp);

	if (ch == '*') {
		ch = fgetc(fp);
		while (ch != EOF && (ch != '*' && ch1 != '?')){

			if(ch == '\n') {
    			lineNum++;
    		}
			if (isspace(ch)) {
				continue;
			}

			ch = fgetc(fp);
			if (ch == '*') {
				ch1 = fgetc(fp);
			}
			
		}
	}
	else {
		while (ch != '\n') {

		}
	}
	
	skipWhiteSpace(fp);
}

Lexeme *
lex(Lexer *l)
{
	FILE *fp = l->fileName;

	char ch; //read in char
	Lexeme *test;

	skipWhiteSpace(fp);

	ch = fgetc(fp);

	if(ch == EOF){
		return newLexeme(ENDOFFILE, "ENDOFFILE", lineNum);
	}

	switch(ch)
	{
		case ':': return newLexeme(COLON, "COLON", lineNum);
		case '[': return newLexeme(OBRACKET, "OBRACKET", lineNum);
		case ']': return newLexeme(CBRACKET, "CBRACKET", lineNum);
		case '(': return newLexeme(OPAREN, "OPAREN", lineNum);
		case ')': return newLexeme(CPAREN, "CPAREN", lineNum);
		case '=': 
			ch = fgetc(fp);
			if (ch == '=') {
				return newLexeme(EQUALSEQUALS, "EQUALSEQUALS", lineNum);
			}
			else { //unget the character we just tested, then return = because not ==
				ungetc(ch, fp);
				return newLexeme(ASSIGN, "ASSIGN", lineNum);
			}
		case '+': 
			ch = fgetc(fp);
			if (ch == '=') {
				return newLexeme(PLUSEQUALS, "PLUSEQUALS", lineNum);
			}
			else if(ch == '+') {
				return newLexeme(PLUSPLUS, "PLUSPLUS", lineNum);
			}
			else { //unget the character we just tested, then return +
				ungetc(ch, fp);
				return newLexeme(PLUS, "PLUS", lineNum);
			}
		case '-':
			ch = fgetc(fp);
			if (ch == '=') {
				return newLexeme(MINUSEQUALS, "MINUSEQUALS", lineNum);
			}
			else if(ch == '-') {
				return newLexeme(MINUSMINUS, "MINUSMINUS", lineNum);
			}
			else { //unget the character we just tested, then return -
				ungetc(ch, fp);
				return newLexeme(MINUS, "MINUS", lineNum);
			}
		case '!':
			ch = fgetc(fp);
			if (ch == '=') {
				return newLexeme(NOTEQUAL, "NOTEQUAL", lineNum);
			}
			else { //unget the character we just tested, then return !
				ungetc(ch, fp);
				return newLexeme(BANG, "BANG", lineNum);
			}
		case '>': 
			ch = fgetc(fp);
			if (ch == '=') {
				return newLexeme(GREATERTHANEQAULTO, "GREATERTHANEQAULTO", lineNum);
			}
			else { //unget the character we just tested, then return >
				ungetc(ch, fp);
				return newLexeme(GREATERTHAN, "GREATERTHAN", lineNum);
			}
		case '<':
			ch = fgetc(fp);
			if (ch == '=') {
				return newLexeme(LESSTHANEQAULTO, "LESSTHANEQAULTO", lineNum);
			}
			else { //unget the character we just tested, then return <
				ungetc(ch, fp);
				return newLexeme(LESSTHAN, "LESSTHAN", lineNum);
			}
		case '*': return newLexeme(TIMES, "TIMES", lineNum);
		case '/': return newLexeme(DIVIDE, "DIVIDE", lineNum);
		case '.': return newLexeme(DOT, "DOT", lineNum);
		case '\"': 
			ungetc(ch, fp);
			test = lexString(fp);
			return test;
		case '%': return newLexeme(MODULUS, "MODULUS", lineNum);
		case '&': 
			ch = fgetc(fp);
			if (ch == '&') {
				return newLexeme(AND, "AND", lineNum);
			}
			else { //unget the character we just tested, then return <
				ungetc(ch, fp);
				return newLexeme(AMPERSAND, "AMPERSAND", lineNum);
			}
		case '|': 
			ch = fgetc(fp);
			if (ch == '|') {
				return newLexeme(OR, "OR", lineNum);
			}
			else { //unget the character we just tested, then return <
				ungetc(ch, fp);
				return newLexeme(BAR, "BAR", lineNum);
			}
		case ',':
			return newLexeme(COMMA, "COMMA", lineNum);
		case '\n': return newLexeme(NEWLINE, "NEWLINE", lineNum);

		default:
			if(isalpha(ch)){
				ungetc(ch, fp);
				test = lexVarOrKeyWord(fp);
				return test;
			}
			else if(isdigit(ch)) {
				ungetc(ch, fp);
				test = lexNumber(fp);
				return test;
			}
			return newUnknownLexeme(UNKNOWN, lineNum, ch);
	}
}

Lexeme *
lexVarOrKeyWord(FILE *fp) 
{
	int length = 64;
	int index = 0;
	char *buffer = malloc(sizeof(char) * length + 1);

	char ch;

	ch = fgetc(fp);

	while(isalpha(ch) || isdigit(ch))
	{	
		if(index == length) return newBadLexeme(BAD, lineNum, ch);
		buffer[index++] = ch;
		buffer[index] = '\0';
		ch = fgetc(fp);
	}
	//unget the punctuation or space
	ungetc(ch, fp);
	
	// buffer holds the variable or keywoard

	if (!strcmp(buffer, DEFINE)) 
		return newLexeme(DEFINE, "DEFINE", lineNum);
	else if (!strcmp(buffer, OBJECT)) 
		return newLexeme(OBJECT, "OBJECT", lineNum);
	else if (!strcmp(buffer, FUNCTION)) 
		return newLexeme(FUNCTION, "FUNCTION", lineNum);
	else if (!strcmp(buffer, AS)) 
		return newLexeme(AS, "AS", lineNum);
	else if (!strcmp(buffer, END)) 
		return newLexeme(END, "END", lineNum);
	else if (!strcmp(buffer, IF)) 
		return newLexeme(IF, "IF", lineNum);
	else if (!strcmp(buffer, ELSE)) 
		return newLexeme(ELSE, "ELSE", lineNum);
	else if (!strcmp(buffer, THEN)) 
		return newLexeme(THEN, "THEN", lineNum);
/*	else if (!strcmp(buffer, FOR)) 
		return newLexeme(FOR, "FOR", lineNum);
*/	else if (!strcmp(buffer, DO)) 
		return newLexeme(DO, "DO", lineNum);
	else if (!strcmp(buffer, WHILE)) 
		return newLexeme(WHILE, "WHILE", lineNum);
/*	else if (!strcmp(buffer, RETURN)) 
		return newLexeme(RETURN, "RETURN", lineNum);
	else if (!strcmp(buffer, OPEN)) 
		return newLexeme(OPEN, "OPEN", lineNum);
	else if (!strcmp(buffer, CLOSE)) 
		return newLexeme(CLOSE, "CLOSE", lineNum);
	else if (!strcmp(buffer, READ)) 
		return newLexeme(READ, "READ", lineNum);
	else if (!strcmp(buffer, PRINT)) 
		return newLexeme(PRINT, "PRINT", lineNum);
	else if (!strcmp(buffer, PRINTLN)) 
		return newLexeme(PRINTLN, "PRINTLN", lineNum);
	else if (!strcmp(buffer, CMD)) 
		return newLexeme(CMD, "CMD", lineNum);
	else if (!strcmp(buffer, ARR)) 
		return newLexeme(ARR, "ARR", lineNum);
	else if (!strcmp(buffer, GET)) 
		return newLexeme(GET, "GET", lineNum);
	else if (!strcmp(buffer, SET)) 
		return newLexeme(SET, "SET", lineNum);
	else if (!strcmp(buffer, LEN)) 
		return newLexeme(LEN, "LEN", lineNum);
*/	else if (!strcmp(buffer, VOID)) 
		return newLexeme(VOID, "VOID", lineNum);
	else 
		return newVarLexeme(VARIABLE, lineNum, buffer);
}

Lexeme *
lexNumber(FILE *fp) 
{
	int length = 64;
	int index = 0;
	char *buffer = malloc(sizeof(char) * length + 1);

	char ch;
	ch = fgetc(fp);

	int flag = 0;

	while((isdigit(ch) || ch == '.') && flag < 2) 
	{
		if(ch == '.') {
			flag++;
		}

		if (index == length) {
			buffer = realloc(buffer, sizeof(char) * length * 2 + 1);
			length += 2;
		}

		buffer[index++] = ch;
		buffer[index] = '\0';
		ch = fgetc(fp);
	}

	ungetc(ch, fp);

	if (flag == 0) {
		int rtn = atoi(buffer);
		return newIntegerLexeme(INTEGER, lineNum, rtn);
	}
	else if(flag == 1) {
		double rtn = strtod(buffer, 0);
		return newRealLexeme(REAL, lineNum, rtn);
	}
	else {
		return newBadLexeme(BAD, lineNum, ch);
	}
	

}

Lexeme *
lexString(FILE *fp)
{
	int length = 64;
	int index = 0;
	char *buffer = malloc(sizeof(char) * length + 1);

	char ch;
	ch = fgetc(fp);

	int flag = 0;

	while(!feof(fp) && flag != 2)
	{
		if(ch == '\"') {
			flag++;
		}

		if (index == length) {
			buffer = realloc(buffer, sizeof(char) * length * 2 + 1);
			length *= 2;
		}

		buffer[index++] = ch;
		buffer[index] = '\0';
		ch = fgetc(fp);
	}

	ungetc(ch, fp);	
	char *buff = removeQuotes(buffer);
	return newStringLexeme(STRING, lineNum, buff);
}
char *removeQuotes(char *string) {
    char *buff = (char *)malloc(strlen(string) - 2 + 1);
    int i = 0;

    while (i < (int)strlen(string) - 2) {
        buff[i] = string[i+1];
        i++;
    }
    buff[i] = 0;
    return buff;
}