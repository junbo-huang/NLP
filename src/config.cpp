#include "config.hpp"

Config::Config(const char* configFile) {
	
	try {
		if (!configReader.load(configFile)) {
			throw std::exception("Failed to read config file.");
		}
	}
	catch (std::exception& err) {
		std::cout << err.what() << std::endl;
	}

}

Config::~Config() {

}

bool Config::initialize(std::string& xo_message) {

	
	try {
		inputPath = configReader.get_value<std::string>(_S("config/input_path"), "");
		outputPath = configReader.get_value<std::string>(_S("config/output_path"), "");

		xmlreader_light::nodes_type template_nodes = configReader.get_nodes(_S("config/templates/template"));
		for (auto& node : template_nodes) {

			std::string template_type = configReader.get_attribute_value<std::string>(node, _S("Type"), "");
			std::vector<std::string> template_files;

			xmlreader_light::nodes_type file_nodes = configReader.get_nodes(node, _S("file"));

			for (auto& file_node : file_nodes) {
				template_files.push_back(file_node.node().first_child().value());
			}

			if (template_files.empty()) {
				throw std::exception("template file(s) path must be provided.");
			}
			if (template_type.empty()) {
				throw std::exception("template type cannot be empty.");
			}

			templates.insert(std::pair<std::string, std::vector<std::string>> (template_type, template_files));

		}

	}
	catch (std::exception& err) {
		xo_message = err.what();
		return false;
	}

	return true;
}