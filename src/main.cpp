#include <iostream>
#include <string>
#include "config.hpp"
#include "readXmlData.hpp"
#include "writeSoftData.hpp"


int main(int argc, char* argv[]) {

	char* xi_configFile = argv[1];
	Config config(xi_configFile);
	std::string errorMessage;

	if (!config.initialize(errorMessage)) {
		throw std::exception(errorMessage.c_str());
	}
	else {
		std::string inputPath = config.get_inputPath();
		std::string outputPath = config.get_outputPath();
		std::map<std::string, std::vector<std::string>> templateFiles = config.get_templates();

		dataReader xmlReader(inputPath);
		//writer fileWriter(outputPath);

		Messages messages;
		std::map<std::string, std::map<std::string, std::string>> sensor_aff_map;

		float duration;
		std::clock_t start = std::clock();//clock start

		xmlReader.get_data(&messages);
		xmlReader.get_sensor_aff(sensor_aff_map);
		xmlReader.get_template_data(templateFiles);

		//fileWriter.write_soft_data(templateFiles, messages);

		duration = (std::clock() - start) / (float)CLOCKS_PER_SEC;//clock end
		std::cout << duration << std::endl;

		//std::cout << messages.size() << std::endl;

		//testing
	}

	return 1;


}