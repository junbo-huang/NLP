#include "readXmlData.hpp"

dataReader::dataReader() {
	try {
		if (!reader.load(DEFAULT_INPATH)) {
			throw std::exception("Failed to open data file.\n");
		}
	}
	catch (std::exception& err) {
		std::cout << err.what() << std::endl;
	}

}

dataReader::dataReader(std::string dataFile) {

	try {
		if (!reader.load(dataFile)) {
			throw std::exception("Failed to open data file.\n");
		}
	}
	catch (std::exception& err) {
		std::cout << err.what() << std::endl;
	}
}

dataReader::~dataReader() {


}

//bool dataReader::read_data() {
//
//	xmlreader_light::nodes_type message_nodes = reader.get_nodes(_S("messages/message"));
//	reports.reserve(message_nodes.size());
//
//	//read data from the xml input and store them into defined structures
//	for (xmlreader_light::node_type node : message_nodes) {
//		reportInfo tempInfo;
//		tempInfo.message_type = reader.get_attribute_value<std::string>(node, _S("Type"), "");
//
//		tempInfo.reporter.sensor_id = reader.get_attribute_value<std::string>(node.node().first_child(), _S("Sensor_ID"), "");
//		tempInfo.reporter.event_id = reader.get_attribute_value<std::string>(node.node().first_child(), _S("EventID"), "");
//		tempInfo.reporter.severity = reader.get_attribute_value<std::string>(node.node().first_child(), _S("Severity"), "");
//		tempInfo.report_time = reader.get_value<std::string>(node, _S("ReportTime"), "");
//
//		if (tempInfo.reporter.event_id == "E1") {
//
//			read(node, &event1, E1);
//
//		}
//		else if (tempInfo.reporter.event_id == "E2") {
//
//			read(node, &event2, E2);
//
//		}
//		else {
//
//			read(node, &event3, E3);
//
//		}
//		
//		reports.push_back(tempInfo);
//
//	}
//
//	/*for (std::vector<reportInfo>::iterator it = reports.begin(); it != reports.end(); it++) {
//
//
//	}*/
//
//	//std::cout << static_cast<eventType1*>(reports[1].linker)->location << std::endl;
//	return true;
//}


bool dataReader::read_data(Messages* mesPtr) {

	xmlreader_light::nodes_type message_nodes = reader.get_nodes(_S("messages/message"));

	for (auto& node : message_nodes) {
		Message tempMessage;
		tempMessage.reportInfo.message_type = reader.get_attribute_value<std::string>(node, _S("Type"), "");
		tempMessage.reportInfo.report_time = reader.get_value<std::string>(node, _S("ReportTime"), "");
		tempMessage.reportInfo.sensor_id = reader.get_attribute_value<std::string>(node.node().first_child(), _S("Sensor_ID"), "");
		
		read_nodes(node, tempMessage.eventInfos);


		mesPtr->push_back(tempMessage);
		
	}


	return true;
}

void dataReader::read_nodes(xmlreader_light::node_type node, std::vector<Info>& infos) {
	xmlreader_light::nodes_type info_nodes = reader.get_nodes(node, _S("infos/info"));

	for (auto& infoNode : info_nodes) {
		Info tempInfo;
		tempInfo.infoType = reader.get_attribute_value<std::string>(infoNode, _S("Type"), "");
		tempInfo.info = static_cast<std::string> (infoNode.node().first_child().value());
		infos.push_back(tempInfo);
	}

}


//void dataReader::read(xmlreader_light::node_type& node, void* eventType, structType event) {
//
//	if (event == E1 || event == E3) {
//
//		eventType1 tempEvent;
//		xmlreader_light::nodes_type info_nodes = reader.get_nodes(node, _S("infos/info"));
//
//		for (auto& infoNode : info_nodes) {
//			string_type tempAttr = reader.get_attribute_value<string_type>(infoNode, _S("Type"), "");
//
//			if (!tempAttr.compare("Brand")) {
//				std::string tempBrand = infoNode.node().first_child().value();
//				splitString(tempBrand, tempEvent.brand, ';');
//				tempEvent.isBrandKnown = true;
//			}
//			else if (!tempAttr.compare("Color")) {
//				std::string tempColor = infoNode.node().first_child().value();
//				splitString(tempColor, tempEvent.color, ';');
//				tempEvent.isColorKnown = true;
//			}
//			else if (!tempAttr.compare("PlateNumber")) {
//				std::string tempPlateNum = infoNode.node().first_child().value();
//				splitString(tempPlateNum, tempEvent.plateNumber, ';');
//			}
//			else if (!tempAttr.compare("Location")) {
//				std::string tempLocation = infoNode.node().first_child().value();
//				tempEvent.location = tempLocation;
//				tempEvent.isLocationKnown = true;
//			}
//			else if (!tempAttr.compare("TotalNumberofPassengers")) {
//				unsigned int temp = std::stoi(infoNode.node().first_child().value());
//				tempEvent.totalNumberOfPassengers = temp;
//				tempEvent.isTotNumOfPaxsKnown = true;
//			}
//			else if (!tempAttr.compare("TotalPepopleNumberInAccident")) {
//				unsigned int temp = std::stoi(infoNode.node().first_child().value());
//				tempEvent.totalPeopleNumberInAccident = temp;
//				tempEvent.isTotPplNumInAccKnown = true;
//			}
//			else if (!tempAttr.compare("CarNumberInAccident")) {
//				unsigned int temp = std::stoi(infoNode.node().first_child().value());
//				tempEvent.carNumberInAccident = temp;
//				tempEvent.isCarNumInAccKnown = true;
//			}
//			else if (!tempAttr.compare("NumberofPassengers")) {
//				std::string tempNumOfPaxs = infoNode.node().first_child().value();
//				splitNumString(tempNumOfPaxs, tempEvent.numberOfPassengers, ';');
//				tempEvent.isNumOfPaxsKnown = true;
//			}
//			else if (!tempAttr.compare("Speed")) {
//				std::string tempSpeed = infoNode.node().first_child().value();
//				splitString(tempSpeed, tempEvent.speed, ';');
//				tempEvent.isSpeedKnown = true;
//			}
//			else if (!tempAttr.compare("start_time")) {
//				std::string tempTime = infoNode.node().first_child().value();
//				tempEvent.start_time = tempTime;
//				tempEvent.isStartTimeKnown = true;
//			}
//
//		}
//
//		
//		//tempEvent.reportIndex = reportInd;
//		static_cast<std::vector<eventType1>*>(eventType)->push_back(tempEvent);
//	}
//
//	else{
//		eventType2 tempEvent;
//		xmlreader_light::nodes_type info_nodes = reader.get_nodes(node, _S("infos/info"));
//		for (auto& infoNode : info_nodes) {
//			string_type tempAttr = reader.get_attribute_value<string_type>(infoNode, _S("Type"), "");
//
//			if (!tempAttr.compare("Color")) {
//				std::string temp = infoNode.node().first_child().value();
//				tempEvent.color = temp;
//				tempEvent.isColorKnown = true;
//			}
//			else if (!tempAttr.compare("buildingNumberInAccident")) {
//				unsigned int temp = std::stoi(infoNode.node().first_child().value());
//				tempEvent.buildingNumberInAccident = temp;
//				tempEvent.isBuildingNumInAccKnown = true;
//			}
//			else if (!tempAttr.compare("Location")) {
//				std::string tempLocation = infoNode.node().first_child().value();
//				tempEvent.location = tempLocation;
//				tempEvent.isLocationKnown = true;
//				//std::cout << static_cast<eventType1*>(tempInfo.linker)->location << std::endl;
//			}
//			else if (!tempAttr.compare("BuildingNumber")) {
//				unsigned int temp = std::stoi(infoNode.node().first_child().value());
//				tempEvent.buildingNumber = temp;
//				tempEvent.isBuildingNumKnown = true;
//			}
//			else if (!tempAttr.compare("NumberofDeath")) {
//				std::string temp = infoNode.node().first_child().value();
//				splitNumString(temp, tempEvent.numberOfDeath, ';');
//				tempEvent.isNumOfDeathKnown = true;
//			}
//			else if (!tempAttr.compare("SuitNumber")) {
//				std::string temp = infoNode.node().first_child().value();
//				splitString(temp, tempEvent.suitNumber, ';');
//				tempEvent.isSuitNumKnown = true;
//			}
//			else if (!tempAttr.compare("start_time")) {
//				std::string tempTime = infoNode.node().first_child().value();
//				tempEvent.start_time = tempTime;
//				tempEvent.isStartTimeKnown = true;
//			}
//		}
//
//		//tempEvent.reportIndex = reportInd;
//		static_cast<std::vector<eventType2>*>(eventType)->push_back(tempEvent);
//	}
//
//}