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

* If you want to finish working with the program enter `exit`
* If you want to use in calculations library functions enter `LIB`
* If you want to use in calculations by summation of a series enter `SUM`

In programm you can turn off or on debug-flags
Enter `NAME_OF_FLAG:on` to turn on or enter `NAME_OF_FLAG:off` to turn off debug  

* `DSUM` flag controls debug of calculation by summation functions
* `DSYS` flag controls system debug
* `DALL` flag controls `DSYS` and `DSUM` flags

For example you can enter `DALL:on` to see all debug information

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