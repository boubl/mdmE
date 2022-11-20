#include "measure.h"

namespace data {
	namespace project {
		bool Measure::hasChannel(int id) {
			return channels.find(id) != channels.end();
		}

		Channel* Measure::operator[](int id) {
			return channels[id];
		}
		float Measure::getTimeSignature() {
			int ts = 0;
			if (hasChannel(2)) {
				ts = strtod(channels[2]->notes[0]->value.c_str(), nullptr, 10) * 4;
			}
			return 0;
		}
		float Measure::getTimeDuration() {
			int ts = getTimeSignature();
			int td = 0;
			if (hasChannel(3)) {
				rd = strtol(channels[3]->notes[0]->value.c_str());
			}
			return 0;
		}
	}
}