#ifndef MDME_DATA_PROJECT_CHANNEL_
#define MDME_DATA_PROJECT_CHANNEL_

#include <map>

#include "measure.h"
#include "note.h"

namespace data {
	namespace project {
		class Channel {
		public:
			int id;
			std::vector<Note*> notes;

			Channel();
			Channel(int id);

			void getBeatsPerIndex();
			void adaptChannel(float beatsPerIndex);
			Note* operator[](int i);

		private:
			Measure mMeasure;
		};
	}
}

#endif // !MDME_DATA_PROJECT_CHANNEL_