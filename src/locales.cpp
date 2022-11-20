#include "locales.h"
#include <dirent.h>
#include <vector>
#include <iostream>
#include <fstream>

std::string Localization::GetString(std::string categoryID, std::string strID)
{
	if (languages[selected].categories.find(categoryID) != languages[selected].categories.end()) {
		//category exist
		if (languages[selected].categories[categoryID].find(strID) != languages[selected].categories[categoryID].end()) {
			//txtfield exist
			return languages[selected].categories[categoryID][strID];
		}
	}
	return "";
}

void Localization::LoadStrings() {
	std::vector<std::string> loadedIds = std::vector<std::string>();
	languages = std::map<std::string, Language>();
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir("./data/locales/")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			std::string name = (char*)&ent->d_name[0];
			if (ent->d_type == DT_REG) {
				std::string ending = ".ini";
				if (name.length() >= ending.length() && 0 == name.compare(name.length() - ending.length(), ending.length(), ending)) {
					std::ifstream f;
					std::string path = "./data/locales/" + name;
					f.open(path.c_str(), std::ios::in);
					std::string filedata;
					while (!f.eof())
						filedata += f.get();
					Load(filedata, name);
					f.close();
				}
			}
		}
		closedir(dir);
	}
	else {
		// couldn't open directory
		perror("Cannot open locales folder");
	}
}

bool Localization::Load(std::string filedata, std::string filename)
{
	Language lang;
	std::string curcategory = "Default";
	lang.name = filename;

	size_t pos;
	while ((pos = filedata.find("\n")) != std::string::npos) {
		std::string line = filedata.substr(0, pos);
		if (line[0] == '[') {
			//get category id
			size_t idend = line.find("]");
			std::string id = line.substr(1, idend-1);
			line.erase(0, idend);

			if (lang.categories.find(id) == lang.categories.end()) {
				//shouldn't happen, maybe just for the default one, but blaa
				lang.categories.insert({ id, CATEGORY() });
			}
			curcategory = id;
		}
		else if (line[0] != '#') {

			//get id
			size_t idend = (line.find(" ") < line.find("=")) ? line.find(" ") : line.find("=");
			std::string id = line.substr(0, idend);
			line.erase(0, idend);

			//get value starting char pos
			size_t valstart = 0;
			bool eqfound = false;
			while (valstart < line.size() && (!eqfound || line[valstart] == ' ')) {
				eqfound = line[valstart] == '=' || eqfound;
				valstart++;
			}

			//starting char pos not found, eol reached :(
			if (valstart >= line.size()) {
				filedata.erase(0, pos + 1);
				continue;
			}

			std::string value = line.substr(valstart, line.size() - valstart);

			if (curcategory == "Default") {
				if (id == "name")
					lang.name = value;
				if (id == "author")
					lang.author = value;
			}
			else {
				if (lang.categories.find(curcategory) == lang.categories.end()) {
					//shouldn't happen, maybe just for the default one, but blaa
					lang.categories.insert({ curcategory, CATEGORY() });
				}

				lang.categories[curcategory].insert({ id, value });
			}
		}
		filedata.erase(0, pos + 1);
	}
	languages.insert({ lang.name, lang });
	return true;
}
