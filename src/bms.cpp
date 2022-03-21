#pragma once
#include "bms.h"
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

	// SceneGenre
	const string SceneGenre::names[10] = {
			"Not selected",
			"Space Station",
			"Retrocity",
			"Castle",
			"Rainy Night",
			"Candyland",
			"Oriental",
			"Let's Groove",
			"Touhou",
			"DJMAX"
	};
		
	SceneGenre::SceneGenre(int v) : value(v) {}
	SceneGenre::SceneGenre() : value(0) {}
	SceneGenre::SceneGenre(const SceneGenre&) = default;
	SceneGenre&  SceneGenre::operator=(const SceneGenre&) = default;

	string SceneGenre::getName() const { return names[value]; }
	string SceneGenre::getWeirdName() const { return "scene_" + intToSomeDigit(value, 2); }
	
	// Note
#pragma region Note
	Note::Note(string v) : value(v) {}
	string Note::getName() const {
		return names.at(value);
	}
	const map<string, string> Note::names = {
		{"01", "Small"},
		{"02", "Small (Up)"},
		{"03", "Small (Down)"},
		{"04", "Medium 1"},
		{"05", "Medium 1 (Up)"},
		{"06", "Medium 1 (Down)"},
		{"07", "Medium 2"},
		{"08", "Medium 2 (Up)"},
		{"09", "Medium 2 (Down)"},
		{"0A", "Large 1"},
		{"0B", "Large 2"},
		{"0C", "Raider"},
		{"0D", "Hammer"},
		{"0E", "Gemini"},
		{"0F", "Hold"},
		{"0G", "Masher"},
		{"0H", "Gear"},
		{"0O", "Speed 1 (Both)"},
		{"0P", "Speed 2 (Both)"},
		{"0Q", "Speed 3 (Both)"},
		{"0R", "Speed 1 (Low)"},
		{"0S", "Speed 2 (Low)"},
		{"0T", "Speed 3 (Low)"},
		{"0U", "Speed 1 (High)"},
		{"0V", "Speed 2 (High)"},
		{"0W", "Speed 3 (High)"},
		{"11", "Boss Melee 1"},
		{"12", "Boss Melee 2"},
		{"13", "Boss Projectile 1"},
		{"14", "Boss Projectile 2"},
		{"15", "Boss Projectile 3"},
		{"16", "Boss Masher 1"},
		{"17", "Boss Masher 2"},
		{"18", "Boss Gear"},
		{"1A", "Boss Entrance"},
		{"1B", "Boss Exit"},
		{"1C", "Boss Ready Phase 1"},
		{"1D", "Boss End Phase 1"},
		{"1E", "Boss Ready Phase 2"},
		{"1F", "Boss End Phase 2"},
		{"1G", "Boss Swap Phase 1-2"},
		{"1H", "Boss Swap Phase 2-1"},
		{"1J", "Hide Notes"},
		{"1K", "Unhide Notes"},
		{"1L", "Hide Boss"},
		{"1M", "Unhide boss"},
		{"1O", "Scene Switch (Space Station)"},
		{"1P", "Scene Switch (Retrocity)"},
		{"1Q", "Scene Switch (Castle)"},
		{"1R", "Scene Switch (Rainy Night)"},
		{"1S", "Scene Switch (Candyland)"},
		{"1T", "Scene Switch (Oriental)"},
		{"1U", "Scene Switch (Groove Coaster)"},
		{"1V", "Scene Switch (Touhou)"},
		{"1W", "Scene Switch (DJMAX)"},
		{"20", "P Item"},
		{"21", "Ghost"},
		{"22", "Heart"},
		{"23", "Note"},
		{"25", "Hide Background"},
		{"26", "Unhide Background"},
		{"27", "Screen Scroll UP"},
		{"28", "Screen Scroll DOWN"},
		{"29", "Screen Scroll OFF"},
		{"2A", "Scanline Ripples ON"},
		{"2B", "Scanline Ripples OFF"},
		{"2C", "Chromatic Aberration ON"},
		{"2D", "Chromatic Aberration OFF"},
		{"2E", "Vignette ON"},
		{"2F", "Vignette OFF"},
		{"2G", "TV static ON"},
		{"2H", "TV static OFF"},
		{"2I", "Flashbang START"},
		{"2J", "Flashbang MID"},
		{"2K", "Flashbang END"},
		{"30", "Medium Bullet"},
		{"31", "Medium Bullet (Up)"},
		{"32", "Medium Bullet (Down)"},
		{"33", "Medium Bullet (Laneshift)"},
		{"34", "Small Bullet"},
		{"35", "Small Bullet (Up)"},
		{"36", "Small Bullet (Down)"},
		{"37", "Small Bullet (Laneshift)"},
		{"38", "Large Bullet"},
		{"39", "Large Bullet (Up)"},
		{"3A", "Large Bullet (Down)"},
		{"3B", "Large Bullet (Laneshift)"},
		{"3C", "Boss Bullet 1"},
		{"3D", "Boss Bullet 1 (Laneshift)"},
		{"3E", "Boss Bullet 2"},
		{"3F", "Boss Bullet 2 (Laneshift)"}
	};
	const Note Note::Small() { return Note("01"); }
	const Note Note::SmallUp() { return Note("02"); }
	const Note Note::SmallDown() { return Note("03"); }
	const Note Note::Medium1() { return Note("04"); }
	const Note Note::Medium1Up() { return Note("05"); }
	const Note Note::Medium1Down() { return Note("06"); }
	const Note Note::Medium2() { return Note("07"); }
	const Note Note::Medium2Up() { return Note("08"); }
	const Note Note::Medium2Down() { return Note("09"); }
	const Note Note::Large1() { return Note("0A"); }
	const Note Note::Large2() { return Note("0B"); }
	const Note Note::Raider() { return Note("0C"); }
	const Note Note::Hammer() { return Note("0D"); }
	const Note Note::Gemini() { return Note("0E"); }
	const Note Note::Hold() { return Note("0F"); }
	const Note Note::Masher() { return Note("0G"); }
	const Note Note::Gear() { return Note("0H"); }
	const Note Note::Speed1Both() { return Note("0O"); }
	const Note Note::Speed2Both() { return Note("0P"); }
	const Note Note::Speed3Both() { return Note("0Q"); }
	const Note Note::Speed1Low() { return Note("0R"); }
	const Note Note::Speed2Low() { return Note("0S"); }
	const Note Note::Speed3Low() { return Note("0T"); }
	const Note Note::Speed1High() { return Note("0U"); }
	const Note Note::Speed2High() { return Note("0V"); }
	const Note Note::Speed3High() { return Note("0W"); }
	const Note Note::BossMelee1() { return Note("11"); }
	const Note Note::BossMelee2() { return Note("12"); }
	const Note Note::BossProjectile1() { return Note("13"); }
	const Note Note::BossProjectile2() { return Note("14"); }
	const Note Note::BossProjectile3() { return Note("15"); }
	const Note Note::BossMasher1() { return Note("16"); }
	const Note Note::BossMasher2() { return Note("17"); }
	const Note Note::BossGear() { return Note("18"); }
	const Note Note::BossEntrance() { return Note("1A"); }
	const Note Note::BossExit() { return Note("1B"); }
	const Note Note::BossReadyPhase1() { return Note("1C"); }
	const Note Note::BossEndPhase1() { return Note("1D"); }
	const Note Note::BossReadyPhase2() { return Note("1E"); }
	const Note Note::BossEndPhase2() { return Note("1F"); }
	const Note Note::BossSwapPhase1_2() { return Note("1G"); }
	const Note Note::BossSwapPhase2_1() { return Note("1H"); }
	const Note Note::HideNotes() { return Note("1J"); }
	const Note Note::UnhideNotes() { return Note("1K"); }
	const Note Note::HideBoss() { return Note("1L"); }
	const Note Note::Unhideboss() { return Note("1M"); }
	const Note Note::SceneSwitchSpaceStation() { return Note("1O"); }
	const Note Note::SceneSwitchRetrocity() { return Note("1P"); }
	const Note Note::SceneSwitchCastle() { return Note("1Q"); }
	const Note Note::SceneSwitchRainyNight() { return Note("1R"); }
	const Note Note::SceneSwitchCandyland() { return Note("1S"); }
	const Note Note::SceneSwitchOriental() { return Note("1T"); }
	const Note Note::SceneSwitchGrooveCoaster() { return Note("1U"); }
	const Note Note::SceneSwitchTouhou() { return Note("1V"); }
	const Note Note::SceneSwitchDJMAX() { return Note("1W"); }
	const Note Note::PItem() { return Note("20"); }
	const Note Note::Ghost() { return Note("21"); }
	const Note Note::Heart() { return Note("22"); }
	const Note Note::Note_() { return Note("23"); }
	const Note Note::HideBackground() { return Note("25"); }
	const Note Note::UnhideBackground() { return Note("26"); }
	const Note Note::ScreenScrollUP() { return Note("27"); }
	const Note Note::ScreenScrollDOWN() { return Note("28"); }
	const Note Note::ScreenScrollOFF() { return Note("29"); }
	const Note Note::ScanlineRipplesON() { return Note("2A"); }
	const Note Note::ScanlineRipplesOFF() { return Note("2B"); }
	const Note Note::ChromaticAberrationON() { return Note("2C"); }
	const Note Note::ChromaticAberrationOFF() { return Note("2D"); }
	const Note Note::VignetteON() { return Note("2E"); }
	const Note Note::VignetteOFF() { return Note("2F"); }
	const Note Note::TVstaticON() { return Note("2G"); }
	const Note Note::TVstaticOFF() { return Note("2H"); }
	const Note Note::FlashbangSTART() { return Note("2I"); }
	const Note Note::FlashbangMID() { return Note("2J"); }
	const Note Note::FlashbangEND() { return Note("2K"); }
	const Note Note::MediumBullet() { return Note("30"); }
	const Note Note::MediumBulletUp() { return Note("31"); }
	const Note Note::MediumBulletDown() { return Note("32"); }
	const Note Note::MediumBulletLaneshift() { return Note("33"); }
	const Note Note::SmallBullet() { return Note("34"); }
	const Note Note::SmallBulletUp() { return Note("35"); }
	const Note Note::SmallBulletDown() { return Note("36"); }
	const Note Note::SmallBulletLaneshift() { return Note("37"); }
	const Note Note::LargeBullet() { return Note("38"); }
	const Note Note::LargeBulletUp() { return Note("39"); }
	const Note Note::LargeBulletDown() { return Note("3A"); }
	const Note Note::LargeBulletLaneshift() { return Note("3B"); }
	const Note Note::BossBullet1() { return Note("3C"); }
	const Note Note::BossBullet1Laneshift() { return Note("3D"); }
	const Note Note::BossBullet2() { return Note("3E"); }
	const Note Note::BossBullet2Laneshift() { return Note("3F"); }
#pragma endregion


	// MDMFile
	MDMFile::MDMFile() {
			name = "Title";
			author = "Author";
			scene = SceneGenre();
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

			zip_entry_open(zip, "info.json");
			{
				void* buf = NULL;
				size_t buf_size;
				zip_entry_read(zip, &buf, &buf_size);
				char* d = (char*)buf;
				string copy(d);
				size_t end = copy.find((char)0xfdfdfdfd);
				copy = copy.substr(0, end);
				replaceAll(copy, "\r\n", "\n");
				replaceAll(copy, "\r", "\n");
				loadInfoJson(copy);
				free(buf);
			}
			zip_entry_close(zip);

		}
		zip_close(zip);
		return true;
	}

	string MDMFile::getHeader(int rank) {
		if (charts[rank].empty())
			return "";
		string output = "*-- Header --*\n#GENRE scene_" + intToSomeDigit(scene.value, 2) +
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
					chanout += note->value;
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
		j["scene"] = scene.getWeirdName();
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
				charts[rank][measure][channel].push_back(Note(strvalue));
				// store it as a note anyway, will retrieve it later with stof()
			}
			else {
				string notes = lines[i].substr(6, lines[i].size() - 6);
				for (int j = 0; j < notes.size() / 2; j++) {
					charts[rank][measure][channel].push_back(Note(notes.substr(j * 2, 2)));
				}
			}
		}
	}

	void MDMFile::loadInfoJson(string file) {
		json j = json::parse(file.c_str());
		name = j["name"].get<string>();
		author = j["author"].get<string>();
		bpmInfo = j["bpm"].get<string>();
		string sceneWeird = j["scene"].get<string>();
		scene = SceneGenre(stoi(sceneWeird.erase(0, 6)));
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

