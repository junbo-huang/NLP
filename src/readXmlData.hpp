#include <iostream>
#include <string>
#include <vector>
#include "helper.hpp"
#include "Structures.h"
#include "xmlreader_light.hpp"


#ifndef READXMLDATA_HPP
#define READXMLDATA_HPP

class dataReader {


private:

	xmlreader_light reader;


	/*std::vector<reportInfo> reports;
	std::vector<eventType1> event1;
	std::vector<eventType2> event2;
	std::vector<eventType1> event3;*/


	void read_nodes(xmlreader_light::node_type node, std::vector<Info>& infos);

public:

	dataReader();
	dataReader(std::string dataFile);
	~dataReader();

	bool read_data(Messages* mesPtr);
	/*Messages get_messages() { return messages; }*/

};

#endif
