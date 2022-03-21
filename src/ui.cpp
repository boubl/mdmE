#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include <string>
#include "IconsFontAwesome5.h"
#include "bms.h"
#include <ImGuiFileDialog/ImGuiFileDialog.h>

namespace ui {
	class MainLayout {
private:
		ImVec2 nextPos;
		float nextSize;
		BMS::MDMFile* file;
		string savedPath = "";

		//windows
		bool showChartInfo = false;

		void ChartInfo() {
			if (ImGui::Begin("Chart info", &showChartInfo)) {
				ImGui::InputText("Name", &file->name);
				ImGui::InputText("Author", &file->author);
				ImGui::InputText("BPM", &file->bpmInfo);
				ImGui::End();
			}
		}

		void DrawMainBar() {
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
					ImGui::MenuItem("Preferences");
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("View")) {
					ImGui::MenuItem("WARIO", "Alt+F4");
					ImGui::MenuItem("FABIO", "Ctrl+Alt+Suppr");
					ImGui::EndMenu();
				}
				ImGui::Separator();
				ImGui::MenuItem(ICON_FA_PLAY);
				ImGui::MenuItem(ICON_FA_PLAY_CIRCLE);
				ImGui::MenuItem(ICON_FA_STOP);

				nextPos = { 0, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y };
				ImGui::EndMainMenuBar();
			}
		}

		void DrawToolBar() {
			ImGui::SetNextWindowPos(nextPos);
			if (ImGui::Begin("Tools", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Button("Brush");
				ImGui::Button("Eraser");
				ImGui::Button("Select");
				ImGui::Button("Mute");
				nextPos = { 0, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y };
				nextSize = ImGui::GetWindowSize().x;
				ImGui::End();
			}
		}

		void DrawNotesBar() {
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

		void DisplayFileDialogs() {

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

	public:
		MainLayout(BMS::MDMFile* mdmfile) : file(mdmfile), nextSize(0), nextPos({ 0, 0 }) {

		}

		void DrawUI() {
			DrawMainBar();
			DrawToolBar();
			DrawNotesBar();
			DisplayFileDialogs();
			//windows
			if (showChartInfo)
				ChartInfo();
		}
	};
}