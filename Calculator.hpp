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

using std::cin;
using std::exception;
using std::getline;
using std::string;

class Calculator
{
  private:
    uint place;
    uint level;

    string str;
    double answer;

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
    explicit Calculator() : place(0), level(10),
                            answer(0), str("") {}

    void Run(int, int);
};

#include "Calculator.cpp"

#endif
