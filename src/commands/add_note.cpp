#include "add_note.h"
#include "../helpers/bms.cpp"

namespace commands {
	AddNote::AddNote(CHANNEL* chan, int bpmeas, float beat, string note) :
		m_chan(chan), m_bpmeas(bpmeas), m_beat(beat), m_note(note)
	{
		float bpi = helpers::bms::get_bpi(m_bpmeas, m_beat);

		helpers::bms::chan_adapt(bpi, m_chan);

		//save old value (should be 0 i guess)
		m_note_old = (*m_chan)[m_beat * bpi];
	}

	void AddNote::redo() {

		float bpi = helpers::bms::get_bpi(m_bpmeas, m_beat);

		helpers::bms::chan_adapt(bpi, m_chan);

		// add the new note
		(*m_chan)[m_beat * bpi] = m_note;
	}

	void AddNote::undo() {

		float bpi = helpers::bms::get_bpi(m_bpmeas, m_beat);

		helpers::bms::chan_adapt(bpi, m_chan);

		// bring back the old value
		(*m_chan)[m_beat * bpi] = m_note_old;
	}
	void AddNote::dispose()
	{
	}
}