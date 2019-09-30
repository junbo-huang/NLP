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
		templateInfos templateFiles = config.get_templates();

		dataReader xmlReader(inputPath);
		writer fileWriter(outputPath);

		Messages messages;
		

		float duration;
		std::clock_t start = std::clock();//clock start

		xmlReader.read_data(&messages);

		fileWriter.write_soft_data(templateFiles, messages);

		duration = (std::clock() - start) / (float)CLOCKS_PER_SEC;//clock end
		std::cout << duration << std::endl;

		//std::cout << messages.size() << std::endl;

	}

	return 1;


}