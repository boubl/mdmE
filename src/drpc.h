#include <discord/discord.h>
#include <memory>
#include <string>

class DRpc {
private:
	static std::unique_ptr<discord::Core> core;
	static bool isinit;
public:
	static bool Init();
	static void ChangeChartname(std::string name);
	static void RunCallbacks();
};