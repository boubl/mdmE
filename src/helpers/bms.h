#pragma once
#include "../bms.h"

namespace helpers {
	namespace bms {
		float getBeatsPerIndex(int beatPerMeasure, float beat);
		void adaptChannel(float beatsPerIndex, CHANNEL* channel);
	}
}