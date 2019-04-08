# Multithreaded Calculator
Concurrency Lessons 2019

## Compile
To compile, use `make` command

## Run  
To run the app, use `make run n=X` command, where `X` is number of threads.

## Example
When running the app, enter the function to compute, like:
`cos(0.15/e)*sin(0.17)*8`

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
