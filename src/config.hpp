#pragma once

#include <iostream>
#include <string>
#include <map>
#include "xmlreader_light.hpp"
#include "Structures.h"

#ifndef CONFIG_HPP
#define CONFIG_HPP



class Config {

private:

	xmlreader_light configReader;

	std::string inputPath;
	std::string outputPath;

	std::map<std::string, std::vector<std::string>> templates;

public:


	Config(const char* configFile);
	~Config();
	bool initialize(std::string& xo_message);

	std::string get_inputPath() { return inputPath; }
	std::string get_outputPath() { return outputPath; }
	std::map<std::string, std::vector<std::string>> get_templates() { return templates; }
};


#endif
