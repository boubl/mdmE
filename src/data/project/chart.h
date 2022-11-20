#ifndef MDME_DATA_PROJECT_CHART_
#define MDME_DATA_PROJECT_CHART_

#include <vector>
#include <string>

#include "measure.h"

namespace data {
	namespace project {
		class Chart {
		public:
			// Beats per minute
			float bpm;
			// Scene string id
			std::string scene;
			// Designer of the map (overload project's levelDesigner)
			std::string designer;
			// Difficulty of the map, usually from 1 to 11. 0 is unknown;
			int difficulty;
			// Measures
			std::vector<Measure*> measures;

			Chart();
			~Chart();
		};
	}
}

#endif // !MDME_DATA_CHART_