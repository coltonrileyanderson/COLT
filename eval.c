/**************************************************************************
 * eval.c
 * Author: Colton Anderson
 * 2/22/2019
 * ************************************************************************/
#include "eval.h"

char *fileName = "";

int cmdc;
char **cmdv;


/**************************************************************************
 * 
 * MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN
 * 
 * ************************************************************************/

int main(int argc, char **argv)
{
	if (argc == 1) Fatal("%d arguments! Use the command \"scanner <f>\", where <f> is the file name\n", argc-1);

	cmdc = argc;
	cmdv = argv;

	fileName = argv[1];
	FILE *fp = fopen(fileName, "r");

	Lexeme *tree;
	tree = parse(fp);

	Lexeme *env;
	env = create();

	insertFunctions(env);
	
	env = eval(tree, env);

	Lexeme *args = 0;
	Lexeme *call = cons(CALL, newVarLexeme(VARIABLE, 0, MAIN), args);

	eval(call, env);
	
	fclose(fp);
	return 0;
}

void insertFunctions(Lexeme *env) {
    //open
    Lexeme *open = newVarLexeme(VARIABLE, 0, OPEN);
	Lexeme *openB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(openB, evalOpen);
	insert(open, openB, env);
    //close
    Lexeme *close = newVarLexeme(VARIABLE, 0, CLOSE);
	Lexeme *closeB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(closeB, evalClose);
	insert(close, closeB, env);
    //readint
    Lexeme *readint = newVarLexeme(VARIABLE, 0, READINT);
	Lexeme *readintB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(readintB, evalReadInt);
	insert(readint, readintB, env);
    //atend
    Lexeme *atend = newVarLexeme(VARIABLE, 0, ATEND);
	Lexeme *atendB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(atendB, evalAtend);
	insert(atend, atendB, env);
	//print
	Lexeme *print = newVarLexeme(VARIABLE, 0, PRINT);
	Lexeme *printB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(printB, evalPrint);
	insert(print, printB, env);
	//println
	Lexeme *println = newVarLexeme(VARIABLE, 0, PRINTLN);
	Lexeme *printlnB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(printlnB, evalPrintln);
	insert(println, printlnB, env);
    //strlen
	Lexeme *strlen = newVarLexeme(VARIABLE, 0, STRLEN);
	Lexeme *strlenB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(strlenB, evalStrLen);
	insert(strlen, strlenB, env);
	//cmdc
	Lexeme *cmdc = newVarLexeme(VARIABLE, 0, CMDC);
	Lexeme *cmdcB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(cmdcB, evalCmdc);
	insert(cmdc, cmdcB, env);
    //cmd
	Lexeme *cmd = newVarLexeme(VARIABLE, 0, CMD);
	Lexeme *cmdB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(cmdB, evalCmd);
	insert(cmd, cmdB, env);
    //array
	Lexeme *array = newVarLexeme(VARIABLE, 0, ARRAY);
	Lexeme *arrayB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(arrayB, evalArray);
	insert(array, arrayB, env);
	//get
	Lexeme *get = newVarLexeme(VARIABLE, 0, GET);
	Lexeme *getB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(getB, evalGet);
	insert(get, getB, env);
	//set
	Lexeme *set = newVarLexeme(VARIABLE, 0, SET);
	Lexeme *setB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(setB, evalSet);
	insert(set, setB, env);
    //len
	Lexeme *len = newVarLexeme(VARIABLE, 0, LEN);
	Lexeme *lenB = newLexeme(BUILTIN, "BUILTIN", 0);
	setfval(lenB, evalLen);
	insert(len, lenB, env);
}

/**************************************************************************
 * 
 * EVAL EVAL EVAL EVAL EVAL EVAL EVAL EVAL EVAL EVAL EVAL EVAL EVAL EVAL
 * 
 * ************************************************************************/

Lexeme *eval(Lexeme *tree, Lexeme *env) {
   if (tree != NULL) {
		if (getLexemeType(tree) == PROGRAM) return evalProgram(tree, env);
		if (getLexemeType(tree) == FUNCTION) return evalFunctionDef(tree, env);
		else if(getLexemeType(tree) == OBJECT) return evalObject(tree, env);
		else if(getLexemeType(tree) == DEFINE) return evalDefine(tree, env);
		else if (getLexemeType(tree) == LIB) return evalLib(tree, env);
		else if (getLexemeType(tree) == STATEMENT) return evalStatements(tree, env);

		// TYPES
		else if(getLexemeType(tree) == VARIABLE) return lookup(tree, env);
		else if(getLexemeType(tree) == INTEGER) return tree;
		else if(getLexemeType(tree) == STRING) return tree;
		else if(getLexemeType(tree) == REAL) return tree;
        else if(getLexemeType(tree) == VOID) return tree;

		// BUILT INS
        else if (getLexemeType(tree) == BUILTIN) return evalBuiltIn(tree, env);

		// FUNCTION, OBJECTS, LOOPS, IFS, LISTS
		else if(getLexemeType(tree) == CALL) return evalCall(tree, env);
		else if (getLexemeType(tree) == LAMBDA) return evalLambda(tree, env);
		else if(getLexemeType(tree) == IF) return evalIf(tree, env);
		else if(getLexemeType(tree) == ELSE) return evalElse(tree, env);
		else if(getLexemeType(tree) == WHILE) return evalWhile(tree, env);

		// OPERATORS
		else if(getLexemeType(tree) == ASSIGN) return evalAssign(tree, env);
        else if (getLexemeType(tree) == DOT) return evalDot(tree, env);
		else if (isSimpleOp(tree)) return evalSimpleOp(tree, env);
        else if (getLexemeType(tree) == OR) return evalOr(tree, env);
    }
    return NULL;
}


/**************************************************************************
 * 
 * PROGRAM DIRECTIVES  PROGRAM DIRECTIVES  PROGRAM DIRECTIVES
 * 
 * ************************************************************************/

Lexeme *evalProgram(Lexeme *tree, Lexeme *env) {
    eval(car(tree), env);
    eval(cdr(tree), env);
    return env;
}

Lexeme *evalStatements(Lexeme *tree, Lexeme *env) {
    Lexeme *result;
    while (tree != NULL) {
        if (tree != NULL && getLexemeType(tree) == STATEMENT) {
            Lexeme *statement = car(tree);

            result = eval(statement, env);

            tree = cdr(tree);
        }
    }
    return result;
}

Lexeme *evalFunctionDef(Lexeme *tree, Lexeme *env) {
    Lexeme *closure = cons(CLOSURE, env, tree);
    Lexeme *fName = car(tree);

    return insert(fName, closure, env);
}


Lexeme *evalObject(Lexeme *tree, Lexeme *env) {
    Lexeme *closure = cons(OCLOSURE, env, tree);
    return insert(car(tree), closure, env);
}


Lexeme *evalObjConstructor(Lexeme *tree, Lexeme *args) {
    Lexeme *senv = car(tree); // env
    Lexeme *obj = cdr(tree);
    Lexeme *name = car(obj);
    Lexeme *block = cdr(obj);

    Lexeme *lenv = extend(0,0, senv);

    eval(block, lenv);

    //maybe put this in call
    insert(lenv, newVarLexeme(VARIABLE, 0, THIS), lenv);

    Lexeme *call = cons(CALL, newVarLexeme(VARIABLE, 0, getLexemeVarName(name)), args);

    eval(call, lenv);

    return lenv;
}

Lexeme *evalDefine(Lexeme *tree, Lexeme *env) {
    Lexeme *var = car(tree);
    Lexeme *val = eval(cdr(tree) ,env);
    return insert(var, val, env);
}


Lexeme *evalLib(Lexeme *tree, Lexeme *env) {
    Lexeme *name = car(tree);
    char *s = getLexemeString(name);
    FILE *fp = fopen(s, "r");

    Lexeme *parsed = parse(fp);

    eval(parsed, env);
    fclose(fp);

    return env;
}


/**************************************************************************
 * 
 * BUILT-INS BUILT-INS  BUILT-INS BUILT-INS BUILT-INS BUILT-INS
 * 
 * ************************************************************************/

Lexeme *evalBuiltIn(Lexeme *tree, Lexeme *env) {
    return callfval(tree, env);
}

Lexeme *evalOpen(Lexeme *args) {
    Lexeme *fname = car(args);

    Lexeme *fp = newLexeme(FP,"FP",0);
    FILE *file = fopen(getLexemeString(fname), "r");
    if (file == NULL)
        return newIntegerLexeme(INTEGER, 0, -1);

    setFP(fp, file);

    return fp;
}


Lexeme *evalClose(Lexeme *args) {
    FILE *fp = getFP(car(args));
    fclose(fp);
    return newIntegerLexeme(INTEGER, 0, 1); // gotta return something!!!!!!
}


Lexeme *evalReadInt(Lexeme *args) {
    FILE *fp = getFP(car(args));
    int x;
    fscanf(fp, "%d", &x);
    return newIntegerLexeme(INTEGER, 0, x);
}

Lexeme *evalAtend(Lexeme *args) {
    FILE *fp = getFP(car(args));

    if(feof(fp)) {
        return newIntegerLexeme(INTEGER, 0, 1);
    }
    return newIntegerLexeme(INTEGER, 0, 0);
}


Lexeme *evalPrint(Lexeme *args) {
    while (args != 0) {
        if (getLexemeType(car(args)) == INTEGER) {
            printf("%d", getLexemeInt(car(args)));
        }
        else if (getLexemeType(car(args)) == REAL) {
            printf("%f", getLexemeReal(car(args)));
        }
        else if (getLexemeType(car(args)) == STRING) {
            char *str = getLexemeString(car(args));
            printf("%s", str);
        }
        else if (getLexemeType(car(args)) == VOID) {
            printf("(void)");
        }

    args = cdr(args);
    }
    return 0;
}


Lexeme *evalPrintln(Lexeme *args) {
    while (args != 0) {
        if (getLexemeType(car(args)) == INTEGER) {
            printf("%d", getLexemeInt(car(args)));
        }
        else if (getLexemeType(car(args)) == REAL) {
            printf("%f", getLexemeReal(car(args)));
        }
        else if (getLexemeType(car(args)) == STRING) {
            char *str = getLexemeString(car(args));
            printf("%s", str);
        }
        else if (getLexemeType(car(args)) == VOID) {
            printf("(void)");
        }
    args = cdr(args);
    }
    printf("\n");
    return 0;
}

Lexeme *evalStrLen(Lexeme *args) {
    if (args != 0) {
        if (getLexemeType(car(args)) == STRING) {
            char *str = getLexemeString(car(args));
            int len = strlen(str);
            return newIntegerLexeme(INTEGER, 0, len);
        }
    }
    return NULL;
}

//cmdc() return integer lexeme with the arg count
Lexeme *evalCmdc(Lexeme *args) {
    if (args == 0 || args != 0)
        return newIntegerLexeme(INTEGER, 0, cmdc);
    return NULL;
}

//cmd(index) return sting at index in argv
Lexeme *evalCmd(Lexeme *args) {
    if (args != 0) {
        Lexeme *idx = car(args);
        return newStringLexeme(STRING, 0, cmdv[getLexemeInt(idx)]);
    }
    return NULL;
}

Lexeme *evalArray(Lexeme *args) {
    if (args != 0 && cdr(args) == 0) {
        Lexeme *lexsize = car(args);
        if (getLexemeType(lexsize) == INTEGER) {
            Lexeme *arr = newLexeme(ARRAY, "ARRAY", getLexemeInt(lexsize));
            if (arr != 0) {
                return arr;
            }
        }
    }
    return NULL;
}

Lexeme *evalGet(Lexeme *args) {
    Lexeme *arr = NULL;
    Lexeme *rargs = NULL;
    Lexeme *idx = NULL;
    Lexeme *val = NULL;
    int i = 0;
    if (args != 0) {
        arr = car(args);
        rargs = cdr(args);
        idx = car(rargs);
        i = getLexemeInt(idx);
        val = getAvalAtIdx(arr, i);
        return val;
    }
    return NULL;
}

Lexeme *evalSet(Lexeme *args) {
    Lexeme *arr = NULL;
    Lexeme *rargs = NULL;
    Lexeme *idx = NULL;
    Lexeme *rrargs = NULL;
    Lexeme *input = NULL;

    int i = 0;
    if (args != 0) {
        arr = car(args);
        rargs = cdr(args);
        idx = car(rargs);
        rrargs = cdr(rargs);
        input = car(rrargs);
        i = getLexemeInt(idx);
        setAvalAtIdx(arr, i, input);
        
        return input;
    }
    return NULL;
}

Lexeme *evalLen(Lexeme *args) {
    Lexeme *arr = NULL;
    int len = 0;
    if (args != 0) {
        arr = car(args);
        len = getAlen(arr);
        return newIntegerLexeme(INTEGER, 0, len);
    }
    return NULL;
}

/**************************************************************************
 * 
 * CALLS AND BLOCKS  CALLS AND BLOCKS  CALLS AND BLOCKS  CALLS AND BLOCKS
 * 
 * ************************************************************************/


Lexeme *evalCall(Lexeme *tree, Lexeme *env) {
/* structure of a call 
            CALL
    func_name   opt_args
*/   
    Lexeme *func_name = car(tree);
    Lexeme *closure = eval(func_name, env);
    Lexeme *opt_args = cdr(tree);

    //check to see if call object
    if (getLexemeType(closure) == OCLOSURE) {
        return evalObjConstructor(closure, opt_args);
    }
    Lexeme *args = evalArgs(opt_args, env);
    if (getLexemeType(closure) == BUILTIN) {
        return evalBuiltIn(closure, args);
    }

    Lexeme *senv = car(closure);
    Lexeme *function = cdr(closure); 
    Lexeme *fdef = cdr(function);
    Lexeme *params = car(fdef);
    Lexeme *block = cdr(fdef);
    Lexeme *lenv = extend(params, args, senv);

    return eval(block, lenv);
}


Lexeme *evalArgs(Lexeme *tree, Lexeme *env) {
    if (tree == NULL) {
        return NULL;
    }
    else {
        Lexeme *expr = car(tree);
        Lexeme *args = cdr(tree);
        return cons(JUNK, eval(expr, env), evalArgs(args, env));
    }
}

Lexeme *evalLambda(Lexeme *tree, Lexeme *env) {
    Lexeme *closure = cons(CLOSURE, env, tree);
    return closure;
}


Lexeme *evalIf(Lexeme *tree, Lexeme *env) {
    Lexeme *then = car(tree);
    Lexeme *optelse = cdr(tree);

    Lexeme *expr = car(then);
    Lexeme *block = cdr(then);

    Lexeme *eexpr = eval(expr, env);
    if (getLexemeType(eexpr) == INTEGER) {
        if (getLexemeInt(eexpr) == 1) {
            if(block != NULL) {
                if (getLexemeType(block) != END) {
                    return eval(block, env);
                }
                else {
                    return eval(cdr(block), env);
                }
            }
        }
        else {
            if (optelse != NULL) {
                return evalElse(optelse, env);
            }
            
        }
    }

    return NULL;
}


Lexeme *evalElse(Lexeme *tree, Lexeme *env) {
    Lexeme *ifs = car(tree);
    Lexeme *elses = cdr(tree);

    if (ifs != NULL) {
        return evalIf(ifs, env);
    }
    if (elses != NULL) {
        if (getLexemeType(elses) == END) {
            return eval(cdr(elses), env);
        }
    }

    return NULL;
}


Lexeme *evalWhile(Lexeme *tree, Lexeme *env) {
    Lexeme *expr = car(tree);
    Lexeme *eexpr = eval(expr, env);

    Lexeme *eblock = NULL;
    if (cdr(tree) != NULL) {
        while (getLexemeInt(eexpr) == 1) {
            eblock = eval(cdr(tree), env);
            eexpr = eval(expr, env);
        }
    }
    return eblock;
}


/**************************************************************************
 * 
 * OPERATORS OPERATORS OPERATORS OPERATORS OPERATORS OPERATORS OPERATORS
 * 
 * ************************************************************************/


Lexeme *evalAssign(Lexeme *tree, Lexeme *env) {
    Lexeme *leq = car(tree);
    Lexeme *req = cdr(tree);
    Lexeme *result = eval(req, env);

    if (getLexemeType(leq) == VARIABLE) {
        update(leq, result, env);
    }
    else if(getLexemeType(leq) == DOT) {
        Lexeme *ldot = car(leq);
        Lexeme *rdot = cdr(leq);

        Lexeme *obj = eval(ldot, env);
        update(rdot, result, obj);
    }
    else {
        Fatal("ERROR: Bad Assignment\n");
    }
    return result;
}


Lexeme *evalDot(Lexeme *tree, Lexeme *env) {
    Lexeme *ldot = car(tree);
    Lexeme *rdot = cdr(tree);
    Lexeme *obj = eval(ldot, env);

    return eval(rdot, obj);
}

/* ************************************************************************
 * SIMPLE OPERATORS  SIMPLE OPERATORS  SIMPLE OPERATORS  SIMPLE OPERATORS
 * ************************************************************************/

int isSimpleOp(Lexeme *tree) {
    if (getLexemeType(tree) == PLUS || getLexemeType(tree) == TIMES || 
    getLexemeType(tree) == MINUS || getLexemeType(tree) == DIVIDE || 
    getLexemeType(tree) == MODULUS || getLexemeType(tree) == PLUSEQUALS || 
    getLexemeType(tree) == MINUSEQUALS || getLexemeType(tree) == PLUSPLUS || 
    getLexemeType(tree) == MINUSMINUS || //next are comparisons
    getLexemeType(tree) == GREATERTHAN || getLexemeType(tree) == GREATERTHANEQAULTO || getLexemeType(tree) == LESSTHAN || 
    getLexemeType(tree) == LESSTHANEQAULTO || getLexemeType(tree) == EQUALSEQUALS || 
    getLexemeType(tree) == NOTEQUAL ||  getLexemeType(tree) == BANG || 
    getLexemeType(tree) == AND || getLexemeType(tree) == OR) {
        return 1;
    }
    else return 0;
}


Lexeme *evalSimpleOp(Lexeme *tree, Lexeme *env) {
    if (getLexemeType(tree) == PLUS) return evalPlus(tree, env); 
    else if (getLexemeType(tree) == TIMES) return evalTimes(tree, env);  
    else if (getLexemeType(tree) == MINUS) return evalMinus(tree, env); 
    else if (getLexemeType(tree) == DIVIDE) return evalDivide(tree, env);
    else if (getLexemeType(tree) == MODULUS) return evalModulus(tree, env); 
    else if (getLexemeType(tree) == GREATERTHAN) return evalGreater(tree, env);  
    else if (getLexemeType(tree) == GREATERTHANEQAULTO) return evalGreaterAssign(tree, env);  
    else if (getLexemeType(tree) == LESSTHAN) return evalLess(tree, env);  
    else if (getLexemeType(tree) == LESSTHANEQAULTO) return evalLessAssign(tree, env); 
    else if (getLexemeType(tree) == EQUALSEQUALS) return evalEqualsEquals(tree, env);  
    else if (getLexemeType(tree) == PLUSEQUALS) return evalPlusEquals(tree, env);  
    else if (getLexemeType(tree) == MINUSEQUALS) return evalMinusEquals(tree, env);  
    else if (getLexemeType(tree) == NOTEQUAL) return evalNotEqual(tree, env);    
    else if (getLexemeType(tree) == PLUSPLUS) return evalPlusPlus(tree, env);
    else if (getLexemeType(tree) == MINUSMINUS) return evalMinusMinus(tree, env);
    else if (getLexemeType(tree) == BANG) return evalBang(tree, env);  
    else if (getLexemeType(tree) == AND) return evalAnd(tree, env);
    else if (getLexemeType(tree) == OR) return evalOr(tree, env);

    return NULL;
}


Lexeme *evalPlusEquals(Lexeme *tree, Lexeme *env) {
    Lexeme *left = car(tree);
    Lexeme *right = cdr(tree);

    Lexeme *test = cons(ASSIGN, left, cons(PLUS, left, right));
    return eval(test, env);
}


Lexeme *evalMinusEquals(Lexeme *tree, Lexeme *env) {
    Lexeme *left = car(tree);
    Lexeme *right = cdr(tree);

    Lexeme *test = cons(ASSIGN, left, cons(MINUS, left, right));
    return eval(test, env);
}


Lexeme *evalPlus(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);

    //left and right exist
    if(left != 0 && right != 0) {
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            int sum = getLexemeInt(left) + getLexemeInt(right);
            return newIntegerLexeme(INTEGER, 0, sum);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            double sum = getLexemeInt(left) + getLexemeReal(right);
            return newRealLexeme(REAL, 0, sum);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            double sum = getLexemeReal(left) + getLexemeInt(right);
            return newRealLexeme(REAL, 0, sum);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            double sum = getLexemeReal(left) + getLexemeReal(right);
            return newRealLexeme(REAL, 0, sum);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalTimes(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);
    
    //left and right exist
    if(left != 0 && right != 0) {
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            int product = getLexemeInt(left) * getLexemeInt(right);
            return newIntegerLexeme(INTEGER, 0, product);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            double product = getLexemeInt(left) * getLexemeReal(right);
            return newRealLexeme(REAL, 0, product);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            double product = getLexemeReal(left) * getLexemeInt(right);
            return newRealLexeme(REAL, 0, product);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            double product = getLexemeReal(left) * getLexemeReal(right);
            return newRealLexeme(REAL, 0, product);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalMinus(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);

    if (left != 0 && right != 0){ // when left == 0, talking about a negation
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            int difference = getLexemeInt(left) - getLexemeInt(right);
            return newIntegerLexeme(INTEGER, 0, difference);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            double difference = getLexemeInt(left) - getLexemeReal(right);
            return newRealLexeme(REAL, 0, difference);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            double difference = getLexemeReal(left) - getLexemeInt(right);
            return newRealLexeme(REAL, 0, difference);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            double difference = getLexemeReal(left) - getLexemeReal(right);
            return newRealLexeme(REAL, 0, difference);
        }
    } else if (left == 0 && right != 0) { //negation
        if (getLexemeType(right) == INTEGER) {
            int difference = 0 - getLexemeInt(right);
            return newIntegerLexeme(INTEGER, 0, difference);
        } else if (getLexemeType(right) == REAL) {
            double difference = 0 - getLexemeReal(right);
            return newRealLexeme(REAL, 0, difference);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }

    return 0;
}


Lexeme *evalDivide(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);

    //left and right exist
    if(left != 0 && right != 0) {
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            int divide = getLexemeInt(left) / getLexemeInt(right);
            return newIntegerLexeme(INTEGER, 0, divide);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            double divide = getLexemeInt(left) / getLexemeReal(right);
            return newRealLexeme(REAL, 0, divide);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            double divide = getLexemeReal(left) / (double)getLexemeInt(right);
            return newRealLexeme(REAL, 0, divide);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            double divide = getLexemeReal(left) / getLexemeReal(right);
            return newRealLexeme(REAL, 0, divide);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalModulus(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);

    //left and right exist
    if ((left != 0 && right != 0) && (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER)) {
            int modulus = getLexemeInt(left) % getLexemeInt(right);
            return newIntegerLexeme(INTEGER, 0, modulus);
    }
    else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalPlusPlus(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);
    
    if (left == 0 && right != 0) { 
        if (getLexemeType(right) == INTEGER) {
            int plusplus = getLexemeInt(right) + 1;
            Lexeme *new = newIntegerLexeme(INTEGER, 0, plusplus);
            return update(cdr(tree), new, env);
        } else if (getLexemeType(right) == REAL) {
            double plusplus = getLexemeReal(right) + 1;
            Lexeme *new = newRealLexeme(REAL, 0, plusplus);
            return update(cdr(tree), new, env);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalMinusMinus(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);

    if (left == 0 && right != 0) { 
        if (getLexemeType(right) == INTEGER) {
            int minusminus = getLexemeInt(right) - 1;
            Lexeme *new = newIntegerLexeme(INTEGER, 0, minusminus);
            return update(cdr(tree), new, env);
        } else if (getLexemeType(right) == REAL) {
            double minusminus = getLexemeReal(right) - 1;
            Lexeme *new = newRealLexeme(REAL, 0, minusminus);
            return update(cdr(tree), new, env);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


/* ************************************************************************
 * COMPARISON OPERATORS  COMPARISON OPERATORS  COMPARISON OPERATORS
 * ************************************************************************/

Lexeme *evalGreater(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);

    //left and right exist
    if(left != 0 && right != 0) {
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            if (getLexemeInt(left) > getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            if (getLexemeInt(left) > getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            if (getLexemeReal(left) > getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            if (getLexemeReal(left) > getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalGreaterAssign(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);
    
    //left and right exist
    if(left != 0 && right != 0) {
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            if (getLexemeInt(left) >= getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            if (getLexemeInt(left) >= getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            if (getLexemeReal(left) >= getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            if (getLexemeReal(left) >= getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalLess(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);
    
    //left and right exist
    if(left != 0 && right != 0) {
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            if (getLexemeInt(left) < getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            if (getLexemeInt(left) < getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            if (getLexemeReal(left) < getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            if (getLexemeReal(left) < getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalLessAssign(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);
    
    //left and right exist
    if(left != 0 && right != 0) {
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            if (getLexemeInt(left) <= getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            if (getLexemeInt(left) <= getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            if (getLexemeReal(left) <= getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            if (getLexemeReal(left) <= getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalEqualsEquals(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);
    
    //left and right exist
    if(left != 0 && right != 0) {
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            if (getLexemeInt(left) == getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            if (getLexemeInt(left) == getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            if (getLexemeReal(left) == getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            if (getLexemeReal(left) == getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == STRING && getLexemeType(right) == STRING){
            int check = strcmp(getLexemeString(left), getLexemeString(right));
            
            if (check < 0 || check > 0) {
                return newIntegerLexeme(INTEGER, 0, 0);
            }
            else {
                return newIntegerLexeme(INTEGER, 0, 1);
            }
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalNotEqual(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);
    
    //left and right exist
    if(left != 0 && right != 0) {
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            if (getLexemeInt(left) != getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            if (getLexemeInt(left) != getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            if (getLexemeReal(left) != getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            if (getLexemeReal(left) != getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == STRING && getLexemeType(right) == STRING){
            int check = strcmp(getLexemeString(left), getLexemeString(right));
            
            if (check < 0 || check > 0) {
                return newIntegerLexeme(INTEGER, 0, 1);
            }
            else {
                return newIntegerLexeme(INTEGER, 0, 0);
            }
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalBang(Lexeme *tree, Lexeme *env) {
    Lexeme *right = cdr(tree);
    Lexeme *result = eval(right, env);

    if(getLexemeType(result) == INTEGER) {
        if(getLexemeInt(result) == 0){
            return newIntegerLexeme(INTEGER, 0, 1);
        } else {
            return newIntegerLexeme(INTEGER, 0, 0);
        }
    }
    else {
        Fatal("Could not understand not expression:\n");
        return 0;
    }
}


Lexeme *evalAnd(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);
    
    //left and right exist
    if(left != 0 && right != 0) {
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            if (getLexemeInt(left) && getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            if (getLexemeInt(left) && getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            if (getLexemeReal(left) && getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            if (getLexemeReal(left) && getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}


Lexeme *evalOr(Lexeme *tree, Lexeme *env) {
    Lexeme *left = eval(car(tree), env);
    Lexeme *right = eval(cdr(tree), env);
    
    //left and right exist
    if(left != 0 && right != 0) {
        if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER){
            if (getLexemeInt(left) || getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL){
            if (getLexemeInt(left) || getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER){
            if (getLexemeReal(left) || getLexemeInt(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL){
            if (getLexemeReal(left) || getLexemeReal(right)) {
                return newIntegerLexeme(INTEGER, 0, 1);
            } else return newIntegerLexeme(INTEGER, 0, 0);
        }
        else {
            Fatal("ERROR: Attempted an improper operation\n");
            return 0;
        }
    } else {
        Fatal("ERROR: Attempted an improper operation\n");
        return 0;
    }
}
