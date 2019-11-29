#include "writeSoftData.hpp"


dataWriter::dataWriter() {


	outputPath = DEFAULT_OUTPATH;

}

dataWriter::dataWriter(std::string xi_path) {

	outputPath = xi_path;

}



dataWriter::~dataWriter() {


}


bool dataWriter::write_soft_data(std::map<std::string, std::vector<templateInfo>>& templateLibrary, Messages& messages, std::map<std::string, std::map<std::string, std::string>>& sensor_aff) {

	std::map<std::string, std::string> outFiles;
	for (auto itr = templateLibrary.begin(); itr != templateLibrary.end(); itr++) {
		/*if ((itr->second).empty()) {
			std::cout << "No template of " << itr->first << " is provided nor specified, therefore not writing " << itr->first << std::endl;
		}*/

		std::string currentOutFile = "";

		outFiles.insert(std::make_pair(itr->first, currentOutFile));
	}

	for (auto& data : messages) {

		bool useGeneralTemplate = false;
		std::string currentSensor = data.reportInfo.sensor_id;
		std::string currentMessageType = data.reportInfo.message_type;
		std::vector<templateInfo> currentTemplates = templateLibrary[currentMessageType];

		if (currentTemplates.empty()) {
			continue;
		}

		int index = 0;

		if (sensor_aff.find(currentSensor) != sensor_aff.end()) {

			//search for matching template
			std::map<std::string, std::string> tempMap = sensor_aff[currentSensor];
			std::vector<int> NumberOfMatches(currentTemplates.size());
			int matchCount = 0;

			for (size_t i = 0; i < currentTemplates.size(); i++) {

				auto currentTemplateAttr = currentTemplates[i].templateAttr;
				if (currentTemplateAttr.empty()) {//general template
					NumberOfMatches[i] = 0;
					continue;
				}
				
				for (auto it = tempMap.begin(); it != tempMap.end(); it++) {
					std::string attr = it->first;
					if (currentTemplateAttr.find(attr) != currentTemplateAttr.end()) {
						if (it->second == currentTemplateAttr[attr]) {
							matchCount++;
						}

					}
				}

				NumberOfMatches[i] = matchCount;
				matchCount = 0;
				
			}

			auto maxPtr = std::max_element(NumberOfMatches.begin(), NumberOfMatches.end());
			if (*maxPtr != 0) {
				index = std::distance(NumberOfMatches.begin(), maxPtr);
			}
			
		}
		else {
			for (size_t i = 0; i < currentTemplates.size(); i++) {
				auto currentTemplateAttr = currentTemplates[i].templateAttr;
				if (currentTemplateAttr.empty()) {//general template
					index = i;
					break;
				}	
			}
		}

		write_template_message(outFiles[currentMessageType], data, currentTemplates[index].Messagelibrary);

	}

	FILE* filePtr;
	for (auto fileItr = outFiles.begin(); fileItr != outFiles.end(); fileItr++) {

		std::string currentFileName = outputPath + fileItr->first + ".txt";
		
		if (!fopen_s(&filePtr, currentFileName.c_str(), "w") && filePtr != NULL) {
		
			fprintf(filePtr, fileItr->second.c_str());
			fclose(filePtr);
		}
		
	}
	return true;
}


void dataWriter::write_template_message(std::string& out, Message& data, std::map<std::string, messageType>& currTemplate) {

	std::string postInfo = "User" + data.reportInfo.sensor_id + "  " + data.reportInfo.report_time + "\n";
	out.append(postInfo);

	//write message for each info
	for (auto& info : data.eventInfos)
	{

		std::string currInfoType = info.infoType;
		if (currTemplate.find(currInfoType) != currTemplate.end()) { //if a piece of info can be found in the template
			std::string outString;
			char* outStringDecode = (char *) malloc(sizeof(char) * 1000);
			if (!currTemplate[currInfoType].variations.empty()) {

				randomize(outString, currTemplate[currInfoType].variations);
				sprintf(outStringDecode, outString.c_str(), info.info.c_str());
				out.append(outStringDecode);
				out.append("\n");
			}

		}

	}

	out.append("\n");
	
}

//bool dataWriter::write_soft_data(templateInfos& templateFiles, Messages& messages) {
//
//	bool isAllTemplatesLoaded = true;
//	
//	for (auto& templateFile : templateFiles) {
//
//		std::string templatePath = templateFile.path;
//
//		if (!templateReader.load(templatePath))
//		{
//			std::string errorMessage = "Failed to load " + templateFile.type + " template file.";
//			std::cout << errorMessage << std::endl;
//
//			isAllTemplatesLoaded = false;
//			throw std::exception(errorMessage.c_str());
//			//return false;
//		}
//		else 
//		{
//			outputFile = outputPath + templateFile.type + ".txt";
//
//			for (auto& message : messages) {
//
//				//case-insensitive string comparison
//				int correctType = _strcmpi(message.reportInfo.message_type.c_str(), templateFile.type.c_str());
//				if (correctType == 0) {
//					filteredMes.push_back(message);
//				}
//
//			}
//
//			write_template_message(filteredMes);
//
//			filteredMes.clear();
//			outputFile.clear();
//		}
//
//	}
//	
//	return true;
//}


//void dataWriter::write_template_message(Messages& messages) {
//
//	FILE* filePtr;
//	if (!fopen_s(&filePtr, outputFile.c_str(), "w") && filePtr != NULL) {
//
//		//iterating through all message nodes
//		for (auto& mes : messages) {
//			//write username and time of post
//			std::string postInfo = "User" + mes.reportInfo.sensor_id + "  " + mes.reportInfo.report_time + "\n";
//			fprintf(filePtr, postInfo.c_str());
//			//write message for each info
//			for (auto& info : mes.eventInfos)
//			{
//
//				xmlreader_light::node_type tempNode = templateReader.get_node(_S("template/" + info.infoType));
//				if (!tempNode.node().empty()) {
//					std::string outString;
//					xmlreader_light::nodes_type vars = templateReader.get_nodes(tempNode, _S("variation"));
//
//					if (!vars.empty()) {
//						choose_template(outString, vars);
//
//						fprintf(filePtr, outString.c_str(), info.info.c_str());
//						fprintf(filePtr, "\n");
//					}
//
//				}
//				else {
//					continue;
//					//std::cout << "Warning! Template for " << info.infoType << " is not specified in the xml file." << std::endl;
//				}
//
//			}
//
//			fprintf(filePtr, "\n");
//		}
//
//		fclose(filePtr);
//
//	}
//
//}

void dataWriter::randomize(std::string& outString, std::vector<variation>& vars) {

	std::map<size_t, unsigned int> varMap;
	srand(time(NULL));
	unsigned int percentage = std::rand() % 100 + 1;
	unsigned int temp = 0;
	unsigned int totPct = 0;

	//error handling
	for (size_t s = 0; s < vars.size(); s++) {
		unsigned int pct = vars[s].possibility;
		if (pct < 0 || pct > 100) {
			throw std::exception("Percentage possibility must be integers in the range of 0 - 100.");
		}
		totPct += pct;
		varMap[s] = pct;
	}
	if (totPct != 100) {
		throw std::exception("Percentage possibility must be add up to 100.");
	}
	//

	else {
		for (auto it = varMap.begin(); it != varMap.end(); it++) {
			if (percentage > temp && percentage <= (temp + it->second)) {
				outString = vars[it->first].content;
				break;
			}

			temp += it->second;
		}
	}

}

