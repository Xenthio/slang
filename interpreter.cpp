#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string prev = "unset";
std::string prev2 = "unset";
std::string operation = "unset";
int output = 0;
int tick = 0;
int column = 0;
int line = 0;

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void error(int type, int line, int column)
{
	if (type == 1) {
		std::cout << "Error: Line " + std::to_string(line) + "char " + std::to_string(column) + "," << std::endl;
		std::cout << "Expected Operation" << std::endl;
	}
}

void process(std::string token)
{
	tick += 1;
	if (tick == 1) {
		int i;
		std::istringstream(token) >> i;
		output += i;
	}
	std::cout << token;
	if (prev == "+") {
		int i;
		std::istringstream(token) >> i;
		output += i;
	} else if (prev == "-") {
		int i;
		std::istringstream(token) >> i;
		output -= i;
	} else if (is_number(prev2)){
		error(1, line, tick);
	}
	prev = token;
}

int main ()
{
	column = 0;
	line = 0;
  std::ifstream file("input.spp");
  std::string str;
  while (std::getline(file, str)) {
		line += 1;
		std::string s = str;
		std::string delimiter = " ";
		output = 0;
		prev = "unset";
		prev2 = "unset";
		operation = "unset";
		tick = 0;
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
