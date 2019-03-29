#ifndef CALC_H
#define CALC_H

// Rules
// G ::= P \0
// N ::= P | I | [ + - ] [ 0 - 9 ] +
// E ::= T { [ + - ] E } *
// T ::= D { [ * / ] T } *
// P ::= [ ( E ) | E ] *
// I ::= [ sin ( | cos ( | ln(  | sqrt( | e |  pi ] E )
// D ::= N ^ { ( E ) | N }

#include "System.hpp"
#include "openmpi/mpi.h"

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cmath>
#include <exception>
#include <iostream>

using std::getline;
using std::string;
using std::cin;
using std::exception;

class Calculator {
private:
    ulong* factorials;
    uint place;
    uint level;

    string str;
    double answer;

    long ProdTree(int l, int r);
    long FactTree(int n);
    long getFactorial(long n);
    void factorial(int n);
    void master(int, int);
    void slave(int, int);
    void printFact();
    double getTime(double start);
    void Dialog();
    bool parse();
    void skip();
    void update();
    void check();
    
	double GetN();
	double GetT();
	double GetE();
	double GetP();
	double GetD();
	double GetI();
    double GetFunc(uint i);

public:
    explicit Calculator() : 
    place(0), factorials(nullptr), level(10), 
    answer(0), str("") { }

    void Run();
};

#include "Calculator.cpp"

#endif
