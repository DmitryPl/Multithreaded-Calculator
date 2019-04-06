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
  uint level;
  ulong *factorials;

  int world_rank;
  int world_size;
  uint start;
  uint end;

  uint place;
  string str;
  double answer;

  //functions of LL-parser
  double GetN();
  double GetT();
  double GetE();
  double GetP();
  double GetD();
  double GetI();
  double GetFunc(uint i);

  //calculation factorial
  long getFactorial(long n);
  void factorial(int n);
  void printFact();

  //sys fun
  void master();
  void slave();
  void halt();
  void init();
  void wait();
  void Dialog();
  void skip();
  void update();
  void check();
  bool parse();
  double getTime(double start);
  void changeDebugSum();
  void changeDebugSys();
  
  //sum func
  double sinus(double x);
  double cosinus(double x);
  //double logariphm(double x);
  //double exponenta(double x);

  Calculator() {}

public:
  explicit Calculator(int rank, int size) : place(0), level(9), answer(0),
                                            str(""), start(0), end(0), world_rank(rank),
                                            world_size(size), factorials(nullptr) {}

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
