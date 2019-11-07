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

		//----------create data reader and writer-----------//
		dataReader xmlReader(inputPath);
		dataWriter fileWriter(outputPath);

		Messages messages; 
		std::map<std::string, std::map<std::string, std::string>> sensor_aff_map;
		std::map<std::string, std::vector<templateInfo>> templateLibrary;

		//--------------------------//


		float duration;
		std::clock_t start = std::clock();//clock start

		//----------read and write data----------//
		xmlReader.get_data(&messages); //populate data
		xmlReader.get_sensor_aff(sensor_aff_map); // populate sensor affiliation data
		xmlReader.get_template_data(templateFiles, templateLibrary);// populate template library

		fileWriter.write_soft_data(templateLibrary, messages, sensor_aff_map); // write soft data to txt files

		//---------------------------------------//


		duration = (std::clock() - start) / (float)CLOCKS_PER_SEC;//clock end
		std::cout << duration << std::endl;

	}

	return 1;


}