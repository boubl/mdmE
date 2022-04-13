#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include <string>
#include "IconsFontAwesome5.h"
#include "bms.h"
#include "editor.h"
#include <ImGuiFileDialog/ImGuiFileDialog.h>
#include <iostream>

#pragma once

namespace ui {
	class MainLayout {

	public:
		static int TopBarHeight;
		static int SideBarWidth;

		MainLayout(BMS::MDMFile* mdmfile, Editor* editor);

		void DrawUI();

	private:
		ImVec2 nextPos;
		float nextSize;
		BMS::MDMFile* file;
		Editor* editor;
		string savedPath = "";

		//windows
		bool showChartInfo;
		bool showPreferences;
		bool showEditEditorView;
		bool showDemo;
		bool showKits;

		void ChartInfo();
		void DrawMainBar();
		void DrawToolBar();
		void DrawNotesBar();
		void DisplayFileDialogs();
		void Preferences();
		void EditEditorView();
		void Kits();
	};
}