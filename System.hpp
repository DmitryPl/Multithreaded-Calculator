#ifndef SYSTEM_H
#define SYSTEM_H

#include <openmpi/mpi.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <exception>

using std::exception;
using std::string;

const static int success = 42;
const static double PI = 3.1415926535897932385;
const static double E = 2.7182818284590452354;

bool SUM = true;
bool DEBUG = true;
bool DEBUG_FACT = false;

enum Commands
{
	ERROR = -1, HALT, INIT,
	SIN = 11, COS, LN
};

class SystemException : public exception
{
  public:
	SystemException() = delete;
	SystemException(string &&whatStr) noexcept : whatStr_(std::move(whatStr)) {}
	SystemException(const string &whatStr) noexcept : whatStr_(whatStr) {}
	SystemException(size_t LINE, const string &func, const string &INF1 = "", const string &INF2 = "") noexcept
	{
		whatStr_ = string("Line: ") + std::to_string(LINE) + string("\nFunc: ") + func + string("\nWhy: ") + INF1 + string(" ") + INF2;
	};

	const char *what() const noexcept override
	{
		return whatStr_.c_str();
	}
	~SystemException() noexcept = default;

  private:
	string whatStr_;
};

class Message
{
  private:
	int first;
	double second;
	int* comm;
	double *data;

  public:
	explicit Message() : first(ERROR), second(ERROR), data(nullptr) {}
	explicit Message(int _first, double _second) : first(_first), second(_second), data(nullptr) {}
	int getFirst() const { return first; }
	double getSecond() const { return second; }
	int getComm(uint i) const { return comm[i]; }
	double getData(uint i) const { return data[i]; }
	void setData(int* _comm, double* _data) { data = _data; comm = _comm; }
	double returnSum(uint size) {
		double sum = 0;
		for (int i = 0; i < size; i++) {
			sum += data[i];
		}
		return sum;
	}
	void set(int _first, double _second)
	{
		first = _first;
		second = _second;
	}
	~Message()
	{
		if (data != nullptr)
		{
			free(comm);
			free(data);
		}
	}
};

void print(const char *what);
void print(const string &what);
bool IsItNumber(const string &word);
int doNothing();
void send(const Message &mes, int rank, int size);
void get(const Message &mes, int rank, int size);

#include "System.cpp"

#endif