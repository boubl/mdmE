#pragma once
#include <undo/undo_command.h>
#include "../bms.h"

namespace commands
{
	class AddNote : public undo::UndoCommand
	{
	public:
		AddNote(CHANNEL* chan, int bpmeas, float beat, string note);
		void redo() override;
		void undo() override;
		void dispose() override;

	private:
		int m_measure;
		CHANNEL* m_chan;
		int m_bpmeas;
		float m_beat;
		string m_note;
		string m_note_old;
	};
}