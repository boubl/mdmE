#include "ui.h"
#include "audio.h"
#include "data.h"
#include <math.h>

namespace ui {
	int MainLayout::SideBarWidth = 0;
	int MainLayout::TopBarHeight = 0;

	void MainLayout::ChartInfo() {
		if (ImGui::Begin("Chart info", &showChartInfo)) {
			ImGui::Text("General:");
			ImGui::InputText("Song title", &file->name);
			ImGui::InputText("Song Author", &file->author);
			ImGui::InputText("BPM description", &file->bpmInfo);

			ImGui::InputFloat("BPM", &file->bpm[1]);
			string preview = file->scene[1]; // set the preview to the id if not found in kits
			if (Kit::allscenes.find(file->scene[1]) != Kit::allscenes.end())
				preview = Kit::allscenes[file->scene[1]];
			if (ImGui::BeginCombo("Scene", preview.c_str(), 0))
			{
				for (auto scene = Kit::allscenes.begin(); scene != Kit::allscenes.end(); scene++)
				{
					const bool is_selected = (file->scene[1] == scene->first);
					if (ImGui::Selectable(scene->second.c_str(), is_selected))
						file->scene[1] = scene->first;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::InputText("Designer", &file->designers[1]);
			ImGui::InputInt("Star difficulty", &file->difficulties[1]);

			ImGui::End();
		}
	}

	static bool musicwasplaying = false;
	static bool sliderwasclicked = false;
	void MainLayout::DrawMainBar() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				char const* lFilterPatterns[1] = { "*.mdm", };
				if (ImGui::MenuItem("New...", "Ctrl+N")) {
					savedPath = "";
					file->~MDMFile();
					*file = BMS::MDMFile();
				}
				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					char* result = tinyfd_openFileDialog("Open...", NULL, 1, lFilterPatterns, "MuseDash map file", 0);
					if (result != NULL) {
						savedPath = result;
						file->Load(savedPath);
					}
				}
				if (ImGui::BeginMenu("Open recent")) {
					ImGui::TextDisabled("No recent files");
					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					if (savedPath == "") {
						char* result = tinyfd_saveFileDialog("Save", NULL, 1, lFilterPatterns, "MuseDash map file");
						if (result != NULL) {
							savedPath = result;
							file->Load(savedPath);
						}
					}
					else
						file->Save(savedPath);
				}
				if (ImGui::MenuItem("Save as...", "Ctrl+S")) {
					char* result = tinyfd_saveFileDialog("Save as...", NULL, 1, lFilterPatterns, "MuseDash map file");
					if (result != NULL) {
						savedPath = result;
						file->Load(savedPath);
					}
				}

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
				if (ImGui::MenuItem("FABIO", "Lanzoni"))
					SDL_OpenURL("https://realboubli.github.io/");
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_FA_FAST_BACKWARD)) {
				BASS_ChannelPause(file->music);
				BASS_ChannelSetPosition(file->music, 0, BASS_POS_BYTE);
			}
			if (ImGui::MenuItem(ICON_FA_PLAY)) {
				BASS_ChannelPlay(file->music, false);
				cout << BASS_ErrorGetCode() << endl;
			}
			if (ImGui::MenuItem(ICON_FA_PAUSE)) {
				BASS_ChannelPause(file->music);
			}
			if (ImGui::MenuItem(ICON_FA_FAST_FORWARD)) {
				BASS_ChannelPause(file->music);
				BASS_ChannelSetPosition(file->music, BASS_ChannelGetLength(file->music, BASS_POS_BYTE), BASS_POS_BYTE);
			}

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
			TopBarHeight = (int)ImGui::GetWindowSize().y;

			// normalized song position
			float normalpos = Audio::GetSongPos() / Audio::GetSongLength();
			ImGui::SliderFloat("##SongSlider", &normalpos, 0, 1, "");
			// the second condition is to fix a bug where sometimes ImGui will not detect the slider editing state correctly
			if (ImGui::IsItemEdited() || (sliderwasclicked && ImGui::IsMouseDown(ImGuiMouseButton_Left))) {
				if (!sliderwasclicked)
					musicwasplaying = Audio::IsSongPlaying();
				sliderwasclicked = true;
				Audio::SetSongPos(normalpos * Audio::GetSongLength());
				Audio::PauseSong();
			}
			else {
				if (sliderwasclicked && musicwasplaying) {
					Audio::PlaySong();
				}
				sliderwasclicked = false;
			}

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
			SideBarWidth = (int)ImGui::GetWindowSize().x;
			nextPos = { 0, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y };
			nextSize = ImGui::GetWindowSize().x;
			ImGui::End();
		}
	}

	void MainLayout::DrawNotesBar() {
		ImGui::SetNextWindowPos(nextPos);
		ImGui::SetNextWindowSize(ImVec2({ nextSize, ImGui::GetIO().DisplaySize.y - nextPos.y }));
		if (ImGui::Begin("Notes", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
			for (auto note = Kit::allnotes.begin(); note != Kit::allnotes.end(); note++) {
				if (note->first == Editor::selectednote) {
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(66.f / 255.f, 150.f / 250.f, 250.f / 255.f, 1.f));
					if (ImGui::Button(note->first.c_str()))
						Audio::PlaySingleSFX(note->second->sound);
					ImGui::PopStyleColor();
				}
				else {
					if (ImGui::Button(note->first.c_str())) {
						Editor::selectednote = note->first;
						Audio::PlaySingleSFX(note->second->sound);
					}
				}
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text(note->second->name.c_str());
					if (!note->second->sound.empty())
						ImGui::TextDisabled(note->second->sound.c_str());
					ImGui::EndTooltip();
				}
			}
		}
		ImGui::End();
	}

	void MainLayout::Preferences() {
		if (ImGui::Begin("Preferences", &showPreferences)) {
			float volume = 0;
			BASS_ChannelGetAttribute(file->music, BASS_ATTRIB_VOL, &volume);
			ImGui::SliderFloat(((string)ICON_FA_PLAY + "Music Volume").c_str(), &volume, 0, 1, NULL, ImGuiSliderFlags_None);
			BASS_ChannelSetAttribute(file->music, BASS_ATTRIB_VOL, volume);
		}
		ImGui::End();
	}

	void MainLayout::EditEditorView()
	{
		if (ImGui::Begin("Edit Editor View", &showEditEditorView)) {
			int span = ChanEditor::f_span;
			ImGui::InputInt("Span", &span);
			ChanEditor::f_span = span;
			int w_zoom = ChanEditor::f_zoom;
			ImGui::InputInt("W Zoom", &w_zoom);
			ChanEditor::f_zoom = w_zoom;
			int h_zoom = ChanEditor::f_h;
			ImGui::InputInt("H Zoom", &h_zoom);
			ChanEditor::f_h = h_zoom;
		}
		ImGui::End();
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
				clipper.Begin((int)Data::kits.size());
				while (clipper.Step())
				{
					for (int index = clipper.DisplayStart; index < clipper.DisplayEnd; index++)
					{
						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);
						if (ImGui::Checkbox(("###Chckbx" + to_string(index)).c_str(), &Data::kits[index].activated)) {
							Data::ReloadKitsData();
						}
						ImGui::SameLine();
						if (ImGui::Button((ICON_FA_ARROW_UP "###Up" + to_string(index)).c_str()) && index > 0) {
							swap(Data::kits[index], Data::kits[index - 1]);
							Data::ReloadKitsData();
						}
						ImGui::SameLine();
						if (ImGui::Button((ICON_FA_ARROW_DOWN "###Down" + to_string(index)).c_str()) && index < Data::kits.size() - 1) {
							swap(Data::kits[index], Data::kits[index + 1]);
							Data::ReloadKitsData();
						}
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(Data::kits[index].name.c_str());
						ImGui::TableSetColumnIndex(2);
						ImGui::Text(Data::kits[index].version.c_str());
					}
				}
				ImGui::EndTable();
			}
		}
		ImGui::End();
	}

	MainLayout::MainLayout(BMS::MDMFile* mdmfile) :
		file(mdmfile),
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