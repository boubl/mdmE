#include "audio.h"
#include "editor.h"
#include "data.h"
#include <iostream>

Audio Audio::instance;

bool Audio::Init()
{
	instance.music = &Data::file->music;
	return true;
}

bool Audio::IsSongLoaded()
{

	BASS_CHANNELINFO info;
	return BASS_ChannelGetInfo(*instance.music, &info);
}

bool Audio::IsSongPlaying()
{
	return BASS_ChannelIsActive(*instance.music) == BASS_ACTIVE_PLAYING;
}

void Audio::PlaySong()
{
	BASS_ChannelPlay(*instance.music, false);
}

void Audio::PauseSong()
{
	BASS_ChannelPause(*instance.music);
}

double Audio::GetSongPos()
{
	return BASS_ChannelBytes2Seconds(*instance.music, BASS_ChannelGetPosition(*instance.music, BASS_POS_BYTE));
}

double Audio::GetSongLength()
{
	return BASS_ChannelBytes2Seconds(*instance.music, BASS_ChannelGetLength(*instance.music, BASS_POS_BYTE));
}

double Audio::GetSongRTime()
{
	return GetSongLength() - GetSongPos();
}

bool Audio::SetSongPos(double position)
{
	return BASS_ChannelSetPosition(*instance.music, BASS_ChannelSeconds2Bytes(*instance.music, position), BASS_POS_BYTE);
}

void Audio::PlaySFXs()
{
	if (!Audio::IsSongPlaying())
		return;

	float beatpos = Editor::GetBeatPos();

	float beatcounter = 0;
	for (int i_measure = 0; i_measure < Data::file->charts[Editor::instance.rank].size(); i_measure++) {
		int time_sign = 4;

		if (Data::file->charts[Editor::instance.rank][i_measure].find("02") != Data::file->charts[Editor::instance.rank][i_measure].end()) {
			// todo: handle time signature multiplier of zero (cancel it)
			if (Data::file->charts[Editor::instance.rank][i_measure]["02"].size() > 0)
				time_sign *= strtof(Data::file->charts[Editor::instance.rank][i_measure]["02"][0].c_str(), NULL);
		}

		if (beatcounter < beatpos && beatcounter + time_sign > beatpos) {
			MEASURE& mea = Data::file->charts[Editor::instance.rank][i_measure];

			for (auto chan = Kit::allchannels.begin(); chan != Kit::allchannels.end(); chan++) {
				if (chan->second->type == ChannelType::NormalChannel && mea.find(chan->first) != mea.end() && mea[chan->first].size() > 0) {

					if (instance.lastplayedsfx.find(chan->first) == instance.lastplayedsfx.end() || instance.lastplayedsfx[chan->first] > beatpos)
						instance.lastplayedsfx[chan->first] = -1;

					float incbeat = time_sign / (float)mea[chan->first].size();
					int i_beat = 0;
					for (auto beat = mea[chan->first].begin(); beat != mea[chan->first].end(); beat++) {

						float notepos = beatcounter + incbeat * i_beat;
						float nextnotepos = notepos + incbeat;
						float previousnotepos = notepos - incbeat;

						if ((beatpos > previousnotepos) && (beatpos >= notepos) && (beatpos < nextnotepos) &&
							notepos != instance.lastplayedsfx[chan->first] &&
							Kit::allnotes.find(*beat) != Kit::allnotes.end() &&
							instance.samples.find(Kit::allnotes[*beat]->sound) != instance.samples.end())
						{
							BASS_ChannelPlay(BASS_SampleGetChannel(instance.samples[Kit::allnotes[*beat]->sound], NULL), false);
							instance.lastplayedsfx[chan->first] = notepos;
							cout << "[SFX]:\"" << Kit::allnotes[*beat]->sound << "\" played." << endl;
						}
						i_beat++;
					}
				}
			}
		}
		beatcounter += time_sign;
		
	}
}

void Audio::PlaySingleSFX(std::string name)
{
	if (instance.samples.find(name) != instance.samples.end()) {
		BASS_ChannelPlay(BASS_SampleGetChannel(instance.samples[name], NULL), false);
		cout << "[SFX]:\"" << name << "\" played." << endl;
	}
}

void Audio::ReloadSamples()
{
	FreeSamples();
	for (auto kit = Data::kits.begin(); kit != Data::kits.end(); kit++) {
		for (auto note = kit->allnotes.begin(); note != kit->allnotes.end(); note++) {
			if (!note->second->sound.empty() && instance.samples.find(note->second->sound) == instance.samples.end()) {
				string sampath = kit->dir + note->second->sound;
				HSAMPLE sound = BASS_SampleLoad(false, (void*)sampath.c_str(), 0, 0, 100, NULL);
				if (BASS_ErrorGetCode() == 0) {
					instance.samples.insert({ note->second->sound, sound });
				}
			}
		}
	}
}

void Audio::FreeSamples()
{
	for (auto sample = instance.samples.begin(); sample != instance.samples.end(); sample++) {
		BASS_SampleFree(sample->second);
	}
	instance.samples.clear();
}

Audio::Audio() : music(nullptr)
{
}
