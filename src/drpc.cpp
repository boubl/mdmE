#include "drpc.h"
#include <iostream>
#include <chrono>

bool DRpc::isinit = false;
std::unique_ptr<discord::Core> DRpc::core = nullptr;

bool DRpc::Init()
{
    discord::Core* coreptr{};
    discord::Core::Create(946195082992840704, DiscordCreateFlags_NoRequireDiscord, &coreptr);
    core.reset(coreptr);
	return !core;
}

void DRpc::ChangeChartname(std::string name)
{
    if (!isinit)
        return;

    discord::Activity activity{};
    activity.SetDetails("Charting...");
    activity.SetState(("Editing " + name).c_str());
    activity.GetAssets().SetLargeImage("melon");
    activity.GetAssets().SetLargeText("This is a watermelon");
    activity.GetTimestamps().SetStart(discord::Timestamp(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()));
    activity.SetType(discord::ActivityType::Playing);
    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed") << " updating activity!\n";
    });
}

void DRpc::RunCallbacks()
{
    if (isinit)
        core->RunCallbacks();
}
