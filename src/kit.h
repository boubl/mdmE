#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

#pragma once

using namespace std;

enum NoteType {
	Normal,
	Double,
	Slider
};

class Note {
public:
	string name;
	NoteType type;
	string sound;
	string sprite;
	vector<string> channels;

	Note();
};

struct NoteFolder {
	map<string, Note> notes;
	map<string, NoteFolder> folders;
};

class Event {
public:
	string name;
	string color;
};

struct EventFolder {
	map<string, Event> events;
	map<string, EventFolder> folders;
};

enum ChannelType {
	NormalChannel,
	Events,
	HexValues,
	Float
};

class Channel {
public:
	string name;
	ChannelType type;

	Channel();
};

class Kit {
public:
	static map<string, Note*> allnotes;
	static map<string, Event*> allevents;
	static map<string, Channel*> allchannels;
	static map<string, string> allscenes;

public:
	bool activated;

	string dir;
	string name;
	string version;
	NoteFolder notes;
	EventFolder events;
	map<string, Channel> channels;
	map<string, string> scenes;

	Kit();

	bool LoadZip(string dirpath, string filename);
	bool LoadDir(string dirpath);
private:
	bool LoadInfo(nlohmann::json& info);
	void LoadNoteFolder(nlohmann::json& jfolder, NoteFolder& nfolder);
	void LoadEventFolder(nlohmann::json* jfolder, EventFolder* efolder);
	void LoadChannels(nlohmann::json* jchans, map<string, Channel>* chans);
};