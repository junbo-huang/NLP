#include "readXmlData.hpp"


dataReader::dataReader(std::string inputPath) {

	try {

		if (!message_reader.load(inputPath + "data.xml")) throw std::exception("Failed to open data.xml file.\n");
		if (!sensor_aff_reader.load(inputPath + "Sensor-Affiliation-Output.xml"))
			throw std::exception("Failed to open Sensor-Affiliation-Output.xml file.\n");
	}
	catch (std::exception& err) {
		std::cout << err.what() << std::endl;
	}
}

dataReader::~dataReader() {


}



bool dataReader::get_data(Messages* mesPtr) {

	xmlreader_light::nodes_type message_nodes = message_reader.get_nodes(_S("messages/message"));

	for (auto& node : message_nodes) {
		Message tempMessage;
		tempMessage.reportInfo.message_type = message_reader.get_attribute_value<std::string>(node, _S("Type"), "");
		tempMessage.reportInfo.report_time = message_reader.get_value<std::string>(node, _S("ReportTime"), "");
		tempMessage.reportInfo.sensor_id = message_reader.get_attribute_value<std::string>(node.node().first_child(), _S("Sensor_ID"), "");
		
		get_message_data(node, tempMessage.eventInfos);


		mesPtr->push_back(tempMessage);
		
	}

	if (mesPtr->empty()) return false;

	return true;
}

void dataReader::get_message_data(xmlreader_light::node_type node, std::vector<Info>& infos) {
	xmlreader_light::nodes_type info_nodes = message_reader.get_nodes(node, _S("infos/info"));

	for (auto& infoNode : info_nodes) {
		Info tempInfo;
		tempInfo.infoType = message_reader.get_attribute_value<std::string>(infoNode, _S("Type"), "");
		tempInfo.info = static_cast<std::string> (infoNode.node().first_child().value());
		infos.push_back(tempInfo);
	}

}


bool dataReader::get_sensor_aff(std::map<std::string, std::map<std::string, std::string>>& aff_map) {

	
	std::string sensor_id;

	xmlreader_light::nodes_type sensor_nodes = sensor_aff_reader.get_nodes(_S("Sensor-Affiliation-Output/Sensor"));

	for (auto& node : sensor_nodes) {
		sensor_id = sensor_aff_reader.get_attribute_value < std::string >(node, _S("ID"), "");

		xmlreader_light::nodes_type aff_nodes = sensor_aff_reader.get_nodes(node, _S("Affiliations"));
		std::string aff_type;
		std::string aff_name;

		std::map<std::string, std::string> temp_aff_map;
		for (auto& aff_node : aff_nodes) {
			aff_type = sensor_aff_reader.get_attribute_value <std::string>(aff_node, _S("Type"), "");
			aff_name = sensor_aff_reader.get_attribute_value <std::string>(aff_node.node().first_child(), _S("name"), "");

			temp_aff_map.insert(std::pair<std::string, std::string> (aff_type, aff_name));
		}

		
		aff_map[sensor_id] = temp_aff_map;
	}

	if (aff_map.empty()) return false;
	return true;
}


bool dataReader::get_template_data(std::map<std::string, std::vector<std::string>>& templateFiles, std::map<std::string, std::vector<templateInfo>>& templateLibrary) {

	

	for (auto it = templateFiles.begin(); it != templateFiles.end(); it++) {

		std::vector<templateInfo> tempTemplates;
		

		for (auto& file : it->second) {
			if (!template_file_reader.load(file)) {
				std::cout << "Error loading " << file << std::endl;
				return false;
			}

			templateInfo tempTemplate;
			xmlreader_light::node_type tempMasterNode = template_file_reader.get_node(_S("template"));

			auto atts = tempMasterNode.node().attributes();
			for (auto a = atts.begin(); a != atts.end(); a++)
			{
				std::string name = (*a).name(); //get the name of the template attribute
				std::string value = (*a).as_string(); // get the value of the template attribute
				
				tempTemplate.templateAttr.insert(std::make_pair(name, value));
			}

			xmlreader_light::nodes_type tempMasterChildren = template_file_reader.get_nodes(tempMasterNode,_S("info"));
			
			for (auto& child : tempMasterChildren) {
				messageType tempMessageType;
				tempMessageType.isVector = template_file_reader.get_attribute_value<bool>(child, _S("isVector"), false);
				
				xmlreader_light::nodes_type tempMasterGrandChildren = template_file_reader.get_nodes(child, _S("variation"));

				for (auto& grandChild : tempMasterGrandChildren) {
					variation tempVariation;
					tempVariation.possibility = grandChild.node().first_attribute().as_uint();
					tempVariation.content = grandChild.node().first_child().value();
					tempMessageType.variations.push_back(tempVariation);
				}


				std::string tempInfoType = template_file_reader.get_attribute_value<std::string>(child, _S("Type"), "");
				tempTemplate.Messagelibrary.insert(std::make_pair(tempInfoType, tempMessageType));
			}
			
			
			tempTemplates.push_back(tempTemplate);
		}


		templateLibrary.insert(std::make_pair(it->first, tempTemplates));

		
	}

		
	


	return true;
}