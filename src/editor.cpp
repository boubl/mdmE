#include "editor.h"
#include "ui.h"
#include "data.h"
#include "chaneditor.h"
#include "kit.h"
#include "audio.h"

Editor Editor::instance;
string Editor::selectednote;
int Editor::beatsnap;

int WorldToSScreen(float fWorld, float span, float zoom) {
	return (int)((fWorld - span) * zoom);
}

float ScreenToWorld(int nScreen, float span, float zoom) {
	return ((float)nScreen / zoom) + span;
}

Editor::Editor() {}

Editor::Editor(SDL_Renderer* rrenderer)
{
	renderer = rrenderer;
	rank = 1;

	//ugly code, i'll remove it later
	channels = vector<ChanEditor*>();
	//channels.push_back(new ChanEditor("Beat per measure", "02", renderer));
	//channels.push_back(new ChanEditor("BPM", "03", renderer));
	channels.push_back(new ChanEditor("Upper", "13", renderer));
	channels.push_back(new ChanEditor("Upper 2", "53", renderer));
	channels.push_back(new ChanEditor("Lower", "14", renderer));
	channels.push_back(new ChanEditor("Lower 2", "54", renderer));
	channels.push_back(new ChanEditor("Events", "15", renderer));
}

bool Editor::Init(SDL_Renderer* renderer) {
	instance.renderer = renderer;
	instance.rank = 1;
	instance.beatsnap = 1;

	//ugly code, i'll remove it later
	instance.channels = vector<ChanEditor*>();
	//channels.push_back(new ChanEditor("Beat per measure", "02", renderer));
	//channels.push_back(new ChanEditor("BPM", "03", renderer));
	instance.channels.push_back(new ChanEditor("Upper", "13", renderer));
	instance.channels.push_back(new ChanEditor("Upper 2", "53", renderer));
	instance.channels.push_back(new ChanEditor("Lower", "14", renderer));
	instance.channels.push_back(new ChanEditor("Lower 2", "54", renderer));
	instance.channels.push_back(new ChanEditor("Events", "15", renderer));

	return true;
}

void Editor::DisplayChannels()
{
	float beatpos = GetBeatPos();

	// offset of the editor (and so the cursor)
	float span = ScreenToWorld(ChanEditor::f_w / 4.f, 0, ChanEditor::f_zoom);
	//apply offset and  set the span to cursor pos
	ChanEditor::f_span = beatpos - span;
	int pos = WorldToSScreen(beatpos, ChanEditor::f_span, ChanEditor::f_zoom);

	bool is_first = true;
	for (auto chanit = instance.channels.begin(); chanit != instance.channels.end(); chanit++) {
		ChanEditor& chan = *(*chanit);

		chan.Begin(is_first);
		float beatcounter = 0;

		for (int i_measure = 0; i_measure < Data::file->charts[instance.rank].size(); i_measure++) {
			int time_sign = 4;

			// get number of beat per measure
			if (Data::file->charts[instance.rank][i_measure].find("02") != Data::file->charts[instance.rank][i_measure].end()) {
				// todo: handle time signature multiplier of zero (cancel it)
				if (Data::file->charts[instance.rank][i_measure]["02"].size() > 0)
					time_sign *= strtof(Data::file->charts[instance.rank][i_measure]["02"][0].c_str(), NULL);
			}

			// handle bpm changes
			// use of is_first because we just want to calculate it once (not for each channel)
			float lastbpmchange = 0;
			if (is_first && Data::file->charts[instance.rank][i_measure].find("03") != Data::file->charts[instance.rank][i_measure].end()) {
				vector<string>& notes = Data::file->charts[instance.rank][i_measure]["03"];

				if (notes.size() > 0) {
					float incbeat = (float)time_sign / (float)notes.size();
					for (auto note = notes.begin(); note != notes.end(); note++) {
						if (*note != "00") {
							instance.bpmchanges.insert({ beatcounter - lastbpmchange, stoi(*note, nullptr, 16) });
							lastbpmchange = beatcounter;
						}
						beatcounter += incbeat;
					}
				}
				else {
					beatcounter += time_sign;
				}
			}
			else {
				beatcounter += time_sign;
			}

			chan.DrawMeasure(Data::file->charts[instance.rank][i_measure][chan.m_channelIds[0]], chan.m_channelIds[0], time_sign);

		}

		is_first = false;

		chan.End(instance.rank);
	}


	// cursor display
	SDL_SetRenderDrawColor(instance.renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(instance.renderer, pos + ChanEditor::f_x, ui::MainLayout::TopBarHeight, pos + ChanEditor::f_x, ui::MainLayout::TopBarHeight + ChanEditor::f_h * instance.channels.size());
}

void Editor::ReloadBPMChanges() {
	float beatcounter = 0;
	float timepos = BASS_ChannelBytes2Seconds(Data::file->music, BASS_ChannelGetPosition(Data::file->music, BASS_POS_BYTE)) / 60.0f;

	for (int i_measure = 0; i_measure < Data::file->charts[instance.rank].size(); i_measure++) {
		int time_sign = 4;

		if (Data::file->charts[instance.rank][i_measure].find("02") != Data::file->charts[instance.rank][i_measure].end()) {
			// todo: handle time signature multiplier of zero (cancel it)
			if (Data::file->charts[instance.rank][i_measure]["02"].size() > 0)
				time_sign *= strtof(Data::file->charts[instance.rank][i_measure]["02"][0].c_str(), NULL);
		}

		float lastbpmchange = 0;
		// channel 03 = bpm changes
		if (Data::file->charts[instance.rank][i_measure].find("03") != Data::file->charts[instance.rank][i_measure].end() && Data::file->charts[instance.rank][i_measure]["03"].size() > 0) {
			vector<string>& notes = Data::file->charts[instance.rank][i_measure]["03"];
			float incbeat = (float)time_sign / (float)notes.size();
			for (auto note = notes.begin(); note != notes.end(); note++) {
				if (*note != "00") {
					instance.bpmchanges.insert({ beatcounter - lastbpmchange, stoi(*note, nullptr, 16) });
					lastbpmchange = beatcounter;
				}
				beatcounter += incbeat;
			}
		}
		else {
			beatcounter += time_sign;
		}
	}
}

float Editor::GetBeatPos() {
	float timepos = Audio::GetSongPos() / 60.f;
	float bpm = Data::file->bpm[instance.rank];

	// this took me 1h30 to make this code functional. i'll never touch it again. fuck bpm changes 
	float beatpos = 0;
	float decount = 0;
	for (auto change = instance.bpmchanges.begin(); change != instance.bpmchanges.end(); change++) {
		if (timepos > decount + (change->first - beatpos) / bpm) {
			decount += (change->first - beatpos) / bpm;
			beatpos = change->first;
			bpm = change->second;
		}
	}
	beatpos += (timepos - decount) * bpm;

	return beatpos;
}

//return -1 if none
int Editor::GetSnapedBeat() {
	ImVec2 mousepos = ImGui::GetMousePos();
	return -1;
}

//return -1 if no channel
int Editor::GetHoveredChannel() {
	ImVec2 mousepos = ImGui::GetMousePos();
	return -1;
}