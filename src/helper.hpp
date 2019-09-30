#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>


template <class Container>
void splitString(const std::string& str, Container& cont, char delim)
{
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim)) {
		cont.push_back(token);
	}
}


template <class Container>
void splitNumString(const std::string& str, Container& cont, char delim)
{
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim)) {
		cont.push_back(std::stoi(token));
	}
}


//bool stringCompareI(std::string& str1, std::string& str2)
//{
//	auto lamda = [](char& c1, char& c2) {
//		return (c1 == c2 || std::toupper(c1) == std::toupper(c2));
//	};
//
//	bool result;
//	result = (str1.size() == str2.size()) && std::equal(str1.begin(), str1.end(), str2.begin(), lamda);
//
//	return result;
//}