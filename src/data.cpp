#include "data.h"
#include <iostream>
#include <dirent.h>


ImFont* Data::defaultfont;
ImFont* Data::monofont;
vector<Kit> Data::kits;

void Data::InitFonts(ImFont* idefaultfont, ImFont* imonofont)
{
	defaultfont = idefaultfont;
	monofont = imonofont;
}

void Data::LoadKits() {
	vector<string> loadedIds = vector<string>();
	kits = vector<Kit>();
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir("./data/kits/")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			string name = (char*)&ent->d_name[0];
			if (ent->d_type == DT_REG) {
				//file
				string name = ent->d_name;
				string ending = ".zip";
				string altending = ".mdk";
				if (name.length() >= ending.length() || name.length() >= altending.length()) {
					if (0 == name.compare(name.length() - ending.length(), ending.length(), ending) ||
						0 == name.compare(name.length() - altending.length(), altending.length(), altending)) {
						Kit kit = Kit();
						cout << "Loading kit: " << ent->d_name << endl;
						if (kit.LoadZip("./data/kits/" + string(ent->d_name))) {
							bool validId = true;
							for (auto id = loadedIds.begin(); id != loadedIds.end(); id++) {
								if (*id == kit.name) {
									validId = false;
									cout << "A kit with id \"" << kit.name << "\" was already loaded, skipping this one" << endl;
									break;
								}
							}
							if (validId) {
								loadedIds.push_back(kit.name);
								kits.push_back(kit);
							}
						}
						else
							cout << "Error while loading the kit, abort loading it." << endl;
					}
				}
			}
			else if (ent->d_type == DT_DIR && name != "." && name != "..") {
				//directory
				//todo: check if a kit.json is present, then load it
				DIR* subdir;
				struct dirent* subent;
				if ((subdir = opendir(("./data/kits/" + string(ent->d_name)).c_str())) != NULL) {
					while ((subent = readdir(subdir)) != NULL) {
						if (subent->d_type == DT_REG) {
							//file
							string subname = (char*)&subent->d_name[0];
							if (subname == "kit.json") {
								cout << "Loading kit (folder): " << ent->d_name << endl;
								Kit kit = Kit();
								if (kit.LoadDir(("data/kits/" + string(ent->d_name) + "/").c_str())) {
									bool validId = true;
									for (auto id = loadedIds.begin(); id != loadedIds.end(); id++) {
										if (*id == kit.name) {
											validId = false;
											cout << "A kit with id \"" << kit.name << "\" was already loaded, skipping this one" << endl;
											break;
										}
									}
									if (validId) {
										loadedIds.push_back(kit.name);
										kits.push_back(kit);
									}
								}
								else
									cout << "Error while loading the kit, abort loading it." << endl;
							}
						}
					}
					closedir(subdir);
				}
				else {
					// couldn't open directory
					perror("Cannot open folder");
				}
			}
		}
		closedir(dir);
	}
	else {
		// couldn't open directory
		perror("Cannot open kits folder");
	}

}