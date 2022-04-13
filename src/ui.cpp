#include "ui.h"
#include "data.h"
#include <math.h>

namespace ui {
	int MainLayout::SideBarWidth = 0;
	int MainLayout::TopBarHeight = 0;

	void MainLayout::ChartInfo() {
		if (ImGui::Begin("Chart info", &showChartInfo)) {
			ImGui::InputText("Name", &file->name);
			ImGui::InputText("Author", &file->author);
			ImGui::InputText("BPM", &file->bpmInfo);
			ImGui::End();
		}
	}

	void MainLayout::DrawMainBar() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New...", "Ctrl+N")) {
					savedPath = "";
					*file = BMS::MDMFile();
				}
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					ImGuiFileDialog::Instance()->OpenDialog("OpenFileDlgKey", "Open...", ".mdm", ".");
				if (ImGui::BeginMenu("Open recent")) {
					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					if (savedPath == "")
						ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Save", ".mdm", ".");
					else
						file->Save(savedPath);
				}
				if (ImGui::MenuItem("Save as...", "Ctrl+S"))
					ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Save as...", ".mdm", ".");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				ImGui::MenuItem("Undo", "Ctrl+Z");
				ImGui::MenuItem("Redo", "Ctrl+Y");
				ImGui::Separator();
				if (ImGui::MenuItem("Chart Info")) {
					showChartInfo = true;
				}
				if (ImGui::MenuItem("Preferences")) {
					showPreferences = true;
				}
				if (ImGui::MenuItem("Editor View")) {
					showEditEditorView = true;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) {
				if (ImGui::MenuItem("ImGui Demo"))
					showDemo = true;
				if (ImGui::MenuItem("Kits"))
					showKits = true;
				ImGui::MenuItem("FABIO", "Lanzoni");
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_FA_FAST_BACKWARD)) {
				BASS_ChannelPause(file->music);
				BASS_ChannelSetPosition(file->music, 0, BASS_POS_BYTE);
			}
			if (ImGui::MenuItem(ICON_FA_PLAY)) {
				BASS_ChannelPlay(file->music, false);
			}
			if (ImGui::MenuItem(ICON_FA_PAUSE)) {
				BASS_ChannelPause(file->music);
			}
			// Who would use this?
			//if (ImGui::MenuItem(ICON_FA_FAST_FORWARD)) {
			//	BASS_ChannelPause(file->music);
			//	BASS_ChannelSetPosition(file->music, BASS_ChannelGetLength(file->music, BASS_POS_BYTE), BASS_POS_BYTE);
			//}
			BASS_CHANNELINFO info;
			string curPos = "--:--";
			string endPos = " --:--";// the space is not a typo, it's for the minus (should i remove the minus?)
			if (BASS_ChannelGetInfo(file->music, &info)) {
				double dCurPos = BASS_ChannelBytes2Seconds(file->music, BASS_ChannelGetPosition(file->music, BASS_POS_BYTE));
				double dEndPos = BASS_ChannelBytes2Seconds(file->music, BASS_ChannelGetLength(file->music, BASS_POS_BYTE)) - dCurPos;
				curPos = helpPlease::intToSomeDigit((int)(dCurPos / 60), 2) + ":" + helpPlease::intToSomeDigit((int)(fmod(dCurPos, 60)), 2);
				endPos = "-" + helpPlease::intToSomeDigit((int)(dEndPos / 60), 2) + ":" + helpPlease::intToSomeDigit((int)(fmod(dEndPos, 60)), 2);
			}
			ImGui::PushFont(Data::monofont);
			ImGui::TextDisabled((curPos + " | " + endPos).c_str());
			ImGui::PopFont();

			nextPos = { 0, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y };
			TopBarHeight = ImGui::GetWindowSize().y;
			float before = editor->hSpan;
			ImGui::SliderFloat("", &editor->hSpan, 0, 1, "");
			double length = BASS_ChannelBytes2Seconds(file->music, BASS_ChannelGetLength(file->music, BASS_POS_BYTE));
			if (editor->hSpan != before) {
				BASS_ChannelPause(file->music);
				BASS_ChannelSetPosition(file->music, BASS_ChannelSeconds2Bytes(file->music, editor->hSpan * length), BASS_POS_BYTE);
			}
			editor->hSpan = BASS_ChannelBytes2Seconds(file->music, BASS_ChannelGetPosition(file->music, BASS_POS_BYTE)) / length;

			ImGui::EndMainMenuBar();
		}
	}

	void MainLayout::DrawToolBar() {
		ImGui::SetNextWindowPos(nextPos);
		if (ImGui::Begin("Tools", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Button(ICON_FA_PEN);
			ImGui::Button(ICON_FA_ERASER);
			ImGui::Button(ICON_FA_EXPAND);
			ImGui::Button(ICON_FA_VOLUME_MUTE);
			SideBarWidth = ImGui::GetWindowSize().x;
			nextPos = { 0, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y };
			nextSize = ImGui::GetWindowSize().x;
			ImGui::End();
		}
	}

	void MainLayout::DrawNotesBar() {
		ImGui::SetNextWindowPos(nextPos);
		ImGui::SetNextWindowSize(ImVec2({ nextSize, ImGui::GetIO().DisplaySize.y - nextPos.y }));
		if (ImGui::Begin("Notes", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Button("Small");
			ImGui::Button("A");
			ImGui::Button("BBB");
			ImGui::Button("prout");
			ImGui::Button("Small");
			ImGui::Button("A");
			ImGui::Button("BBB");
			ImGui::Button("prout");
			ImGui::Button("Small");
			ImGui::Button("A");
			ImGui::Button("BBB");
			ImGui::Button("prout");
			ImGui::Button("Small");
			ImGui::Button("A");
			ImGui::Button("BBB");
			ImGui::Button("prout");
			ImGui::Button("Small");
			ImGui::Button("A");
			ImGui::Button("BBB");
			ImGui::Button("prout");
			ImGui::Button("Small");
			ImGui::Button("A");
			ImGui::Button("BBB");
			ImGui::Button("prout");
			ImGui::Button("Small");
			ImGui::Button("A");
			ImGui::Button("BBB");
			ImGui::Button("prout");
			ImGui::Button("Small");
			ImGui::Button("A");
			ImGui::Button("BBB");
			ImGui::Button("prout");
			ImGui::End();
		}
	}

	void MainLayout::DisplayFileDialogs() {

		if (ImGuiFileDialog::Instance()->Display("OpenFileDlgKey"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				savedPath = ImGuiFileDialog::Instance()->GetFilePathName();
				file->Load(savedPath);
			}
			ImGuiFileDialog::Instance()->Close();
		}

		if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				savedPath = ImGuiFileDialog::Instance()->GetFilePathName();
				file->Save(savedPath);
			}
			ImGuiFileDialog::Instance()->Close();
		}
	}

	void MainLayout::Preferences() {
		if (ImGui::Begin("Preferences", &showPreferences)) {
			float volume = 0;
			BASS_ChannelGetAttribute(file->music, BASS_ATTRIB_VOL, &volume);
			ImGui::SliderFloat(((string)ICON_FA_PLAY + "Music Volume").c_str(), &volume, 0, 1, NULL, ImGuiSliderFlags_None);
			BASS_ChannelSetAttribute(file->music, BASS_ATTRIB_VOL, volume);
			ImGui::End();
		}
	}

	void MainLayout::EditEditorView()
	{
		if (ImGui::Begin("Edit Editor View", &showEditEditorView)) {
			ImGui::InputFloat("H Zoom", &editor->hZoom);
			ImGui::InputFloat("V Zoom", &editor->vZoom);
			ImGui::InputFloat("H Span", &editor->hSpan);
			ImGui::InputFloat("V Span", &editor->vSpan);
			ImGui::End();
		}
	}

	void MainLayout::Kits() {
		if (ImGui::Begin("Kits", &showKits)) {

			static ImGuiTableFlags flags =
				ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg |
				ImGuiTableFlags_BordersOuter |
				ImGuiTableFlags_BordersV;
			ImVec2 outer_size = ImVec2(0.0f, 20 * 8);
			if (ImGui::BeginTable("table_scrolly", 3, flags, outer_size))
			{
				ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
				ImGui::TableSetupColumn("Activate", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn("Version", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				// Demonstrate using clipper for large vertical lists
				ImGuiListClipper clipper;
				clipper.Begin(Data::kits.size());
				while (clipper.Step())
				{
					for (int index = clipper.DisplayStart; index < clipper.DisplayEnd; index++)
					{
						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);
						ImGui::Checkbox(("###Chckbx" + to_string(index)).c_str(), &Data::kits[index].activated);
						ImGui::SameLine();
						if (ImGui::Button((ICON_FA_ARROW_UP "###Up" + to_string(index)).c_str()) && index > 0) {
							swap(Data::kits[index], Data::kits[index - 1]);
						}
						ImGui::SameLine();
						if (ImGui::Button((ICON_FA_ARROW_DOWN "###Down" + to_string(index)).c_str()) && index < Data::kits.size() - 1) {
							swap(Data::kits[index], Data::kits[index + 1]);
						}
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(Data::kits[index].name.c_str());
						ImGui::TableSetColumnIndex(2);
						ImGui::Text(Data::kits[index].version.c_str());
					}
				}
				ImGui::EndTable();
			}

			ImGui::End();
		}
	}

	MainLayout::MainLayout(BMS::MDMFile* mdmfile, Editor* editorInstance) :
		file(mdmfile),
		editor(editorInstance),
		nextSize(0),
		nextPos({0, 0}),
		savedPath(""),
		showChartInfo(false),
		showPreferences(false),
		showEditEditorView(false),
		showDemo(false),
		showKits(false)
	{

	}

	void MainLayout::DrawUI() {
		DrawMainBar();
		DrawToolBar();
		DrawNotesBar();
		DisplayFileDialogs();
		//windows
		if (showChartInfo)
			ChartInfo();
		if (showPreferences)
			Preferences();
		if (showEditEditorView)
			EditEditorView();
		if (showDemo) {
			ImGui::ShowDemoWindow();
		}
		if (showKits) {
			Kits();
		}
	}
}