#include "Calculator.hpp"
#include "System.hpp"
#include <exception>
#include <string>

using std::string;
using std::exception;

int main() {
    try {
        Calculator calc;
        calc.Run();
    }
    catch (const exception& exc) {
		print(exc.what());
	}
	catch (...) {
		print("Error - default\n");
	}
    return 0;
}