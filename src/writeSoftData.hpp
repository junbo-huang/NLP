#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <iostream>
#include <xmlreader_light.hpp>
#include <map>
#include "stdio.h"
#include "string.h"
#include "structures.h"

#ifndef wrtiesoftdata_hpp
#define wrtiesoftdata_hpp



class dataWriter
{
private:

	std::string outputPath;
	//std::string outputFile;

	//xmlreader_light templatereader;
	//messages filteredmes;
	

	/*messages tweet;
	messages phonecall;
	messages facebook;
	messages textmessage;*/

	void write_template_message(std::string& outPath, Message& data, std::map<std::string, messageType>& currentTemplate);

	void randomize(std::string& outString, std::vector<variation>& vars);
	

public:

	dataWriter();
	dataWriter(std::string xi_path);
	~dataWriter();

	//bool write_soft_data(templateinfos& templates, messages& messages);
	bool write_soft_data(std::map<std::string, std::vector<templateInfo>>& templateLibrary, Messages& Messages, std::map<std::string, std::map<std::string, std::string>>& sensor_aff);

};



#endif