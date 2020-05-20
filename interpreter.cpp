#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bits/stdc++.h>
// slang is a programming language for the 21st century. nope its a calculator right now.
// https://github.com/Xenthio/slang

std::map<std::string, std::string> Variables{{ "test1", "5"}, { "test2", "10" }};
std::map<std::string, std::string> Functions{{ "testfunc", "i = 2;return i"}, { "testfunc2", "return 10" }};

std::string str = "unset";
std::string prev = "unset";
std::string prev2 = "unset";
std::string operation = "unset";
std::string previousOp = "unset";
int mode = 1; // 0: Uninitialised.
							// 1: Math
							// 2: Variable Set
							// 3: Function Set
							// 4: Function Run
std::string first = "unset";
int output = 0; // the sum of the
int tick = 0; // basically column
int column = 0; // used for error handing
int line = 0;
std::string redColour = "\x1B[31m";

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
	std::cout << redColour + "Error: Line " + std::to_string(line) + ", Character " + std::to_string(column) + "." << std::endl;
	if (type == 1) {
		std::cout << std::to_string(type) + ": " + "Expected Operation." << std::endl;
	} else if (type == 2) {
		std::cout << std::to_string(type) + ": " + "Expected value. got end of line." << std::endl;
	} else if (type == 3) {
		std::cout << std::to_string(type) + ": " + "Expected value." << std::endl;
	} else if (type == 4) {
		std::cout << std::to_string(type) + ": " + "Got unknown operation." << std::endl;
	} else if (type == 5) {
		std::cout << std::to_string(type) + ": " + "Cannot set variable at this time." << std::endl;
	} else {
		std::cout << "0: Unknown Error." << std::endl;
	}
} // usage: error

void process(std::string token)
{
	tick += 1;
	std::cout << token;
	if (!is_number(token)) {
		if (Variables.count( token ) == 1) {
			token = Variables[token];
		} else if (Functions.count( token ) == 1) {
			mode = 4;
		}
	}
	if (tick == 1) {
		int i;
		std::istringstream(token) >> i;
		output += i;
		column += 1;
	} else {
		column += 2;
	}

	if (prev == "+" || prev == "-" || prev == "/" || prev == "*" || prev == "=") {
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
		} else if (prev == "=") {
			// oh boy array time
			if (tick != 3) {
				error(5); // Oops! you used tried to set a variable at the wrong point of time!
				exit(1);
			} else {
			 first = prev2;
			 mode = 2;
			 output += i;
			}
		}
	} else if (is_number(prev) && is_number(token)) {
		error(1); // expected operation
		exit(1);
	} else {

	}
	//std::cout << prev;
	if (token == "{") {
		// oh boy function time
		first = prev;
		mode = 3;
		Variables.insert ({ prev, "" }) ;
	}
	prev2 = prev;
	prev = token;
}

int main(int argc, char *argv[])
{
	// freshly copied and pasted from stack overflow (:
	if (argc <= 1)
	{
		std::cout << "Usage: " << argv[0] << " <spp file>" << '\n';
		std::exit(1);
	}

	column = 0;
	line = 0;
  std::ifstream file(argv[1]);
  std::string str;
  while (std::getline(file, str)) {
		if (mode == 3) {
			if (Functions.count( first ) == 1) {
				Functions[first] = Functions[first] + ";" + str;
			}
		} else if (mode == 4) {

		} else {
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
			if (mode == 1) {
				std::cout << std::endl << output << std::endl;
			} else if (mode == 2) {
				//std::cout << std::endl << output << std::endl;
				Variables.insert ({ first, std::to_string(output) }) ;
				mode = 1;
			}
		}
  }
}
