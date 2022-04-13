#pragma once
#include <string>
#include <map>
#include <vector>
#include <zip/zip.h>
#include <nlohmann/json.hpp>
#include <bass/bass.h>
using namespace nlohmann;
using namespace std;

#define CHANNEL vector<string>
#define MEASURE map<string, CHANNEL>
#define CHART vector<MEASURE>

namespace helpPlease {
	string intToSomeDigit(int n, int len);
	void replaceAll(string& str, const string& from, const string& to);
}

namespace BMS {

	class MDMFile {
	public:
		string name;
		string author;
		string bpmInfo;//in info.json file, bpm is a string and is more informative than useful
		float bpm[4];
		string scene;
		string designers[4];
		int difficulties[4];
		// ranks (array) -> measures (list) -> channels (map) -> notes (list)
		CHART charts[4];

		HSTREAM music;

		MDMFile();

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