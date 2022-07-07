#include "headers/discord.h"
#include "headers/ChangedLib.h"
#include <iostream>
#include <memory>
#include <ctime>

long int start;

struct DiscordState {
    discord::User currentUser;
    std::unique_ptr<discord::Core> core;
};

extern "C" {

DiscordState state{};
discord::Core* core{};
int i = 100;

void OnUpdate() {
    state.core->RunCallbacks();
    if (i < 100) { i++; return; }
    else { i = 0; }
    char* newState = "Exploring";
    if (InTitleScreen()) { newState = "Looking at the title screen"; }
    discord::Activity activity{};
    activity.SetDetails("");
    activity.GetAssets().SetSmallImage("");
    activity.GetAssets().SetSmallText("");
    activity.GetAssets().SetLargeText("");
    activity.GetAssets().SetLargeImage("largeicon");
    activity.SetState(newState);
    activity.GetTimestamps().SetStart(start);
    activity.SetType(discord::ActivityType::Playing);
    state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        std::cout << "[RichPresence]: " << ((result == discord::Result::Ok) ? "Updated activity status" : "Failed to update activity status") << " (" << (int)result << ")" << std::endl;
    });
}

DWORD WINAPI MainThread(void* args) {
    std::cout << "[RichPresence]: Loaded!" << std::endl;
    while (1) {
        CallFuncFromMainThread(OnUpdate);
        Sleep(100);
    }
}

ModMain void ChangedRichPresence() {
    std::cout << "[RichPresence]: Loading RichPresence..." << std::endl;
    start = static_cast<long int>(time(NULL));
    auto result = discord::Core::Create(991894951971528774, DiscordCreateFlags_NoRequireDiscord, &core);
    state.core.reset(core);
    if (!state.core) {
        std::cout << "[RichPresence]: Fatal! Error code: " << (int)result << std::endl;
        return;
    }
    state.core->SetLogHook(
    discord::LogLevel::Debug, [](discord::LogLevel level, const char* message) {
        std::cerr << "Log(" << static_cast<uint32_t>(level) << "): " << message << "\n";
    });
    state.core->ActivityManager().RegisterSteam(814540);
    CreateThread(0, 0, MainThread, 0, 0, 0);
}

}