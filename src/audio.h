#include <string>
#include <map>

#include <miniaudio.h>

#pragma once

class Audio {
public:
	static bool Init();

	static bool IsSongLoaded();

	static bool IsSongPlaying();
	// play/unpause song
	static void PlaySong();
	// pause song
	static void PauseSong();
	// get song position in seconds
	static double GetSongPos();
	// get song length in seconds
	static double GetSongLength();
	// get song remaining time in seconds
	static double GetSongRTime();
	// set song position in seconds, return true if the operation succeed
	static bool SetSongPos(double position);

	static void PlaySFXs();

	static void PlaySingleSFX(std::string name);

	static void ReloadSamples();

	static void FreeSamples();

	static Audio instance;

	Audio();

private:
	HSTREAM* music;
	std::map<std::string, HSAMPLE> samples;
	// chanId, beatpos
	std::map<std::string, float> lastplayedsfx;
};