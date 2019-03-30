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
  ulong *factorials;

  int world_rank;
  int world_size;

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

  long getFactorial(long n);
  void factorial(int n);
  void printFact();

  void master();
  void halt();
  void init();
  void wait();
  void slave();
  Calculator() {}

public:
  explicit Calculator(int rank, int size) : place(0), level(10), answer(0), str(""),
                                            world_rank(rank), world_size(size), factorials(nullptr) {}

  void Run();
  ~Calculator()
  {
    if (factorials != nullptr)
    {
      free(factorials);
    }
  }
};

#include "Calculator.cpp"

#endif
