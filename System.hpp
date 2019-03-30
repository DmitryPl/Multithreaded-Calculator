#ifndef SYSTEM_H
#define SYSTEM_H

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

#define HALT    0
#define INIT    1
#define ERROR   -1
#define SIN     11
#define COS     12
#define LN      13
#define EXP     14

class SystemException : public exception {
public:
	SystemException() = delete;
	SystemException(string &&whatStr) noexcept : whatStr_(std::move(whatStr)) { }
	SystemException(const string &whatStr) noexcept : whatStr_(whatStr) { }
	SystemException(size_t LINE, const string& func, const string& INF1 = "", const string& INF2 = "") noexcept {
		whatStr_ = string("Line: ") + std::to_string(LINE) + string("\nFunc: ") + func + string("\nWhy: ") + INF1 + string(" ") + INF2;
	};

	const char* what() const noexcept override {
		return whatStr_.c_str();
	}
	~SystemException() noexcept = default;
private:
	string whatStr_;
};

void print(const char* what);
void print(const string& what);
bool IsItNumber(const string& word);
int doNothing();
void ping(int line);

#include "System.cpp"

#endif