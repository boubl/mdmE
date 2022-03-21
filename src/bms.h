#pragma once
#include <string>
#include <map>
#include <vector>
#include <kubazip/zip/zip.h>
#include <nlohmann/json.hpp>
using namespace nlohmann;
using namespace std;

#define CHANNEL vector<Note>
#define MEASURE map<string, CHANNEL>
#define CHART vector<MEASURE>

namespace helpPlease {
	string intToSomeDigit(int n, int len);
	void replaceAll(string& str, const string& from, const string& to);
}

namespace BMS {
	class SceneGenre {
	public:
		int value;
		//todo: write header file and make the names static
		const static string names[10];
		SceneGenre(int v);
		SceneGenre();
		SceneGenre(const SceneGenre&);
		SceneGenre& operator=(const SceneGenre&);

		string getName() const;
		string getWeirdName() const;
	};

	class Note {
	public:
		string value;
		Note(string v);
		string getName() const;
		const static map<string, string> names;
		const static Note Small();
		const static Note SmallUp();
		const static Note SmallDown();
		const static Note Medium1();
		const static Note Medium1Up();
		const static Note Medium1Down();
		const static Note Medium2();
		const static Note Medium2Up();
		const static Note Medium2Down();
		const static Note Large1();
		const static Note Large2();
		const static Note Raider();
		const static Note Hammer();
		const static Note Gemini();
		const static Note Hold();
		const static Note Masher();
		const static Note Gear();
		const static Note Speed1Both();
		const static Note Speed2Both();
		const static Note Speed3Both();
		const static Note Speed1Low();
		const static Note Speed2Low();
		const static Note Speed3Low();
		const static Note Speed1High();
		const static Note Speed2High();
		const static Note Speed3High();
		const static Note BossMelee1();
		const static Note BossMelee2();
		const static Note BossProjectile1();
		const static Note BossProjectile2();
		const static Note BossProjectile3();
		const static Note BossMasher1();
		const static Note BossMasher2();
		const static Note BossGear();
		const static Note BossEntrance();
		const static Note BossExit();
		const static Note BossReadyPhase1();
		const static Note BossEndPhase1();
		const static Note BossReadyPhase2();
		const static Note BossEndPhase2();
		const static Note BossSwapPhase1_2();
		const static Note BossSwapPhase2_1();
		const static Note HideNotes();
		const static Note UnhideNotes();
		const static Note HideBoss();
		const static Note Unhideboss();
		const static Note SceneSwitchSpaceStation();
		const static Note SceneSwitchRetrocity();
		const static Note SceneSwitchCastle();
		const static Note SceneSwitchRainyNight();
		const static Note SceneSwitchCandyland();
		const static Note SceneSwitchOriental();
		const static Note SceneSwitchGrooveCoaster();
		const static Note SceneSwitchTouhou();
		const static Note SceneSwitchDJMAX();
		const static Note PItem();
		const static Note Ghost();
		const static Note Heart();
		const static Note Note_();
		const static Note HideBackground();
		const static Note UnhideBackground();
		const static Note ScreenScrollUP();
		const static Note ScreenScrollDOWN();
		const static Note ScreenScrollOFF();
		const static Note ScanlineRipplesON();
		const static Note ScanlineRipplesOFF();
		const static Note ChromaticAberrationON();
		const static Note ChromaticAberrationOFF();
		const static Note VignetteON();
		const static Note VignetteOFF();
		const static Note TVstaticON();
		const static Note TVstaticOFF();
		const static Note FlashbangSTART();
		const static Note FlashbangMID();
		const static Note FlashbangEND();
		const static Note MediumBullet();
		const static Note MediumBulletUp();
		const static Note MediumBulletDown();
		const static Note MediumBulletLaneshift();
		const static Note SmallBullet();
		const static Note SmallBulletUp();
		const static Note SmallBulletDown();
		const static Note SmallBulletLaneshift();
		const static Note LargeBullet();
		const static Note LargeBulletUp();
		const static Note LargeBulletDown();
		const static Note LargeBulletLaneshift();
		const static Note BossBullet1();
		const static Note BossBullet1Laneshift();
		const static Note BossBullet2();
		const static Note BossBullet2Laneshift();
	};

	class MDMFile {
	public:
		string name;
		string author;
		string bpmInfo;//in info.json file, bpm is a string and is more informative than useful
		float bpm[4];
		SceneGenre scene;
		string designers[4];
		int difficulties[4];
		// ranks (array) -> measures (list) -> channels (map) -> notes (list)
		CHART charts[4];

		MDMFile();

		bool Save(string file);
		bool Load(string file);
	private:
		string getHeader(int rank);
		string getMainData(int rank);
		string getInfoJson();
		void loadHeader(int rank, vector<string> lines);
		void loadMainData(int rank, vector<string> lines);
		void loadInfoJson(string file);
	};
}