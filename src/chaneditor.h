#include "kit.h"
#include <SDL2/SDL.h>
#define NOTES vector<string>

#pragma once

class ChanEditor {
public:

	static int f_x;
	static int f_y;
	static int f_w;
	static int f_h;
	static float f_zoom;
	static float f_span;
	static float f_bpxsize;

public:
	char* m_name;
	ChannelType m_type;
	vector<char*> m_channelIds;
	int m_currentChan;
	SDL_Texture* m_texture;
	int m_txX;
	int m_txY;
	SDL_Renderer* m_renderer;

	map<string, float> m_slidermem;
	float m_beatcounter;
public:
	// create a Channel Editor instance
	ChanEditor(const char* name, const char* channel_id, SDL_Renderer* renderer);

	~ChanEditor();
	// add a channel to edit to the chaneditor
	bool AddChannel(char* channel_id);
	// reset draw surface
	bool Begin(bool is_first);
	// draw all notes of a given measure
	bool DrawMeasure(NOTES notes, char* channel_id, int time_sign = 4);
	// idk
	void End(int rank);
};