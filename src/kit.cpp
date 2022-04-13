#include "kit.h"
#include "zip/zip.h"
#include <nlohmann/json.hpp>
#include <dirent.h>
#include <iostream>
#include <fstream>

Note::Note() :
	name(""),
	type((NoteType)0),
	sound(false),
	sprite(false),
	channels(vector<string>()) {

}

Channel::Channel() :
	name(""),
	type((ChannelType)0)
{

}

Kit::Kit() :
	activated(false),
	name(""),
	version(""),
	notes(NoteFolder()),
	events(EventFolder()),
	channels(map<string, Channel>()),
	scenes(map<string, string>())
{
}

bool Kit::LoadZip(string filepath) {
	zip_t* zip = zip_open(filepath.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');

	void* infoBuf;
	size_t infoSize = 0;
	if (zip_entry_open(zip, "kit.json") == 0)
	{
		zip_entry_read(zip, &infoBuf, &infoSize);
	}
	else {
		cout << "Couldn't open kit.json in the archive. Abort loading the kit." << endl;
		return false;
	}
	char* tmp = (char*)calloc(infoSize + 1, sizeof(char));
	snprintf(tmp, infoSize + 1, "%s", (char*)infoBuf);
	nlohmann::json j = nlohmann::json::parse(tmp);

	if (!LoadInfo(&j)) {
		cout << "Couldn't load kit.json correctly." << endl;
		return false;
	}

	activated = true;

	free(infoBuf);
	free(tmp);

	return true;
}

bool Kit::LoadDir(string dirpath) {
	ifstream f;
	string path = dirpath + "kit.json";
	f.open(path.c_str(), ios::in);
	nlohmann::json j = nlohmann::json::parse(f);

	if (!LoadInfo(&j)) {
		cout << "Couldn't load kit.json correctly.";
		return false;
	}

	activated = true;

	f.close();
	return true;
}

bool Kit::LoadInfo(nlohmann::json* info) {
	if (info->contains("name")) {
		if (!info->at("name").is_string()) {
			cout << "Kit's name isn't a string, abort loading it." << endl;
			return false;
		}
		name = info->at("name").get<string>();
	}
	else {
		cout << "Kit doesn't have a name, abort loading it." << endl;
		return false;
	}

	//version isn't required
	if (info->contains("version") && info->at("version").is_string())
		version = info->at("version").get<string>();
	else {
		cout << "Kit doesn't have a version, setting it to Unknown" << endl;
		version = "Unknown";
	}

	//notes aren't required
	if (info->contains("notes") && info->at("notes").is_object()) {
		notes = NoteFolder();
		LoadNoteFolder(&info->at("notes"), &notes);
	}
	else {
		cout << "Kit doesn't have any notes defined, nothing to load." << endl;
		notes = NoteFolder();
	}

	//events aren't required
	if (info->contains("events") && info->at("events").is_object()) {
		events = EventFolder();
		LoadEventFolder(&info->at("events"), &events);
	}
	else {
		cout << "Kit doesn't have any events defined, nothing to load." << endl;
		notes = NoteFolder();
	}

	//channels aren't required
	if (info->contains("channels") && info->at("channels").is_object()) {
		channels = map<string, Channel>();
		LoadChannels(&info->at("channels"), &channels);
	}
	else {
		cout << "Kit doesn't have any channel defined, nothing to load." << endl;
		notes = NoteFolder();
	}

	//scenes aren't required
	if (info->contains("scenes") && info->at("scenes").is_object()) {
		scenes = map<string, string>();
		nlohmann::json* jscenes = &info->at("scenes");
		for (auto ent = jscenes->begin(); ent != jscenes->end(); ent++) {
			if (ent->is_string())
				scenes.insert({ ent.key(), ent->get<string>() });
			else
				cout << "Scene \"" << ent.key() << "\" isn't a string, not adding it to the list" << endl;
		}
	}
	else {
		cout << "Kit doesn't have any scene defined, nothing to load." << endl;
		notes = NoteFolder();
	}
}

void Kit::LoadNoteFolder(nlohmann::json* jfolder, NoteFolder* nfolder) {
	nfolder->folders = map<string, NoteFolder>();
	nfolder->notes = map<string, Note>();

	for (auto ent = jfolder->begin(); ent != jfolder->end(); ent++) {
		string entryKey = ent.key();
		if (entryKey.size() == 2) {
			Note note = Note();
			nlohmann::json* jnote = &ent.value();

			//name, type and channels are required fields.
			if (jnote->contains("name")) {
				if (!jnote->at("name").is_string()) {
					cout << "Note's name isn't a string, abort loading this one." << endl;
					continue;
				}
				note.name = jnote->at("name").get<string>();
			}
			else {
				cout << "Note doesn't have a name, abort loading this one." << endl;
				continue;
			}

			if (jnote->contains("type")) {
				if (!jnote->at("type").is_number_integer()) {
					cout << "Note's type isn't an int, abort loading this one." << endl;
					continue;
				}
				note.type = (NoteType)jnote->at("type").get<int>();
			}
			else {
				cout << "Note doesn't have a type, abort loading this one." << endl;
				continue;
			}

			if (jnote->contains("channels")) {
				note.channels = vector<string>();
				if (!jnote->at("channels").is_array()) {
					cout << "Note's channel list isn't an array, abort loading this one." << endl;
					continue;
				}
				for (auto chan = jnote->at("channels").begin(); chan != jnote->at("channels").end(); chan++) {
					if (!chan.value().is_string()) {
						cout << "A member of Note's channel list isn't a string, not adding it to the list." << endl;
						continue;
					}
					note.channels.push_back(chan.value().get<string>());
				}
			}
			else {
				cout << "Note doesn't have a channel list, abort loading this one." << endl;
				continue;
			}

			if (jnote->contains("sound") && jnote->at("sound").is_boolean())
				note.sound = jnote->at("sound").get<bool>();
			else {
				cout << "Note doesn't have a sound field, setting it to false" << endl;
				note.sound = false;
			}

			if (jnote->contains("sprite") && jnote->at("sprite").is_boolean())
				note.sprite = jnote->at("sprite").get<bool>();
			else {
				cout << "Note doesn't have a sprite field, setting it to false" << endl;
				note.sprite = false;
			}

			nfolder->notes.insert({ entryKey, note });
		}
		else {
			nfolder->folders.insert({ entryKey, NoteFolder() });
			LoadNoteFolder(&ent.value(), &nfolder->folders.at(entryKey));
		}
	}
}

void Kit::LoadEventFolder(nlohmann::json* jfolder, EventFolder* efolder) {
	efolder->folders = map<string, EventFolder>();
	efolder->events = map<string, Event>();

	for (auto ent = jfolder->begin(); ent != jfolder->end(); ent++) {
		string entryKey = ent.key();
		if (entryKey.size() == 2) {
			Event event = Event();
			nlohmann::json* jnote = &ent.value();

			//name, type and channels are required fields.
			if (jnote->contains("name")) {
				if (!jnote->at("name").is_string()) {
					cout << "Event's name isn't a string, abort loading this one." << endl;
					continue;
				}
				event.name = jnote->at("name").get<string>();
			}
			else {
				cout << "Event doesn't have a name, abort loading this one." << endl;
				continue;
			}

			if (jnote->contains("color")) {
				if (!jnote->at("color").is_string()) {
					cout << "Event's color isn't a string, abort loading this one." << endl;
					continue;
				}
				event.color = jnote->at("color").get<string>();
			}
			else {
				cout << "Event doesn't have a color, abort loading this one." << endl;
				continue;
			}

			efolder->events.insert({ entryKey, event });
		}
		else {
			efolder->folders.insert({ entryKey, EventFolder() });
			LoadEventFolder(&ent.value(), &efolder->folders.at(entryKey));
		}
	}
}

void Kit::LoadChannels(nlohmann::json* jchans, map<string, Channel>* chans) {
	for (auto ent = jchans->begin(); ent != jchans->end(); ent++) {
		string entKey = ent.key();
		if (entKey.size() != 2) {
			cout << "Channel ID not valid (should be 2 characters), abort loading it." << endl;
			continue;
		}

		Channel chan = Channel();
		//name isn't required, can be replaced with the channel id
		if (ent->contains("name") && ent->at("name").is_string()) {
			chan.name = ent->at("name").get<string>();
		}
		else {
			cout << "Channel doesn't have a name, naming it by it's ID." << endl;
			chan.name = entKey;
		}

		//type is required, it's important
		if (ent->contains("type")) {
			if (!ent->at("type").is_number_integer()) {
				cout << "Channel's type isn't an integer, abort loading it." << endl;
				continue;
			}
			chan.type = (ChannelType)ent->at("type").get<int>();
		}
		else {
			cout << "Channel doesn't have a type, abort loading it." << endl;
			continue;
		}

		chans->insert({ entKey, chan });
	}
}