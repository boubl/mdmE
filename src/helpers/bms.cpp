#include "../helpers/bms.h"
#include <cmath>
#include "../data.h"

namespace helpers {
	namespace bms {
		float get_bpi(int bpmeas, float beat) {
			// determining how much beats there is in one indice
			int ib = bpmeas; // ib = indice beat
			while (fmod(beat, 1) != 0) {
				ib *= 2;
				beat *= 2;
			}
			while (ib % 2 == 0 && fmod(beat, 2) == 0) {
				ib /= 2;
				beat /= 2;
			}
			float bpi = (float)(bpmeas) / ib;
			return bpi;
		}

		//adapt the size of a channel if it's too small for a certain bpi
		void chan_adapt(float bpi, CHANNEL* chan) {
			while (bpi < 8.f / (float)chan->size()) {
				int size = chan->size();
				int j = 1;
				for (int i = 0; i < size; i++) {
					chan->insert(chan->begin() + j, "00");
					j += 2;
				}
			}
		}

		MEASURE* get_measure(int meas_index) {
			return &Data::file->charts[2][meas_index];
		}

		CHANNEL* get_channel(int meas_index, string chan_index) {
			return &(*get_measure(meas_index))[chan_index];
		}

		string get_note(int meas_index, string chan_index, int note_index) {
			return (* get_channel(meas_index, chan_index))[note_index];
		}

		// be sure to adapt channel's bpi before using
		// return true if the note has been set
		bool set_note(int meas_index, string chan_index, int note_index, string value) {
			CHANNEL* chan = get_channel(meas_index, chan_index);
			if (chan->size() > note_index)
				(*chan)[note_index] = value;
			return chan->size() > note_index;
		}
	}
}