#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string str = "unset";
std::string prev = "unset";
std::string prev2 = "unset";
std::string operation = "unset";
std::string previousOp = "unset";
int output = 0; // the sum of the
int tick = 0; // basically column
int column = 0; // used for error handing
int line = 0;


bool is_number(const std::string& s) // A function to see if a string is a number.
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
} // usage: is_number(stringtocheck). Returns true if the input is a number.

void error(int type) // Error handling, takes in a code and outputs respective info.
{
	for (int i=0; i<tick; ++i) {
    std::cout << "\b";
	}
	std::cout << "Error: Line " + std::to_string(line) + ", Character " + std::to_string(column) + "." << std::endl;
	if (type == 1) {
		std::cout << std::to_string(type) + ": " + "Expected Operation." << std::endl;
	} else if (type == 2) {
		std::cout << std::to_string(type) + ": " + "Expected value. got end of line." << std::endl;
	} else if (type == 3) {
		std::cout << std::to_string(type) + ": " + "Expected value." << std::endl;
	} else if (type == 4) {
		std::cout << std::to_string(type) + ": " + "Got unknown operation." << std::endl;
	} else {
		std::cout << "0: Unknown Error." << std::endl;
	}
} // usage: error

void process(std::string token)
{
	tick += 1;
	if (tick == 1) {
		int i;
		std::istringstream(token) >> i;
		output += i;
		column += 1;
	} else {
		column += 2;
	}
	if (prev == "+" || prev == "-" || prev == "/" || prev == "*") {
		if (column == (str.length() - 2)) {
			error(2); // expected value, got end of line
			exit(1);
		}
		previousOp = prev;
		int i;
		std::istringstream(token) >> i;
		if (prev == "+") {
			output += i;
		} else if (prev == "-") {
			output -= i;
		} else if (prev == "/") {
			output /= i;
		} else if (prev == "*") {
			output *= i;
		}


	} else if (is_number(prev) && is_number(token)) {
		error(1); // expected operation
		exit(1);
	} else {

	}
	//std::cout << prev;
	std::cout << token;
	prev2 = prev;
	prev = token;
}

int main ()
{
	column = 0;
	line = 0;
  std::ifstream file("input.spp");
  while (std::getline(file, str)) {
		line += 1;
		std::string s = str;
		std::string delimiter = " ";
		output = 0;
		prev = "unset";
		prev2 = "unset";
		operation = "unset";
		previousOp = "unset";
		tick = 0;
		column = 0;
		size_t pos = 0;
		std::string token;

		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			process(token);
			s.erase(0, pos + delimiter.length());
		}
		process(s);
		std::cout << std::endl << output << std::endl;
  }
}
