# COLT
COLT Object Oriented Programming Language.

Once downloaded, COLT can be used by typing "make" on the command line. 
Once it is compiled, the language is run through ./colt or colt with the source code 
to run as the first argument and any other arguments following.

Example: running the file problem.colt with a testfile.txt argument

make
./colt problem.colt testfile.txt
make clean

Running make clean will clean the object files and not allow any other scripts to be run with 
./colt or colt

How to write COLT source code:

Comments:
---------
? single line

?? multi
line ??

Variable defintion:
-------------------
def var = 10 

Function definition:
--------------------
def function foo(params) as
	? statements
end

Object definition: 
------------------
def object obj as
	? obj vars
	def function obj(params) as
		? obj constructor -- optional
	end

	? obj functions
end

Conditionals:
-------------
<cond> is a comparsion:
	a < b, a > b
	a <= b, a >= b
	a != b, a == b
	a && b, a || b

if <cond> then
	? consequents
end

if <cond> then
	? consequents
else 
	? alternatives
end

if <cond1> then
	? consequents
else if <cond2> then
	? consequents
end

if <cond1> then
	? consequents
else if <cond2> then
	? consequents
else 
	? alternatives
end

Iteration:
----------
(below loops from 0 to 10)
def temp = 0

general: 
while <cond> do 
	?statements
end

<cond> is same as Conditionals

while temp < 10 do
	? statements
	++temp ? need to iterate, or infinite loop
end

(below loops from 1 to 9)
def temp = 0
while ++temp < 10 do
	?statements
end

lambda expressions:
-------------------
def l = (function(x) as x+x end)
l(5) should be 10

arrays:
-------
defining an arrary of size 5:
a = array(5)

setting array value at index 2:
set(a, 2, "string")

getting array value at index 2:
get(a, 2)

getting array length:
len(a)

librarys:
---------
define "library.colt"