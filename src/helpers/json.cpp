#include "json.h"

#include <vector>
#include <iostream>

namespace helpers {
	// load a string value from a json to the destination string, using a fallback if not found
	bool json::loadString(nlohmann::json j, std::string property_name, std::string* destination, std::string fallback) {
		bool found = j.contains(property_name) && j.at(property_name).is_string();
		if (found) {
			*destination = j.at(property_name).get<std::string>();
		}
		else {
			// property not found, using fallback
			*destination = fallback;
		}
		return found;
	}
	// load a false int (string) value from a json to the destination int, using a fallback if not found
	bool json::loadInt(nlohmann::json j, std::string property_name, int* destination, int fallback) {
		bool found = j.contains(property_name) && j.at(property_name).is_string();
		if (found) {
			// first load the string then convert it to an int. if it's an invalid string or "?", it will be set to 0.
			*destination = std::strtol(j.at(property_name).get<std::string>().c_str(), nullptr, 10);
		}
		else {
			// property not found, using fallback
			*destination = fallback;
		}
		return found;
	}
	// load a string array from a json to the destination vector of strings, using a fallback if not found
	bool json::loadStringArray(nlohmann::json j, std::string property_name, std::vector<std::string>* destination, std::vector<std::string> fallback) {
		bool found = j.contains(property_name) && j.at(property_name).is_string();
		if (found) {
			*destination = j.at(property_name).get<std::vector<std::string>>();
		}
		else {
			// property not found, using fallback
			*destination = fallback;
		}
		return found;
	}
}