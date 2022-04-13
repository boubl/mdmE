#include "editor.h"
#include "ui.h"
#include "data.h"
#include <map>
using namespace BMS;

int WorldToScreen(float fWorld, float span, float zoom) {
	return (int)((fWorld - span) * zoom);
}

float ScreenToWorld(int nScreen, float span, float zoom) {
	return ((float)nScreen / zoom) + span;
}

Editor::Editor(SDL_Renderer* drenderer, BMS::MDMFile* dfile) :
	renderer(drenderer),
	file(dfile),
	layers(vector<int>()),
	vSpan(0),
	vZoom(0),
	hSpan(0),
	hZoom(100),
	nextwindowX(0),
	nextwindowY(0),
	nextwindowW(0),
	nextwindowH(0)
{

}

void Editor::Begin()
{
	nextwindowX = ui::MainLayout::SideBarWidth;
	nextwindowY = ui::MainLayout::TopBarHeight;
	nextwindowW = (int)ImGui::GetIO().DisplaySize.x - nextwindowX;
	nextwindowH = (int)ImGui::GetIO().DisplaySize.y - nextwindowY;
}

int Editor::BeatCount(int rank) {
	double length = BASS_ChannelBytes2Seconds(file->music, BASS_ChannelGetLength(file->music, BASS_POS_BYTE));
	vector<MEASURE>* measures = &file->charts[rank];
	float beatCount = 0;
	int measureCount = 0;
	while (beatCount * ((1 / file->bpm[rank]) * 60) < length) {
		float beatsPM = 4;
		if (measureCount < measures->size()) {
			if (measures->at(measureCount).find("02") != measures->at(measureCount).end()) {
				beatsPM *= stof(measures->at(measureCount).at("02")[0]);
			}
		}
		beatCount += beatsPM;
		measureCount++;
	}
	return beatCount;
}

void Editor::DisplayBeatLayer(int rank)
{
	double length = BASS_ChannelBytes2Seconds(file->music, BASS_ChannelGetLength(file->music, BASS_POS_BYTE));
	vector<MEASURE>* measures = &file->charts[rank];
	float beatCount = BeatCount(rank);
	//Create uninitialized texture
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, nextwindowW, nextwindowH);
	if (texture == NULL)
	{
		printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
	}
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	
	float lastPos = 0;
	float span = hSpan * beatCount;
	int measureCount = 0;
	while (lastPos * ((1 / file->bpm[rank])*60) < length) {
		float beatsPM = 4; // not bpm
		if (measureCount < measures->size()) {
			if (measures->at(measureCount).find("02") != measures->at(measureCount).end()) {
				beatsPM *= stof(measures->at(measureCount).at("02")[0]);
			}
		}
		lastPos += beatsPM;
		int pos = WorldToScreen(lastPos, span, hZoom);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(renderer, pos + (nextwindowW / 2), 0, pos + (nextwindowW / 2), 300);

		if (measureCount < measures->size()) {
			for (auto chan = measures->at(measureCount).begin(); chan != measures->at(measureCount).end(); chan++) {
				for (auto kit = Data::kits.begin(); kit != Data::kits.end(); kit++) {
					
				}
			}

			if (measures->at(measureCount).find("13") != measures->at(measureCount).end()) {
				CHANNEL* chan = &measures->at(measureCount).at("13");
				float basepos = lastPos - beatsPM;
				float incpos = beatsPM / chan->size();
				for (int i = 0; i < chan->size(); i++) {
					int subpos = WorldToScreen(basepos + incpos * i, span, hZoom);
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
					if (chan->at(i) != "00") {
						SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
						SDL_Rect rect = SDL_Rect();
						rect.x = (WorldToScreen((basepos + incpos * i), span, hZoom) - 5) + (nextwindowW / 2);
						rect.y = (300 / 4) - 5;
						rect.w = 10;
						rect.h = 10;
						SDL_RenderFillRect(renderer, &rect);
					}
				}
			}
		}

		measureCount++;
	}

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127);

	SDL_RenderDrawLine(renderer, nextwindowW / 2, 0, nextwindowW / 2, 300);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderCopy(renderer, texture, new SDL_Rect({ 0, 0, nextwindowW, nextwindowH }), new SDL_Rect({ nextwindowX, nextwindowY, nextwindowW, nextwindowH }));
	SDL_DestroyTexture(texture);
}
