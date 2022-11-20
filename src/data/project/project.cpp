#include "project.h"

#include <vector>
#include <string>

#include "../../audio.h"
#include "../../helpers/json.h"
#include <zip/zip.h>
#include "../../helpers/miscellaneous.h"

namespace data {
	namespace project {
		Project::Project() {
			name = "";
			nameRomanized = "";
			author = "";
			bpmInformation = "";
			levelDesigner = "";
			hideBmsMode = HideMode::Click;
			hideBmsDifficulty = HideDifficulty::Default;
			hideBmsMessage = "";
			song = 0;
			charts[0] = nullptr;
			charts[1] = nullptr;
			charts[2] = nullptr;
			charts[3] = nullptr;
		}

		Project::~Project() {
			if (Audio::IsSongLoaded()) {
				BASS_StreamFree(song);
				free(mSongBuffer);
			}
			name = "";
			nameRomanized = "";
			author = "";
			bpmInformation = "";
			levelDesigner = "";
			hideBmsMode = HideMode::Click;
			hideBmsDifficulty = HideDifficulty::Default;
			hideBmsMessage = "";
			song = 0;
			delete charts[0];
			delete charts[1];
			delete charts[2];
			delete charts[3];
		}

		bool Project::Load(std::string filepath) {
			// Unload previous project
			// Still need to see if it's a good way of doing it. (maybe use an external function to load/sav files?)
			this->~Project();

			// Try opening archive from filepath
			struct zip_t* zip = zip_open(filepath.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');
			bool isOpened = zip != NULL;
			// Load content if it was opened
			if (isOpened)
			{
				// Load map[i].bms files
				for (int i = 0; i < 4; i++) {
					std::string fname = "map" + std::to_string(i + 1) + ".bms";

					// Try opening a file (if inexistant, the buffer will be NULL)
					// TODO: Check if this return a negtive number if file is inexistant.
					if (zip_entry_open(zip, fname.c_str()))
					{
						void* buffer = NULL;
						size_t bufferSize;
						
						if (zip_entry_read(zip, &buffer, &bufferSize) < 0) {
							// mapX.bms exist, load it

							// parse and convert bms file content
							char* tmp = (char*)calloc(bufferSize + 1, sizeof(char));
							snprintf(tmp, bufferSize + 1, "%s", (char*)buffer);
							std::string bmsContent = tmp;

							// remove '\r\n' combo and solo '\r' to unify it to '\n'
							helpers::miscellaneous::replaceAll(bmsContent, "\r\n", "\n");
							helpers::miscellaneous::replaceAll(bmsContent, "\r", "\n");

							// create two lists, on for head data and one of body data
							std::vector<std::string> head = std::vector<std::string>();
							std::vector<std::string> body = std::vector<std::string>();
							size_t pos;
							while ((pos = bmsContent.find("\n")) != std::string::npos) {
								std::string line = bmsContent.substr(0, pos);
								// any line who do not start by a # is considered as a comment
								if (line.length() > 2 && line[0] == '#') {
									if (isdigit(line[1])) {
										body.push_back(line.erase(0, 1));
									}
									else {
										head.push_back(line.erase(0, 1));
									}
								}
								bmsContent.erase(0, pos + 1);
							}

							// init new chart
							charts[i] = new Chart();

							// Load header into chart
							for (int i = 0; i < head.size(); i++) {
								int pos = (int)head[i].find(" ");
								std::string valueName = head[i].substr(0, pos);
								if (valueName == "BPM") {
									std::string value = head[i].substr(pos + 1, head[i].size() - pos);
									// bpm set to 0 if couldn't parse number
									charts[i]->bpm = strtof(value.c_str(), nullptr);
								}
								else if (valueName == "GENRE") {
									std::string value = head[i].substr(pos + 1, head[i].size() - pos);
									charts[i]->scene = value;
								}
							}

							// Load body data into chart
							for (int i = 0; i < body.size(); i++) {
								int measure = stoi(body[i].substr(0, 3));

								// measure +1 here because the 1st measure is the zero
								while (charts[i]->measures.size() < measure + 1) {
									charts[i]->measures.push_back(new Measure());
								}

								// get channel id
								int channel = strtol(body[i].substr(3, 2).c_str(), nullptr, 10);
								charts[i]->measures[measure]->channels.insert({ channel, new Channel(channel) });

								// integrate specials channels (only 02 for now)
								if (channel == 02) {
									std::string strvalue = body[i].substr(6, body[i].size() - 6);
									charts[i]->measures[measure]->channels[channel]->notes.push_back(new Note(strvalue));
									// store it as a note anyway, will retrieve it later with stof()
								}
								else {
									// normal channel
									std::string notes = body[i].substr(6, body[i].size() - 6);
									for (int j = 0; j < notes.size() / 2; j++) {
										charts[i]->measures[measure]->channels[channel]->notes.push_back(new Note(notes.substr(j * 2, 2)));
									}
								}
							}

							free(buffer);
						}
						else {
							// mapX.bms do not exist

							charts[i] = nullptr;
						}
					}
					zip_entry_close(zip);
				}

				// Load info.json
				if (zip_entry_open(zip, "info.json"))
				{
					void* buf = NULL;
					size_t bufsize;
					if (zip_entry_read(zip, &buf, &bufsize)) {
						// nightmare to make it work (corruption and stuff)
						// TODO: check for UTF-8 and other formats
						char* tmp = (char*)calloc(bufsize + 1, sizeof(char));
						snprintf(tmp, bufsize + 1, "%s", (char*)buf);
						nlohmann::json j = nlohmann::json::parse(tmp);

						helpers::json::loadString(j, "name", &name);
						helpers::json::loadString(j, "name_romanized", &nameRomanized);
						helpers::json::loadString(j, "author", &author);
						helpers::json::loadString(j, "bpm", &bpmInformation);
						helpers::json::loadString(j, "levelDesigner", &levelDesigner);
						helpers::json::loadInt(j, "unlockLevel", &unlockLevel);
						helpers::json::loadInt(j, "hideBmsMode", &hideBmsMode);
						helpers::json::loadInt(j, "hideBmsDifficulty", &hideBmsDifficulty);
						helpers::json::loadString(j, "hideBmsMessage", &hideBmsMessage);
						helpers::json::loadStringArray(j, "searchTags", &searchTags);
						
						// load maps designers/difficulties if they exist
						for (int i = 0; i < 4; i++) {
							if (charts[i] != nullptr) {
								// chart exist, load properties
								helpers::json::loadString(j, "levelDesigner" + std::to_string(i + 1), &(charts[i]->designer));
								helpers::json::loadInt(j, "difficulty" + std::to_string(i + 1), &(charts[i]->difficulty));
							}
						}

						free(tmp);
					}
				}
				zip_entry_close(zip);

				// load song file
				if (zip_entry_open(zip, "music.ogg") == 0 || zip_entry_open(zip, "music.wav") == 0 || zip_entry_open(zip, "music.mp3") == 0)
				{
					mSongBuffer = NULL;
					mSongSize = 0;
					song = 0;
					if (zip_entry_read(zip, &mSongBuffer, &mSongSize)) {
						song = BASS_StreamCreateFile(true, mSongBuffer, 0, mSongSize + 1, NULL);
						// std::cout << "BASS error code music loading: " << BASS_ErrorGetCode() << std::endl;
					}

					// we dont free mSongBuffer, BASS needs it
				}
				zip_entry_close(zip);
			}
			zip_close(zip);

			return isOpened;
		}

		bool Project::Save(std::string filepath) {
			struct zip_t* zip = zip_open(filepath.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
			{
				// generate a bms file for each chart
				for (int i = 0; i < 4; i++) {
					// check if chart exist
					if (charts[i] != nullptr) {
						// filename mapX.bms
						std::string filename = "map" + std::to_string(i + 1) + ".bms";

						// generate bms header
						std::string header = "*-- Header --*\n#GENRE " + charts[i]->scene +
							"\n#BPM " + std::to_string(charts[i]->bpm) + "\n\n";
						// add WAVxx support later

						// generate body/main data
						std::string body = "*-- Main Data --*\n";
						int measuresCount = 0;
						for (auto measure = charts[i]->measures.begin(); measure != charts[i]->measures.end(); ++measure) {
							// iterate through measures
							std::string measureOut = "";
							for (auto channel = (*measure)->channels.begin(); channel != (*measure)->channels.end(); ++channel) {
								// then through channels
								std::string channelOut = (*channel)->id + ":";
								for (auto note = (*channel)->notes.begin(); note != (*channel)->notes.end(); ++note) {
									// and finally through notes
									channelOut += (*note)->value;
								}
								measureOut += "#" + helpers::miscellaneous::intToSomeDigit(measuresCount, 3) + channelOut + "\n";
							}
							body += measureOut + "\n";
							measuresCount++;
						}

						zip_entry_open(zip, filename.c_str());
						{
							// write data to bms file
							const char* buf = (header + body).c_str();
							zip_entry_write(zip, buf, strlen(buf));
						}
						zip_entry_close(zip);
					}
				}

				// generate a info.json file
				zip_entry_open(zip, "info.json");
				{

					nlohmann::ordered_json j;
					j["name"] = name;
					j["author"] = author;
					j["bpm"] = bpmInformation;
					// j["scene"] = scene;
					j["levelDesigner"] = levelDesigner;

					for (int i = 0; i < 4; i++) {
						if (charts[i] != nullptr) {
							j["levelDesigner" + std::to_string(i + 1)] = charts[i]->designer;
							j["difficulty" + std::to_string(i + 1)] = std::to_string(charts[i]->difficulty);
						}
						else {
							j["levelDesigner" + std::to_string(i + 1)] = "";
							j["difficulty" + std::to_string(i + 1)] = "";
						}
					}

					if (charts[3] != nullptr) {
						std::string modes[] = { "CLICK", "PRESS", "TOGGLE" };
						j["hideBmsMode"] = modes[hideBmsMode];
						j["hideBmsDifficulty"] = std::to_string(hideBmsDifficulty);
						j["hideBmsMessage"] = hideBmsMessage;
						j["unlockLevel"] = std::to_string(unlockLevel);
					}

					const char* buf = j.dump(4).c_str();
					zip_entry_write(zip, buf, strlen(buf));
				}
				zip_entry_close(zip);

				// see later to implement lame mp3 encoding of the song
			}
			zip_close(zip);
			return true;
		}
	}
}