# Multithreaded Calculator
Concurrency Lessons 2019

## Example
cos(0.15/e)*sin(0.17)*8

## Compile && Run
mpic++ -g -o test.o main.cpp (in VS)  
mpirun -np N test.o (N - num of threads)  

## System commands
exit -> halt all threads and exit  
DSYS:change ->  
changes the boolean flag to another state : debug some system parts  
SUM:change ->  
changes the boolean flag to another state : uses of sums or lib functions  
DSUM:change ->  
changes the boolean flag to another state : debug some sum funcs  
DALL:change ->  
changes all previous debug flags

## Rules  
G ::= P \0  
N ::= P | I | [ + - ] [ 0 - 9 ] +  
E ::= T { [ + - ] E } *  
T ::= D { [ * / ] T } *  
P ::= [ ( E ) | E ] *  
I ::= [ sin ( | cos ( | ln(  | sqrt( | e |  pi ] E )  
D ::= N ^ { ( E ) | N }  

## Comments
In the sum mode, the main thread sends a message to other threads and to itself which function to calculate and the value of the parameter