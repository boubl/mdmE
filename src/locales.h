#include <map>
#include <string>

#pragma once

#define CATEGORY std::map<std::string, std::string>

struct Language {
	std::map<std::string, CATEGORY> categories;
	std::string name;
	std::string author;
	std::string version;
	std::string website;
};

class Localization {
public:
	std::map<std::string, Language> languages;
	std::string selected;

	std::string GetString(std::string categoryID, std::string strID);
	void LoadStrings();
private:
	bool Load(std::string filedata, std::string filename);
};