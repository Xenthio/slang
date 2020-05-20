#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bits/stdc++.h>
// slang is a programming language for the 21st century. nope its a calculator right now.
// https://github.com/Xenthio/slang

std::map<std::string, std::string> Variables{{ "test1", "5"}, { "test2", "10" }};
std::map<std::string, std::string> Functions{{ "testfunc", "i = a * 2;2 + i"}, { "testfunc2", "1 + 1" }};

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
int output2 = 0; // the sum of the
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
	//if (mode != 4) {
		std::cout << token;
	//}
	if (!is_number(token)) {
		if (Variables.count( token ) == 1) {
			token = Variables[token];
		} else if (Functions.count( token ) == 1) {
			first = token;
			mode = 4;
			output2 = 0;
			std::string s = Functions[first];
			std::string delimiter = ";";
			std::string token;
			size_t pos = 0;
			while ((pos = s.find(delimiter)) != std::string::npos) {
				token = s.substr(0, pos);
				std::string delimiter2 = " ";
				std::string token2;
				size_t pos2 = 0;
				std::string s2 = token;
				while ((pos2 = s2.find(delimiter2)) != std::string::npos) {
					token2 = s2.substr(0, pos2);
					process(token);
					s2.erase(0, pos2 + delimiter2.length());

				}
				process(s2);
				if (mode == 2) {
					//std::cout << std::endl << output << std::endl;
					Variables.insert ({ first, std::to_string(output) }) ;
					mode = 4;
				}
				s.erase(0, pos + delimiter.length());
			}
			token = output2;
			mode = 1;
		}
	}

		if (tick == 1) {
			int i;
			std::istringstream(token) >> i;
			if (mode == 4) {
				output2 += i;
			} else {
				output += i;
			}
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
				if (mode == 4) {
					output2 += i;
				} else {
					output += i;
				}
			} else if (prev == "-") {
				if (mode == 4) {
					output2 -= i;
				} else {
					output -= i;
				}
			} else if (prev == "/") {
				if (mode == 4) {
					output2 /= i;
				} else {
					output /= i;
				}
			} else if (prev == "*") {
				if (mode == 4) {
					output2 *= i;
				} else {
					output *= i;
				}
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
			Functions.insert ({ prev, "" }) ;
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
			output = 0;
			if (str == "}") {
				mode = 1;
			} else if (Functions.count( first ) == 1) {
				if (Functions[first].length() != 0) {
					str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
					Functions[first] = Functions[first] + ";" + str;
				} else {
					str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
					Functions[first] = str;
				}
			}
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
			} else if (mode == 3) {
				std::cout << " \b";
			} else if (mode == 4) {
				std::cout << " \b";
			}
		}
		//std::cout << "(!";
		//std::cout << Functions["ok"];
		//std::cout << "!)";
  }
}
