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

			struct templateInfo _template;
			_template.type = configReader.get_attribute_value<std::string>(node, _S("Type"), "");
			_template.path = node.node().first_child().value();

			if (_template.path.empty()) {
				throw std::exception("template file path cannot be empty.");
			}
			else if (_template.type.empty()) {
				throw std::exception("template type cannot be empty.");
			}

			templates.push_back(_template);

		}
		return true;
	}
	catch (std::exception& err) {
		xo_message = err.what();
		return false;
	}

	return true;
}