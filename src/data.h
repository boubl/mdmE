#include <imgui.h>
#include <miniaudio.h>
#include <map>
#include "kit.h"
#include "bms.h"
#include <SDL.h>

#pragma once

class Data {
public:
	static BMS::MDMFile* file;
	static ImFont* defaultfont;
	static ImFont* monofont;
	static void InitFonts(ImFont* defaultfont, ImFont* monofont);
	static map<string, SDL_Surface*> sprites;

	static vector<Kit> kits;
	static void LoadKits();
	static void ReloadKitsData();

private:
	static void ReloadNotes(NoteFolder& folder, string kitpath);
	static void ReloadEvents(EventFolder& folder);
};