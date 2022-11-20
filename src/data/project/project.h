#ifndef MDME_DATA_PROJECT_PROJECT_
#define MDME_DATA_PROJECT_PROJECT_

#include <vector>
#include <string>

#include "chart.h"
#include <bass/bass.h>

namespace data {
	namespace project {
		class Project {
		public:
			// Name of the song.
			std::string name;
			// Name of the song (roman characters).
			std::string nameRomanized;
			// Author of the song.
			std::string author;
			// In info.json file, "bpm" is a string that's more informative than useful.
			// If bpm is a range just write "<min bpm> - <max bpm> (<avg bpm>)".
			std::string bpmInformation;
			// Contain search-friendly designer strings without being visible in-game
			std::string levelDesigner;
			// At which level the chart can be played.
			int unlockLevel;
			// How the hidden chart is activated.
			int hideBmsMode;
			// Rank of the hidden chart (where it will be placed). -1 is the Special rank
			int hideBmsDifficulty;
			// Message displayed when activating the hidden chart.
			std::string hideBmsMessage;
			// Keywords to use in the search box in order to find the chart.
			std::vector<std::string> searchTags;
			// Song handle
			HSTREAM song;
			// Charts in order [Easy, Hard, Master, Hidden].
			Chart* charts[4];

			Project();
			~Project();

			// Load a project file from the given path.
			bool Load(std::string filepath);
			// Save a project file to the given path.
			bool Save(std::string filepath);

		private:
			// Buffer where song data is stored
			void* mSongBuffer;
			// Size in bytes of the song
			size_t mSongSize;
		};

		enum HideMode {
			Click, // Default, the hidden chart will be activated by spam-clicking on the difficulty text.
			Press, // The hidden chart will be activated by pressing and holding on the chart's cover art in the song select.
			Toggle // The custom chart will be activated by switching between all three difficulties back and forth like ouroboros hidden. This requires 3 difficulties.
		};

		enum HideDifficulty {
			// need to check what it does
			Special = -1,
			// need to check what is the default
			Default,
			Easy,
			Hard,
			Master
		};
	}
}

#endif // !MDME_DATA_PROJECT_PROJECT_