#include <iostream>
#include <fstream>
#include <string>

int main ()
{
  std::ifstream file("input.spp");
  std::string str;
  while (std::getline(file, str)) {
    std::cout << str << "\n";
  }
	std::string s = str;
	std::string delimiter = " ";

	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
	    token = s.substr(0, pos);
	    std::cout << token << std::endl;
	    s.erase(0, pos + delimiter.length());
	}
	std::cout << s << std::endl;
}
