#include "data.h"
#include <iostream>
#include <dirent.h>
#include <io.h>
#include "audio.h"
#include <SDL2/SDL_image.h>

BMS::MDMFile* Data::file;
ImFont* Data::defaultfont;
ImFont* Data::monofont;
vector<Kit> Data::kits;
map<string, SDL_Surface*> Data::sprites;

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
				string ending = ".zip";
				string altending = ".mdk";
				if (name.length() >= ending.length() || name.length() >= altending.length()) {
					if (0 == name.compare(name.length() - ending.length(), ending.length(), ending) ||
						0 == name.compare(name.length() - altending.length(), altending.length(), altending)) {
						Kit kit = Kit();
						cout << "Extracting and loading kit: " << ent->d_name << endl;
						if (kit.LoadZip("./data/kits/", string(ent->d_name))) {
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

	ReloadKitsData();
}

void Data::ReloadKitsData() {
	cout << "Reloading kit data" << endl;

	Kit::allnotes = map<string, Note*>();
	Kit::allevents = map<string, Event*>();
	Kit::allchannels = map<string, Channel*>();
	Kit::allscenes = map<string, string>();

	for (auto kit = Data::kits.begin(); kit != Data::kits.end(); kit++) {
		if (!kit->activated)
			continue;

		ReloadNotes(kit->notes, kit->dir);
		ReloadEvents(kit->events);

		for (auto chan = kit->channels.begin(); chan != kit->channels.end(); chan++) {
			if (Kit::allchannels.find(chan->first) == Kit::allchannels.end()) {
				Kit::allchannels.insert({ chan->first, &chan->second });
			}
		}

		for (auto scene = kit->scenes.begin(); scene != kit->scenes.end(); scene++) {
			if (Kit::allscenes.find(scene->first) == Kit::allscenes.end()) {
				Kit::allscenes.insert({ scene->first, scene->second });
			}
		}
	}

	Audio::ReloadSamples();
}

void Data::ReloadNotes(NoteFolder& folder, string kitpath) {
	for (auto note = folder.notes.begin(); note != folder.notes.end(); note++) {
		if (Kit::allnotes.find(note->first) == Kit::allnotes.end()) {
			Kit::allnotes.insert({ note->first, &note->second });
			if (Data::sprites.find(note->second.sprite) == Data::sprites.end() &&
				!note->second.sprite.empty() && access((kitpath + note->second.sprite).c_str(), 0))
			{
				SDL_Surface* img = IMG_Load((kitpath + note->second.sprite).c_str());
				Data::sprites.insert({ note->second.sprite, img });
			}
		}
	}
	for (auto subfold = folder.folders.begin(); subfold != folder.folders.end(); subfold++) {
		Data::ReloadNotes((*subfold).second, kitpath);
	}
}

void Data::ReloadEvents(EventFolder& folder) {
	for (auto event = folder.events.begin(); event != folder.events.end(); event++) {
		if (Kit::allevents.find(event->first) == Kit::allevents.end()) {
			Kit::allevents.insert({ event->first, &event->second });
		}
	}
	for (auto subfold = folder.folders.begin(); subfold != folder.folders.end(); subfold++) {
		Data::ReloadEvents((*subfold).second);
	}
}