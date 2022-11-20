#ifndef MDME_HELPERS_JSON_
#define MDME_HELPERS_JSON_

#include <string>

#include <nlohmann/json.hpp>

namespace helpers {
	namespace json {
		bool loadString(nlohmann::json j, std::string property_name, std::string* destination, std::string fallback = "");
		bool loadInt(nlohmann::json j, std::string property_name, int* destination, int fallback = 0);
		bool loadStringArray(nlohmann::json j, std::string property_name, std::vector<std::string>* destination, std::vector<std::string> fallback = std::vector<std::string>());
	}
}

#endif // !MDME_HELPERS_JSON_
