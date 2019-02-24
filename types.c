/*
- types.c 
- by Colton Andreson
- Jan 25, 2019
- 
*/	

#include "types.h"

//pp/eval lexemes
char *CLOSURE = "CLOSURE";
char *OCLOSURE = "OCLOSURE";
char *PROGRAM = "PROGRAM";
char *CALL = "CALL";
char *LIST = "LIST";
char *FDEF = "FDEF";
char *JUNK = "JUNK";
char *STATEMENT = "STATEMENT";
char *FCLAUSE = "FCLAUSE";
char *LAMBDA = "LAMBDA";
char *BUILTIN = "BUILTIN";

//environment lexemes
char *ENV = "ENV";
char *TAB = "TAB";
char *VAR = "VAR";
char *VAL = "VAL";
char *JOIN = "JOIN";

//errors
char *BAD = "bad";
char *UNKNOWN = "unknown";

//keywords - fuctions/objects
char *MAIN = "main";
char *THIS = "this";
char *DEFINE = "def";
char *OBJECT = "object";
char *FUNCTION = "function";
char *AS = "as";
char *END = "end";
char *LIB = "LIB";

//keywords - file functions
char *FP = "FP";
char *OPEN = "open";
char *CLOSE = "close";
char *READINT = "readint";
char *ATEND = "atend";

//keywords - print
char *PRINT = "print";
char *PRINTLN = "println";
char *STRLEN = "strlen";

//keyword command lines
char *CMDC = "cmdc";
char *CMD = "cmd";

//keywords - arrays
char *ARRAY = "array";
char *NEW = "new";
char *GET = "get";
char *SET = "set";
char *LEN = "len";
char *VOID = "void";

//keywords - block statements
char *IF = "if";
char *ELSE = "else";
char *THEN = "then";
char *DO = "do";
char *WHILE = "while";

//literals
char *VARIABLE = "variable";
char *INTEGER = "integer";
char *REAL = "real";
char *STRING = "string";

//symbols
char *SEMI = ";";
char *COLON = ":";
char *OBRACKET = "[";
char *CBRACKET = "]";
char *OPAREN = "(";
char *CPAREN = ")";
char *ASSIGN = "=";
char *EQUALSEQUALS = "==";
char *PLUS = "+";
char *PLUSPLUS = "++";
char *PLUSEQUALS = "+=";
char *MINUS = "-";
char *MINUSMINUS = "--";
char *MINUSEQUALS = "-=";
char *BANG = "!";
char *NOTEQUAL = "!=";
char *GREATERTHAN = ">";
char *GREATERTHANEQAULTO = ">=";
char *LESSTHAN = "<";
char *LESSTHANEQAULTO = "<=";
char *TIMES = "*";
char *DIVIDE = "/";
char *DOT = ".";
char *AND = "&&";
char *OR = "||";
char *QUOTE = "\"";
char *NEWLINE = "\n";
char *MODULUS = "%";
char *AMPERSAND = "&";
char *BAR = "|";
char *COMMA = ",";
char *ENDOFFILE = "endOfFile";