#ifndef EXEC_H
#define EXEC_H

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

using std::exception;

class Executor
{
  private:
    uint level;
    ulong *factorials;
    int world_rank;
    int world_size;
    Executor() {}
    //void wait();
    //void send();

    double getTime(double start);
    long getFactorial(long n);
    void factorial(int n);
    void printFact();

  public:
    explicit Executor(int rank, int size) : level(0), factorials(nullptr),
                                            world_rank(rank), world_size(size) { }
    void Run();
};

#include "Executor.cpp"

#endif