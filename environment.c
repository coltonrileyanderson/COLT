/*
- environment.c 
- by Colton Andreson
- Feb 10, 2019
- 
*/	
#include "environment.h"

Lexeme *create()
{
	Lexeme *rtn = extend(0,0,0);

	return rtn;
}

Lexeme *insert(Lexeme *var, Lexeme *val, Lexeme* env)
{
	Lexeme *table = car(env);

	setcar(table, cons(VAR, var, car(table)));
	setcdr(table, cons(VAL, val, cdr(table)));

	return table;

}

int same(Lexeme *var1, Lexeme *var2)
{
	if(!strcmp(getLexemeVarName(var1), getLexemeVarName(var2))) {
		return 1;
	}
	return 0;
}
Lexeme *lookup(Lexeme *var, Lexeme *env) 
{
	Lexeme *table; Lexeme *vars; Lexeme *vals;

	while (env != 0) {
		table = car(env);
		vars = car(table);
		vals = cdr(table);
		while (vars != 0){
			if (same(var, car(vars))) {
				return car(vals);
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(env);
	}

	FatalNoExit("Variable: %s is undefined", getLexemeVarName(var));
	return 0;
}

Lexeme *update(Lexeme *var, Lexeme *new, Lexeme *env) {
	Lexeme *table; Lexeme *vars; Lexeme *vals;

	while (env != 0) {
		table = car(env);
		vars = car(table);
		vals = cdr(table);
		while (vars != 0){
			if (same(var, car(vars))) {
				Lexeme * newVal = setcar(vals, new);
				return newVal;
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(env);
	}

	FatalNoExit("Variable: %s is undefined", getLexemeVarName(var));
	return 0;
}

Lexeme *extend(Lexeme *vars, Lexeme *vals, Lexeme *env) 
{
	return cons(ENV, cons(TAB, vars, vals), env);
}


void displayEnv(Lexeme *env, FILE *fp)
{
	if (env != 0) {
		fprintf(fp, "The environment is:\n");
		Lexeme *table; Lexeme *vars; Lexeme *vals;

		while (env != 0) {
			table = car(env);
			vars = car(table);
			vals = cdr(table);
			
			while(vars != 0) {
				fprintf(fp, "The var: %s has value: ", getLexemeVarName(car(vars)));
				display(car(vals), fp, 0);

				vars = cdr(vars);
				vals = cdr(vals);
			}
			env = cdr(env);
		}
	}
	else {
		FatalNoExit("The environemt doesn't exist");
	}
	return;
}

void displayLocal(Lexeme *env, FILE *fp) 
{
	if (env != 0) {
		fprintf(fp, "The local environment is:\n");
		Lexeme *table; Lexeme *vars; Lexeme *vals;

		table = car(env);
		vars = car(table);
		vals = cdr(table);
		
		while(vars != 0) {
			fprintf(fp, "The var: %s has value: ", getLexemeVarName(car(vars)));
			display(car(vals), fp, 0);

			vars = cdr(vars);
			vals = cdr(vals);
		}
	}
	else {
		FatalNoExit("The environemt doesn't exist");
	}
	return;
}

void
FatalNoExit(char *fmt, ...)
{
    va_list ap;

    fprintf(stderr,"\nERROR ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    //exit(-1);
}
