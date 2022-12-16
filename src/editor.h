#include <vector>
#include <map>
#include "SDL.h"
#include "bms.h"
#include "chaneditor.h"

#pragma once

using namespace std;

class Editor {
public:
	SDL_Renderer* renderer;

	vector<ChanEditor*> channels;
	int rank;

	Editor();
	Editor(SDL_Renderer* renderer);

	static bool Init(SDL_Renderer* renderer);
	static void DisplayChannels();
	static void ReloadBPMChanges();
	static float GetBeatPos();
	static int GetHoveredChannel();
	static int Editor::GetSnapedBeat();

	static string selectednote;
	static Editor instance;
	static int beatsnap;

private:
	int nextwindowX;
	int nextwindowY;
	int nextwindowW;
	int nextwindowH;

	// beat count since last change, bpm
	map<float, float> bpmchanges;
};