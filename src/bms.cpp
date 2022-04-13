#pragma once
#include "bms.h"
#include "bass/bass.h"
#include <iostream>
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
			name = "Title";
			author = "Author";
			scene = "";
			for (int i = 0; i < 4; i++) {
				bpm[i] = 0;
				designers[i] = "";
				difficulties[i] = 0;
				charts[i] = CHART();
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
		struct zip_t* zip = zip_open(file.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');
		{
			for (int i = 0; i < 4; i++) {
				string fname = "map" + to_string(i + 1) + ".bms";
				string fcontent = getHeader(i) + getMainData(i);
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

						size_t pos = 0;
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

			zip_entry_open(zip, "info.json");
			{
				zip_entry_read(zip, &buf, &bufsize);
			}
			zip_entry_close(zip);

			char* tmp = (char*)calloc(bufsize + 1, sizeof(char));
			snprintf(tmp, bufsize + 1, "%s", (char*)buf);
			loadInfoJson(tmp);
			free(buf);
			free(tmp);


			music_buf = NULL;
			music_size = 0;

			if (zip_entry_open(zip, "music.ogg") == 0 || zip_entry_open(zip, "music.wav") == 0 || zip_entry_open(zip, "music.mp3") == 0)
			{
				zip_entry_read(zip, &music_buf, &music_size);
			}
			zip_entry_close(zip);

			music = BASS_StreamCreateFile(true, music_buf, 0, music_size +1, NULL);
			cout << BASS_ErrorGetCode();
		}
		zip_close(zip);
		return true;
	}

	string MDMFile::getHeader(int rank) {
		if (charts[rank].empty())
			return "";
		string output = "*-- Header --*\n#GENRE scene_" + scene +
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
		j["scene"] = scene;
		j["levelDesigner"] = designers[0];
		j["levelDesigner1"] = designers[0];
		j["levelDesigner2"] = designers[1];
		j["levelDesigner3"] = designers[2];
		j["levelDesigner4"] = designers[3];
		j["difficulty1"] = difficulties[0];
		j["difficulty2"] = difficulties[1];
		j["difficulty3"] = difficulties[2];
		j["unlockLevel"] = difficulties[3];
		string output = j.dump(4);
		return output;
	}

	void MDMFile::loadHeader(int rank, vector<string> lines) {
		for (int i = 0; i < lines.size(); i++) {
			int pos = lines[i].find(" ");
			string valueName = lines[i].substr(0, pos);
			if (valueName == "BPM") {
				string value = lines[i].substr(pos, lines[i].size() - pos);
				bpm[rank] = stof(value);
			}
			// add this code if GENRE is important (and make genre an array too)
			//else if (valueName == "GENRE") {
			//	string value = lines[i].substr(pos, lines[i].size() - pos);
			//	scene = SceneGenre(stoi(value.erase(0, 6)));
			//}
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
		name = j["name"].get<string>();
		author = j["author"].get<string>();
		bpmInfo = j["bpm"].get<string>();
		scene = j["scene"].get<string>();
		designers[0] = j["levelDesigner"].get<string>();
		designers[0] = j["levelDesigner1"].get<string>();
		designers[1] = j["levelDesigner2"].get<string>();
		designers[2] = j["levelDesigner3"].get<string>();
		designers[3] = j["levelDesigner4"].get<string>();
		difficulties[0] = stoi(j["difficulty1"].get<string>());
		difficulties[1] = stoi(j["difficulty2"].get<string>());
		difficulties[2] = stoi(j["difficulty3"].get<string>());
		difficulties[3] = stoi(j["unlockLevel"].get<string>());
		j.clear();
	}
}

