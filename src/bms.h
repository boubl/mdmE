#pragma once

#include <string>
#include <map>
#include <vector>
#include <zip.h>
#include <nlohmann/json.hpp>
#include <miniaudio.h>
using namespace nlohmann;
using namespace std;

#define CHANNEL vector<string>
#define MEASURE map<string, CHANNEL>
#define CHART vector<MEASURE>

namespace helpPlease {
	string intToSomeDigit(int n, int len);
	void replaceAll(string& str, const string& from, const string& to);
}

enum HideBmsMode {
	Click, //Default, the hidden chart will activate by spam-clicking on the difficulty text.
	Press, //The hidden chart will activate by pressing and holding on the chart's cover art in the song select.
	Toggle //The custom chart will activate by switching between all three difficulties back and forth like ouroboros hidden. This requires 3 difficulties.
};

enum HideBmsDifficulty {
	Special = -1,
	Default,
	Easy,
	Hard,
	Master
};

namespace BMS {

	class MDMFile {
	public:
		string name;
		string author;
		string bpmInfo;//in info.json file, bpm is a string and is more informative than useful
		float bpm[4];
		string scene[4];
		string designers[4];
		int difficulties[4];
		bool ismapactive[4];
		HideBmsMode hideBmsMode;
		HideBmsDifficulty hideBmsDifficulty;
		string hideBmsMessage;
		// ranks (array of measures) -> measures (map of channels) -> channels (list of notes) -> notes (simple string)
		CHART charts[4];

		HSTREAM music;

		MDMFile();
		~MDMFile();

		bool Save(string file);
		bool Load(string file);
	private:
		void* music_buf;
		size_t music_size;
		string getHeader(int rank);
		string getMainData(int rank);
		string getInfoJson();
		void loadHeader(int rank, vector<string> lines);
		void loadMainData(int rank, vector<string> lines);
		void loadInfoJson(char* file);
	};
}