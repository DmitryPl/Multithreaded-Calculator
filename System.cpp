#include "System.hpp"

void print(const string& what) {
    printf("%s\n", what.c_str());
}

void print(const char* what) {
    printf("%s\n", what);
}

int doNothing() {
	return success;
}

bool IsItNumber(const string& word) {
	size_t i = 0;
	if (word[0] == '-') {
		i++;
	}
	while (word[i] != '\0') {
		if (!isdigit(word[i++]))
			return false;
	}
	return true;
}       

void ping(int line) {
	printf("PING! %d\n", line);
}