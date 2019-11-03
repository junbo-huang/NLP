//#include "writeSoftData.hpp"
//
//
//writer::writer() {
//
//
//	outputPath = DEFAULT_OUTPATH;
//
//}
//
//writer::writer(std::string xi_path) {
//
//	outputPath = xi_path;
//
//}
//
//
//
//writer::~writer() {
//
//
//}
//
//
//bool writer::write_soft_data(templateInfos& templateFiles, Messages& messages) {
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
//
//
//void writer::write_template_message(Messages& messages) {
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
//
//void writer::choose_template(std::string& outString, xmlreader_light::nodes_type& vars) {
//
//	std::map<size_t, unsigned int> varMap;
//	unsigned int percentage = std::rand() % 100 + 1;
//	unsigned int temp = 0;
//	unsigned int totPct = 0;
//
//	for (size_t s = 0; s < vars.size(); s++) {
//		unsigned int pct = vars[s].node().attribute("possibility").as_uint();
//		if (pct < 0 || pct > 100) {
//			throw std::exception("Percentage possibility must be integers in the range of 0 - 100.");
//		}
//		totPct += pct;
//		varMap[s] = pct;
//	}
//
//	if (totPct != 100) {
//		throw std::exception("Percentage possibility must be add up to 100.");
//	}
//	else {
//		for (std::map<size_t, unsigned int>::iterator it = varMap.begin(); it != varMap.end(); it++) {
//			if (percentage > temp && percentage <= (temp + it->second)) {
//				outString = vars[it->first].node().first_child().value();
//				break;
//			}
//
//			temp += it->second;
//		}
//	}
//
//}
//
