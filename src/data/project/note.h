#ifndef MDME_DATA_PROJECT_NOTE_
#define MDME_DATA_PROJECT_NOTE_

#include <vector>
#include <string>

namespace data {
	namespace project {
		class Note {
		public:
			std::string value;

			Note();
			Note(std::string value);
		};
	}
}

#endif // !MDME_DATA_PROJECT_NOTE_