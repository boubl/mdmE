#pragma once
#include "bms.h"
#include "bass/bass.h"
#include <iostream>
#include "audio.h"
using namespace nlohmann;
using namespace std;

namespace helpPlease {
	string intToSomeDigit(int n, int len)
	{
		// lol i don't understand anything in this function found on stack, but it works and seems fast
		string result(len--, '0');
		for (int val = (n < 0) ? -n : n; len >= 0 && val != 0; --len, val /= 10)
			result[len] = '0' + val % 10;
		if (len >= 0 && n < 0) result[0] = '-';
		return result;
	}

	void replaceAll(string& str, const string& from, const string& to) {
		if (from.empty())
			return;
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

}

using namespace helpPlease;

namespace BMS {

	// MDMFile
	MDMFile::MDMFile() {
			name = "";
			author = "";
			for (int i = 0; i < 4; i++) {
				bpm[i] = 0;
				scene[i] = "";
				designers[i] = "";
				difficulties[i] = 0;
				charts[i] = CHART();
			}
		}

	MDMFile::~MDMFile() {
		if (Audio::IsSongLoaded()) {
			BASS_StreamFree(music);
			free(music_buf);
		}
	}

	bool MDMFile::Save(string file) {
		struct zip_t* zip = zip_open(file.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
		{
			for (int i = 0; i < 4; i++) {
				if (charts[i].empty())
					continue;
				string fname = "map" + to_string(i + 1) + ".bms";
				string fcontent = getHeader(i) + getMainData(i);
				zip_entry_open(zip, fname.c_str());
				{
					const char* buf = fcontent.c_str();
					zip_entry_write(zip, buf, strlen(buf));
				}
				zip_entry_close(zip);
			}

			zip_entry_open(zip, "info.json");
			{
				string why = getInfoJson();
				const char* buf = why.c_str();
				zip_entry_write(zip, buf, strlen(buf));
			}
			zip_entry_close(zip);

		}
		zip_close(zip);
		return true;
	}

	bool MDMFile::Load(string file) {
		this->~MDMFile();
		struct zip_t* zip = zip_open(file.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');
		{
			for (int i = 0; i < 4; i++) {
				string fname = "map" + to_string(i + 1) + ".bms";
				zip_entry_open(zip, fname.c_str());
				{
					void* buf = NULL;
					size_t buf_size;
					zip_entry_read(zip, &buf, &buf_size);
					if (buf != NULL) {
						char* d = (char*)buf;
						string strbuf(d);
						strbuf = strbuf.substr(0, strbuf.size() - 4);
						replaceAll(strbuf, "\r\n", "\n");
						replaceAll(strbuf, "\r", "\n");
						vector<string> head = vector<string>();
						vector<string> data = vector<string>();

						size_t pos;
						while ((pos = strbuf.find("\n")) != string::npos) {
							string line = strbuf.substr(0, pos);
							if (line[0] == '#') {
								if (isdigit(line[1])) {
									data.push_back(line.erase(0, 1));
								}
								else {
									head.push_back(line.erase(0, 1));
								}
							}
							strbuf.erase(0, pos + 1);
						}

						loadHeader(i, head);
						loadMainData(i, data);
						free(d);
					}
					//free(buf);
				}
				zip_entry_close(zip);
			}


			void* buf = NULL;
			size_t bufsize;

			if (zip_entry_open(zip, "info.json"))
			{
				zip_entry_read(zip, &buf, &bufsize);
				char* tmp = (char*)calloc(bufsize + 1, sizeof(char));
				snprintf(tmp, bufsize + 1, "%s", (char*)buf);
				loadInfoJson(tmp);
				free(tmp);
			}
			zip_entry_close(zip);

			free(buf);


			music_buf = NULL;
			music_size = 0;

			if (zip_entry_open(zip, "music.ogg") == 0 || zip_entry_open(zip, "music.wav") == 0 || zip_entry_open(zip, "music.mp3") == 0)
			{
				zip_entry_read(zip, &music_buf, &music_size);

				music = BASS_StreamCreateFile(true, music_buf, 0, music_size + 1, NULL);
				cout << "BASS error code music loading: " << BASS_ErrorGetCode() << endl;

				//we dont free music_buf, BASS needs it
			}
			zip_entry_close(zip);
		}
		zip_close(zip);
		return true;
	}

	string MDMFile::getHeader(int rank) {
		if (charts[rank].empty())
			return "";
		string output = "*-- Header --*\n#GENRE " + scene[rank] +
			"\n#BPM " + to_string(bpm[rank]) + "\n\n";
		// add WAVxx support later
		return output;
	}

	string MDMFile::getMainData(int rank) {
		if (charts[rank].empty())
			return "";

		string output = "*-- Main Data --*\n";
		int mcount = 0;
		// yeah so this shit just write all the data to output
		for (auto measure = charts[rank].begin(); measure != charts[rank].end(); ++measure) {
			string meashead = "#" + intToSomeDigit(mcount, 3);
			string measout = "";
			for (auto channel = measure->begin(); channel != measure->end(); ++channel) {
				string chanout = channel->first + ":";
				for (auto note = channel->second.begin(); note != channel->second.end(); ++note) {
					chanout += *note;
				}
				measout += meashead + chanout + "\n";
			}
			output += measout + "\n";
			mcount++;
		}
		return output;
	}

	string MDMFile::getInfoJson() {
		ordered_json j;
		j["name"] = name;
		j["author"] = author;
		j["bpm"] = bpmInfo;
		j["scene"] = scene[1];
		j["levelDesigner"] = designers[0];
		j["levelDesigner1"] = designers[0];
		j["levelDesigner2"] = designers[1];
		j["levelDesigner3"] = designers[2];
		j["levelDesigner4"] = designers[3];
		j["difficulty1"] = (difficulties[0] == 0) ? "?" : to_string(difficulties[0]);
		j["difficulty2"] = (difficulties[1] == 0) ? "?" : to_string(difficulties[1]);
		j["difficulty3"] = (difficulties[2] == 0) ? "?" : to_string(difficulties[2]);
		j["unlockLevel"] = (difficulties[3] == 0) ? "?" : to_string(difficulties[3]);
		if (ismapactive[3]) {
			if (hideBmsMode == HideBmsMode::Click) {
				j["hideBmsMode"] = "CLICK";
			}
			else if (hideBmsMode = HideBmsMode::Press) {
				j["hideBmsMode"] = "PRESS";
			}
			else {
				j["hideBmsMode"] = "TOGGLE";
			}
			j["hideBmsMessage"] = hideBmsMessage;
			j["hideBmsDifficulty"] = to_string(hideBmsDifficulty);
		}
		string output = j.dump(4);
		return output;
	}

	void MDMFile::loadHeader(int rank, vector<string> lines) {
		for (int i = 0; i < lines.size(); i++) {
			int pos = (int)lines[i].find(" ");
			string valueName = lines[i].substr(0, pos);
			if (valueName == "BPM") {
				string value = lines[i].substr(pos+1, lines[i].size() - pos);
				bpm[rank] = stof(value);
			}
			else if (valueName == "GENRE") {
				string value = lines[i].substr(pos+1, lines[i].size() - pos);
				scene[rank] = value;
			}
		}
	}

	void MDMFile::loadMainData(int rank, vector<string> lines) {
		charts[rank] = CHART();
		for (int i = 0; i < lines.size(); i++) {
			int measure = stoi(lines[i].substr(0, 3));

			// measure +1 here because the 1st measure is the zero
			while (charts[rank].size() < measure + 1) {
				charts[rank].push_back(MEASURE());
			}

			string channel = lines[i].substr(3, 2);
			charts[rank][measure].insert({ channel, CHANNEL() });

			//integrate specials channels (only 02 for now)
			if (channel == "02") {
				string strvalue = lines[i].substr(6, lines[i].size() - 6);
				charts[rank][measure][channel].push_back(strvalue);
				// store it as a note anyway, will retrieve it later with stof()
			}
			else {
				string notes = lines[i].substr(6, lines[i].size() - 6);
				for (int j = 0; j < notes.size() / 2; j++) {
					charts[rank][measure][channel].push_back(notes.substr(j * 2, 2));
				}
			}
		}
	}

	void MDMFile::loadInfoJson(char* file) {
		json j = json::parse(file);

		//name isn't required
		if (j.contains("name") && j.at("name").is_string())
			name = j.at("name").get<string>();
		else {
			cout << "Map doesn't have a name, setting it to \"No name\"." << endl;
			name = "No name";
		}

		//author isn't required
		if (j.contains("author") && j.at("author").is_string())
			author = j.at("author").get<string>();
		else {
			cout << "Map doesn't have an author, setting it to Unknown." << endl;
			author = "Unknown";
		}

		//bpm isn't required
		if (j.contains("bpm") && j.at("bpm").is_string())
			bpmInfo = j.at("bpm").get<string>();
		else {
			cout << "Map doesn't have a bpm (informative one), setting it to Unknown." << endl;
			bpmInfo = "Unknown";
		}

		//scene in the json is useless

		//levelDesigner isn't required
		if (j.contains("levelDesigner") && j.at("levelDesigner").is_string())
			designers[0] = j.at("levelDesigner").get<string>();
		else {
			cout << "Map doesn't have a default level designer, setting it to Unknown." << endl;
			designers[0] = "Unknown";
		}

		//levelDesigner1 isn't required
		if (j.contains("levelDesigner1") && j.at("levelDesigner1").is_string())
			designers[0] = j.at("levelDesigner1").get<string>();
		else {
			cout << "Map doesn't have a level designer for the easy map, setting it to an empty string." << endl;
			designers[0] = "";
		}

		//levelDesigner2 isn't required
		if (j.contains("levelDesigner2") && j.at("levelDesigner2").is_string())
			designers[1] = j.at("levelDesigner2").get<string>();
		else {
			cout << "Map doesn't have a level designer for the hard map, setting it to an empty string." << endl;
			designers[1] = "";
		}

		//levelDesigner3 isn't required
		if (j.contains("levelDesigner3") && j.at("levelDesigner3").is_string())
			designers[2] = j.at("levelDesigner3").get<string>();
		else {
			cout << "Map doesn't have a level designer for the master map, setting it to an empty string." << endl;
			designers[2] = "";
		}

		//levelDesigner4 isn't required
		if (j.contains("levelDesigner4") && j.at("levelDesigner4").is_string())
			designers[3] = j.at("levelDesigner4").get<string>();
		else {
			cout << "Map doesn't have a level designer for the hidden map, setting it to an empty string." << endl;
			designers[3] = "";
		}

		//difficulty1 isn't required
		if (j.contains("difficulty1") && j.at("difficulty1").is_string()) {
			string diff = j.at("difficulty1").get<string>();
			char* result;
			long converted = strtol(diff.c_str(), &result, 10);
			if (*result) {
				cout << "Map doesn't have a valid difficulty (or is \"?\" then everything is fine) for the easy map, setting it to 0." << endl;
				difficulties[0] = 0;
			}
			else {
				difficulties[0] = converted;
			}
		}
		else {
			cout << "Map doesn't have a difficulty for the easy map, setting it to 0." << endl;
			difficulties[0] = 0;
		}

		//difficulty2 isn't required
		if (j.contains("difficulty2") && j.at("difficulty2").is_string()) {
			string diff = j.at("difficulty2").get<string>();
			char* result;
			long converted = strtol(diff.c_str(), &result, 10);
			if (*result) {
				cout << "Map doesn't have a valid difficulty (or is \"?\" then everything is fine) for the hard map, setting it to 0." << endl;
				difficulties[1] = 0;
			}
			else {
				difficulties[1] = converted;
			}
		}
		else {
			cout << "Map doesn't have a difficulty for the hard map, setting it to 0." << endl;
			difficulties[1] = 0;
		}

		//difficulty3 isn't required
		if (j.contains("difficulty3") && j.at("difficulty3").is_string()) {
			string diff = j.at("difficulty3").get<string>();
			char* result;
			long converted = strtol(diff.c_str(), &result, 10);
			if (*result) {
				cout << "Map doesn't have a valid difficulty (or is \"?\" then everything is fine) for the master map, setting it to 0." << endl;
				difficulties[2] = 0;
			}
			else {
				difficulties[2] = converted;
			}
		}
		else {
			cout << "Map doesn't have a difficulty for the master map, setting it to 0." << endl;
			difficulties[2] = 0;
		}

		//unlockLevel isn't required
		if (j.contains("unlockLevel") && j.at("unlockLevel").is_string()) {
			string diff = j.at("unlockLevel").get<string>();
			char* result;
			long converted = strtol(diff.c_str(), &result, 10);
			if (*result) {
				cout << "Map doesn't have a valid difficulty (or is \"?\" then everything is fine) for the hidden map, setting it to 0." << endl;
				difficulties[3] = 0;
			}
			else {
				difficulties[3] = converted;
			}
		}
		else {
			cout << "Map doesn't have a difficulty for the hidden map, setting it to 0." << endl;
			difficulties[3] = 0;
		}

		//hideBmsMode isn't required
		if (j.contains("hideBmsMode") && j.at("hideBmsMode").is_string()) {
			string mode = j.at("hideBmsMode").get<string>();
			if (mode == "CLICK") {
				hideBmsMode = HideBmsMode::Click;
			}
			else if (mode == "PRESS") {
				hideBmsMode = HideBmsMode::Press;
			}
			else if (mode == "TOGGLE") {
				hideBmsMode = HideBmsMode::Toggle;
			}
			else {
				cout << "hideBmsMode was not valid, defaulting to CLICK" << endl;
				hideBmsMode = HideBmsMode::Click;
			}
		}
		else {
			cout << "hideBmsMode was not specified, defaulting to CLICK" << endl;
			hideBmsMode = HideBmsMode::Click;
		}

		//hideBmsDifficulty isn't required
		if (j.contains("hideBmsDifficulty") && j.at("hideBmsDifficulty").is_string()) {
			string diff = j.at("hideBmsDifficulty").get<string>();
			char* result;
			long converted = strtol(diff.c_str(), &result, 10);
			if (*result) {
				cout << "Map doesn't have a valid rank for the hidden map, setting it to default (0)." << endl;
				difficulties[3] = 0;
			}
			else if (converted >= -1 && converted <= 3) {
				difficulties[3] = converted;
			}
			else {
				cout << "Map doesn't have a valid rank for the hidden map, setting it to default (0)." << endl;
			}
		}
		else {
			cout << "hideBmsMode was not specified, defaulting to CLICK" << endl;
			hideBmsMode = HideBmsMode::Click;
		}

		j.clear();
	}
}

