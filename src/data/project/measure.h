#ifndef MDME_DATA_PROJECT_MEASURE_
#define MDME_DATA_PROJECT_MEASURE_

#include <map>
#include "channel.h"

namespace data {
	namespace project {
		class Measure {
		public:
			std::map<int, Channel*> channels;
			Measure *pPreviousMeasure;
			Measure *pNextMeasure;

			// call getChannel()
			Channel* operator[](int id);

			bool hasChannel(int x);
			float getTimeSignature();
			float getTimeDuration();
		};
	}
}

#endif // !MDME_DATA_PROJECT_MEASURE_