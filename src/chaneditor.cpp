#include "chaneditor.h"
#include "ui.h"
#include "data.h"
#include "SDL2/SDL_ttf.h"

int WorldToScreen(float fWorld, float span, float zoom) {
	return (int)((fWorld - span) * zoom);
}

int ChanEditor::f_x;
int ChanEditor::f_y;
int ChanEditor::f_w;
int ChanEditor::f_h = 50;
float ChanEditor::f_zoom = 50;
float ChanEditor::f_span;
float ChanEditor::f_bpxsize;

ChanEditor::ChanEditor(const char* name, const char* channel_id, SDL_Renderer* renderer):
	m_name((char*)name),
	m_type(ChannelType::NormalChannel),
	m_renderer(renderer),
	m_channelIds(),
	m_currentChan(),
	m_beatcounter(),
	m_texture(nullptr),
	m_txX(),
	m_txY()
{
	if (Kit::allchannels.find(channel_id) != Kit::allchannels.end())
		m_type = Kit::allchannels[channel_id]->type;
	m_channelIds.push_back((char*)channel_id);
}

bool ChanEditor::Begin(bool is_first) {
	// set the starting point for the first channel editor and the width
	if (is_first) {
		f_x = ui::MainLayout::SideBarWidth;
		f_y = ui::MainLayout::TopBarHeight;
		f_w = (int)ImGui::GetIO().DisplaySize.x - f_x;
	}
	else { //increase the pos y for next channel
		f_y += f_h;
	}

	// screen size changed or textures weren't created
	if (m_txX != f_w || m_txY != f_h || m_texture == nullptr) {
		m_txX = f_w;
		m_txY = f_h;

		if (m_texture != nullptr)
			SDL_DestroyTexture(m_texture);

		m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_txX, m_txY);
		cout << "Texture of channel editor \"" << m_name << "\" updated." << endl;
		if (m_texture == NULL)
		{
			printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
		}

		SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
	}

	SDL_SetRenderTarget(m_renderer, m_texture);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 50);
	SDL_RenderClear(m_renderer);

	m_slidermem = map<string, float>();
	m_beatcounter = 0;

	return true;
}

bool ChanEditor::AddChannel(char* channel_id) {
	for (auto chan = m_channelIds.begin(); chan != m_channelIds.end(); chan++) {
		//check if id was already added
		if (*chan == channel_id)
			return false;
		//check if id is declared in kits -- shouldn't happen
		if (Kit::allchannels.find(*chan) == Kit::allchannels.end())
			return false;
		//check if id is the same type as the channel editor
		if (Kit::allchannels[*chan]->type != m_type)
			return false;
	}
	//channel id wasn't present, we can add it
	m_channelIds.push_back(channel_id);
	return true;
}

bool ChanEditor::DrawMeasure(NOTES notes, char* channel_id, int time_sign) {

	// place a separator at the beginning of the measure
	int pos = WorldToScreen(m_beatcounter, f_span, f_zoom);
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 180);
	SDL_RenderDrawLine(m_renderer, pos, 0, pos, ChanEditor::f_h);

	//draw beatsnaps
	int beatsnapadd = WorldToScreen(1.f / float(Editor::beatsnap), 0, f_zoom);
	for (int i = 0; i < time_sign * Editor::beatsnap; i++) {
		SDL_RenderDrawLine(m_renderer, pos + beatsnapadd*i, 0, pos + beatsnapadd * i, ChanEditor::f_h);
	}

	// skip if there's no notes
	if (notes.size() <= 0) {
		m_beatcounter += time_sign;
		return true;
	}

	float incbeat = (float)time_sign / (float)notes.size();

	for (auto note = notes.begin(); note != notes.end(); note++) {

		if (m_type == ChannelType::NormalChannel) {
			// check if note exist and is referenced by kits
			if (*note != "00" && Kit::allnotes.find(*note) != Kit::allnotes.end()) {
				if (Kit::allnotes[*note]->type == NoteType::Slider) {
					if (m_slidermem.find(*note) == m_slidermem.end())
						// a slider start was placed, memorizing it
						m_slidermem.insert({ *note, m_beatcounter });
					else {

						// draw slider bar
						SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 100);
						int oldnotepos = WorldToScreen(m_slidermem[*note], f_span, f_zoom);
						int newnotepos = WorldToScreen(m_beatcounter, f_span, f_zoom);
						SDL_Rect rect = SDL_Rect();
						rect.x = oldnotepos;
						rect.y = (f_h / 2) - 3;
						rect.w = newnotepos - oldnotepos;
						rect.h = 6;
						SDL_RenderFillRect(m_renderer, &rect);

						//reset memory
						m_slidermem.erase(*note);
					}
				}
				SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 255);
				int notepos = WorldToScreen(m_beatcounter, f_span, f_zoom);
				SDL_Rect rect = SDL_Rect();
				rect.x = notepos - 5;
				rect.y = (f_h / 2) - 5;
				rect.w = 10;
				rect.h = 10;
				SDL_RenderFillRect(m_renderer, &rect);
			}
		}
		else if (m_type == ChannelType::Events) {
			if (*note != "00" && Kit::allevents.find(*note) != Kit::allevents.end()) {
				SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
				int notepos = WorldToScreen(m_beatcounter, f_span, f_zoom);
				SDL_Rect rect = SDL_Rect();
				rect.x = notepos - 5;
				rect.y = (f_h / 2) - 5;
				rect.w = 10;
				rect.h = 10;
				SDL_RenderFillRect(m_renderer, &rect);
			}
		}
		m_beatcounter += incbeat;
	}
	return true;
}

void ChanEditor::End(int rank) {
	SDL_SetRenderTarget(m_renderer, NULL);
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

	SDL_Rect src; {
		src.x = 0;
		src.y = 0;
		src.w = f_w;
		src.h = f_h;
	}
	SDL_Rect dst; {
		dst.x = f_x;
		dst.y = f_y;
		dst.w = f_w;
		dst.h = f_h;
	}
	SDL_RenderCopy(m_renderer, m_texture, &src, &dst);
	// draw borders around channel editor
	SDL_SetRenderDrawColor(m_renderer, 80, 80, 80, 128);
	dst.h++;
	dst.w++;
	SDL_RenderDrawRect(m_renderer, &dst);
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
}

ChanEditor::~ChanEditor()
{
	SDL_DestroyTexture(m_texture);
	delete m_name;
}
