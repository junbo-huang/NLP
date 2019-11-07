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
	std::string outputFile;

	//xmlreader_light templatereader;
	//messages filteredmes;
	

	/*messages tweet;
	messages phonecall;
	messages facebook;
	messages textmessage;*/

	void write_template_message(messages& mes);

	//void choose_template(std::string& outstring, xmlreader_light::nodes_type& vars);
	

public:

	dataWriter();
	dataWriter(std::string xi_path);
	~dataWriter();

	//bool write_soft_data(templateinfos& templates, messages& messages);
	bool write_soft_data(std::map<std::string, std::vector<templateInfo>>& templateLibrary, Messages& Messages, std::map<std::string, std::map<std::string, std::string>>& sensor_aff);

};



#endif