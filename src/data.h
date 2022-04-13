#include <imgui/imgui.h>
#include "kit.h"

#pragma once

class Data {
public:
	static ImFont* defaultfont;
	static ImFont* monofont;
	static void InitFonts(ImFont* defaultfont, ImFont* monofont);

	static vector<Kit> kits;
	static void LoadKits();
};