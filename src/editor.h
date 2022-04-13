#include <vector>
#include <SDL2/SDL.h>
#include "bms.h"

#pragma once

using namespace std;

class Editor {
public:
	SDL_Renderer* renderer;
	BMS::MDMFile* file;

	vector<int> layers;
	float vZoom;
	float vSpan;
	float hZoom;
	float hSpan;

	Editor(SDL_Renderer* renderer, BMS::MDMFile* file);

	void Begin();

	void DisplayBeatLayer(int rank);
private:
	int nextwindowX;
	int nextwindowY;
	int nextwindowW;
	int nextwindowH;

	int BeatCount(int rank);
};