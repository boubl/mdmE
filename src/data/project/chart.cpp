#include "chart.h"

namespace data {
	namespace project {
		Chart::Chart() {
			bpm = 128.f;
			scene = "";
			designer = "";
			difficulty = 0;
			measures = std::vector<Measure*>();
		}
	}
}