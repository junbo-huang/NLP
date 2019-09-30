#include <iostream>
#include <xmlreader_light.hpp>
#include <map>
#include "stdio.h"
#include "string.h"
#include "Structures.h"

#ifndef WRTIESOFTDATA_HPP
#define WRTIESOFTDATA_HPP


class writer
{
private:

	std::string outputPath;
	std::string outputFile;

	xmlreader_light templateReader;
	Messages filteredMes;
	

	/*Messages tweet;
	Messages phoneCall;
	Messages facebook;
	Messages textMessage;*/

	void write_template_message(Messages& mes);
	void choose_template(std::string& outString, xmlreader_light::nodes_type& vars);
	

public:

	writer();
	writer(std::string xi_path);
	~writer();

	bool write_soft_data(templateInfos& templates, Messages& messages);


};



#endif