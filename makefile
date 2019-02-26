# 
# makefile 
# by Colton Andreson
# Feb 24, 2019
# 
# this is the makefile that is used to build my language executable
#
# 	commands:
#   	make 	creates the executable, each other is a test. 
#				the first rule cats the input, 
# 				the second rule executes it.
#
#		make error1
#   	make error1x
#   	make error2
#   	make error2x
#   	make error3
#   	make error3x
#   	make error4
#   	make error4x
#   	make error5
#   	make error5x
#   	make arrays
#   	make arraysx
#   	make conditionals
#   	make conditionalsx
#   	make recursion
#   	make recursionx
#   	make iteration
#   	make iterationx
#   	make functions
#   	make functionsx      
#   	make lambda      
#   	make lambdax      
#   	make objects      
#   	make objectsx
#		make problem
#		make problemx
#

OBJS = types.o lexeme.o lexer.o
PRSENV = parser.o environment.o
OOPTS = -Wall -Wextra -g -c -std=c99
LOPTS = -Wall -Wextra -g -std=c99

all : pretty eval 

pretty : $(OBJS) $(PRSENV) pretty.o
	gcc  $(LOPTS) $(OBJS) $(PRSENV) pretty.o -o pp

eval : $(OBJS) $(PRSENV) eval.o
	gcc  $(LOPTS) $(OBJS) $(PRSENV) eval.o -o colt

#OBJS
types.o : types.c types.h
	gcc $(OOPTS) types.c

lexeme.o : lexeme.c lexeme.h
	gcc $(OOPTS) lexeme.c

lexer.o : lexer.c lexer.h
	gcc $(OOPTS) lexer.c

#PRSENV
parser.o : parser.c parser.h
	gcc $(OOPTS) parser.c

environment.o : environment.c environment.h
	gcc $(OOPTS) environment.c
	
#EXE
pretty.o : pretty.c pretty.h
	gcc $(OOPTS) pretty.c

eval.o : eval.c eval.h
	gcc $(OOPTS) eval.c

# commands #
############


error1 : eval
	cat error1.colt
error1x : eval
	./colt error1.colt


error2 : eval
	cat error2.colt
error2x : eval
	./colt error2.colt


error3 : eval
	cat error3.colt
error3x : eval
	./colt error3.colt


error4 : eval
	cat error4.colt
error4x : eval
	./colt error4.colt


error5 : eval
	cat error5.colt
error5x : eval
	./colt error5.colt


arrays : eval
	cat arrays.colt
arraysx : eval
	./colt arrays.colt


conditionals : eval
	cat conditionals.colt
conditionalsx : eval
	./colt conditionals.colt

recursion : eval
	cat recursion.colt
recursionx : eval
	./colt recursion.colt


iteration : eval
	cat iteration.colt
iterationx : eval
	./colt iteration.colt


functions : eval
	cat functions.colt
functionsx : eval
	./colt functions.colt


lambda : eval
	cat lambda.colt
lambdax : eval
	./colt lambda.colt


objects : eval
	cat objects.colt
objectsx : eval
	./colt objects.colt


problem : eval
	cat problem.colt
problemx : eval
	./colt problem.colt


test1 : all
	echo Original file:
	cat test1.colt
	echo Pretty Printed version of the original:
	pp test1.colt > test1.pp.1
	cat test1.pp.1
	echo Pretty Printed version of the pretty printed version:
	pp test1.pp.1 > test1.pp.2
	cat test1.pp.2
	diff -s -q test1.pp.1 test1.pp.2


test2 : all
	echo Original file:
	cat test2.colt
	echo Pretty Printed version of the original:
	pp test2.colt > test2.pp.1
	cat test2.pp.1
	echo Pretty Printed version of the pretty printed version:
	pp test2.pp.1 > test2.pp.2
	cat test2.pp.2
	diff -s -q test2.pp.1 test2.pp.2

test3 : all
	echo Original file:
	cat test3.colt
	echo Pretty Printed version of the original:
	pp test3.colt > test3.pp.1
	cat test3.pp.1
	echo Pretty Printed version of the pretty printed version:
	pp test3.pp.1 > test3.pp.2
	cat test3.pp.2
	diff -s -q test3.pp.1 test3.pp.2

test4 : all
	echo Original file:
	cat test4.colt
	echo Pretty Printed version of the original:
	pp test4.colt > test4.pp.1
	cat test4.pp.1
	echo Pretty Printed version of the pretty printed version:
	pp test4.pp.1 > test4.pp.2
	cat test4.pp.2
	diff -s -q test4.pp.1 test4.pp.2

test5 : all
	echo Original file:
	cat test5.colt
	echo Pretty Printed version of the original:
	pp test5.colt > test5.pp.1
	cat test5.pp.1
	echo Pretty Printed version of the pretty printed version:
	pp test5.pp.1 > test5.pp.2
	cat test5.pp.2
	diff -s -q test5.pp.1 test5.pp.2

## clean ###
############
clean : 
	rm -f $(OBJS) $(PRSENV) *.o pp colt
