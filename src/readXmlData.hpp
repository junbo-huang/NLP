#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "helper.hpp"
#include "Structures.h"
#include "xmlreader_light.hpp"


#ifndef READXMLDATA_HPP
#define READXMLDATA_HPP

class dataReader {


private:

	xmlreader_light message_reader;
	xmlreader_light sensor_aff_reader;

	xmlreader_light template_file_reader;


	void get_message_data(xmlreader_light::node_type node, std::vector<Info>& infos);

public:

	dataReader(std::string inputPath);
	~dataReader();

	bool get_data(Messages* mesPtr);
	bool get_sensor_aff(std::map<std::string, std::map<std::string, std::string>>& aff_map);
	bool get_template_data(std::map<std::string, std::vector<std::string>>& templateFiles);

};

#endif
