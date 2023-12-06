#include <sstream>
#include <string>
#include <iostream>

#define URN 0
#define PARITY 1
#define DAY 2
#define CLASS_NUM 3

inline std::vector<std::string> parse_request_string(std::string target) {
	target.erase(target.begin());
	std::vector<std::string> parsed_values;

	std::string s;
	std::stringstream ss(target);

	while (std::getline(ss, s, '/')) {
		parsed_values.push_back(s);
	}
	return parsed_values;
}