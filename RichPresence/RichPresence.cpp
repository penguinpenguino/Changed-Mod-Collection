#include "headers/discord.h"
#include "headers/ChangedLib.h"
#include <iostream>
#include <memory>
#include <ctime>
#include <stdlib.h>

long int start;

struct DiscordState {
    discord::User currentUser;
    std::unique_ptr<discord::Core> core;
};

extern "C" {

DiscordState state{};
discord::Core* core{};

char* State = "";
char* Details = "Looking at the title screen";
bool Update = true;
int MapID = NONE;

void OnUpdate() {
    state.core->RunCallbacks();
    if (Update) {
        discord::Activity activity{};
        activity.SetDetails(Details);
        activity.GetAssets().SetSmallImage("");
        activity.GetAssets().SetSmallText("");
        activity.GetAssets().SetLargeText("");
        activity.GetAssets().SetLargeImage("largeicon");
        activity.SetState(State);
        activity.GetTimestamps().SetStart(start);
        activity.SetType(discord::ActivityType::Playing);
        state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
            std::cout << ((result == discord::Result::Ok) ? "" : "[RichPresence]: Failed to update activity status\n");
        });
        Update = false;
    }
}

DWORD WINAPI MainThread(void* args) {
    std::cout << "[RichPresence]: Loaded!" << std::endl;
    int LocalMapID;
    while (1) {
        LocalMapID = GetCurrentMap();
        if (MapID != LocalMapID) {
            MapID = LocalMapID;
            if ((strcmp(Details, "Getting wet") == 0 ||
                strcmp(Details, "Fighting the Tiger Shark") == 0 ||
                strcmp(Details, "Fighting the Squid Dog") == 0) &&
                MapID < POOL_5 + 0x20 &&
                MapID > POOL_1
            ) {
                if (MapID == GENERATOR) {
                    Details = "Turning the main generator";
                    State = "on";
                }
            } else if (
                MapID == LAB_ROOM_2 ||
                MapID == HALLWAY_3
            ) {
                Details = "Still waking up";
                State = "";
            } else if (MapID == CAT_ROOM) {
                Details = "Running from a cat";
                State = "";
            } else if (
                MapID == LIGHT_ROOM_1 ||
                MapID == LIGHT_ROOM_2 ||
                MapID == LIGHT_ROOM_3 ||
                MapID == LIGHT_ROOM_4
            ) {
                Details = "Tiptoeing around Dark Latex";
                State = "creatures";
            } else if (
                MapID == DARK_LATEX_1 ||
                MapID == DARK_LATEX_CRYSTALS_2 ||
                MapID == DARK_LATEX_CRYSTALS_3 ||
                MapID == DARK_LATEX_CRYSTALS_4
            ) {
                Details = "Exploring the Dark Latex zone";
                State = "";
            } else if (
                MapID == DARK_LATEX_NEST_2 ||
                MapID == DARK_LATEX_NEST_3 ||
                MapID == DARK_LATEX_2
            ) {
                Details = "In the Dark Latex's nest";
                State = "";
            } else if (
                MapID == THE_ELDER_ONE &&
                strcmp(Details, "In prison") != 0
            ) {
                Details = "Fighting The Elder One";
                State = "";
            } else if (
                MapID == LIBRARY_2 ||
                MapID == LIBRARY_3 ||
                MapID == LIBRARY_4 ||
                MapID == LIBRARY_BALCONY
            ) {
                Details = "Exploring the library";
                State = "";
            } else if (
                MapID == LIBRARY_PURO ||
                MapID == VENT_PURO ||
                MapID == STORAGE_PURO
            ) {
                Details = "Talking to Puro";
                State = "";
            } else if (MapID == VENT_3) {
                Details = "In the vents";
                State = "";
            } else if (
                MapID == STORAGE ||
                MapID == STORAGE_MAZE
            ) {
                Details = "In the storage rooms";
                State = "";
            } else if (MapID == GAS) {
                Details = "Holding their breath";
                State = "";
            } else if (
                MapID == POOL_2 ||
                MapID == POOL_4 ||
                MapID == POOL_5 ||
                MapID == POOL_HALLWAY
            ) {
                Details = "Getting wet";
                State = "(in the pool)";
            } else if (MapID == POOL_TIGER_SHARK) {
                Details = "Fighting the Tiger Shark";
                State = "";
            } else if (MapID == POOL_SQUID_DOG) {
                Details = "Fighting the Squid Dog";
                State = "";
            } else if (MapID == GENERATOR) {
                Details = "Turning the main generator";
                State = "on";
            } else if (
                MapID == OVERGROWN_BALCONY ||
                MapID == OVERGROWN_BALCONY_2
            ) {
                Details = "Getting a breath of fresh";
                State = "air";
            } else if (
                MapID == WHITE_LATEX_1_1 ||
                MapID == WHITE_LATEX_1_2 ||
                MapID == WHITE_LATEX_1_3 ||
                MapID == WHITE_LATEX_1_4 
            ) {
                Details = "In the White Latex's nest";
                State = "";
            } else if (
                MapID == PRISON_1 ||
                MapID == PRISON_2
            ) {
                Details = "In prison";
                State = "";
            } else if (MapID == TILED_LAB_4) {
                Details = "Solving a puzzle";
                State = "";
            } else if (
                MapID == GROUND_FLOOR_HALL &&
                strcmp(Details, "Exploring the Dark Latex zone") == 0
            ) {
                Details = "Making a big decision";
                State = "";
            } else if (MapID == ENDING_GOOD) {
                Details = "Got the good ending";
                State = "";
            } else if (MapID == CREDITS) {
                Details = "In the credits";
                State = "";
            }
            Update = true;
        }
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