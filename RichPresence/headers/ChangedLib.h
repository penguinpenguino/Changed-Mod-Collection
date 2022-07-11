#pragma once

#if _WIN64
#error Must be compiled as a 32-bit DLL
#endif

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>

/* Defines */

#define ModMain extern "C" __attribute__((constructor))
#define ModExit extern "C" __attribute__((destructor))

// Map IDs (Split by cut off points)

#define NONE 0x0
#define STARTING_ROOM 0x5C
#define BALCONY_1 0x58
#define HALLWAY_1 0x61
#define LAB_ROOM_1 0x5A
#define LAB_ROOM_2 0x5F
#define LAB_ROOM_3 0x5E
#define CAT_ROOM 0xFF
#define HALLWAY_2 0x54
#define HALLWAY_3 0x78
#define LIGHT_ROOM_1 0x57
#define LIGHT_ROOM_2 0x70
#define LIGHT_ROOM_3 0x80
#define LIGHT_ROOM_4 0x7E
#define DARK_LATEX_1 0x6E
#define DARK_LATEX_NEST_1 0x7D
#define DARK_LATEX_NEST_2 0xA7
#define DARK_LATEX_NEST_3 0x118
#define THE_ELDER_ONE 0xA5
#define DARK_LATEX_2 0x7B
#define LIBRARY_1 0x56
#define LIBRARY_2 0x55
#define LIBRARY_3 0x79
#define LIBRARY_4 0xA1
#define LIBRARY_5 0x5E
#define LIBRARY_6 0x67
#define LIBRARY_BALCONY 0x73
#define LIBRARY_PURO 0x75
#define MAINTENANCE_1 0x5A
#define MAINTENANCE_2 0x5B
#define MAINTENANCE_3 0x77
#define MAINTENANCE_4 0x5B
#define MAINTENANCE_5 0x53
#define STORAGE 0x68
#define STORAGE_MAZE 0xC1
#define MAINTENANCE_PURO 0x86
#define STORAGE_PURO 0x62
#define STORAGE_HALL 0x54
#define GAS 0x72
#define GAS_2 0x72
#define GAS_PURO 0x5C
#define GAS_HALL 0x53
#define POOL_1 0x5B
#define POOL_2 0x8B
#define POOL_3 0x66
#define POOL_TIGER_SHARK 0x64
#define POOL_4 0x60
#define POOL_SQUID_DOG 0x65
#define POOL_5 0x6D
#define GENERATOR 0x52
#define POOL_HALLWAY 0x98
#define OVERGROWN_BALCONY 0x7F
#define WHITE_LATEX_1_0 0xBF
#define WHITE_LATEX_1_1 0xC2
#define WHITE_LATEX_1_2 0xC3
#define WHITE_LATEX_1_3 0xCC
#define WHITE_LATEX_1_4 0xD0
#define WHITE_LATEX_COMPUTER_ROOM 0x58
#define WHITE_LATEX_2 0x76
#define OVERGROWN_BALCONY_2 0xBF
#define DR_K_OFFICE 0x5A
#define PRISON_1 0x94
#define PRISON_2 0xA8
#define PRISON_GAS 0xA5
#define PRISON_ESCAPE 0x66
#define DR_K_OFFICE_2 0x67
#define DR_K_OFFICE_3 0x5E
#define DR_K_HALLWAY 0x56
#define BALCONY_2 0x53
#define TILED_LAB_1 0x5E
#define TILED_LAB_2 0x5D
#define TILED_LAB_3 0x61
#define TILED_LAB_4 0x63
#define TILED_LAB_5 0x5D
#define WHITE_LATEX_3 0x7D
#define WHITE_LATEX_4 0x76
#define DARK_LATEX_CRYSTALS_1 0x5B
#define DARK_LATEX_CRYSTALS_2 0x18F
#define DARK_LATEX_CRYSTALS_3 0x12C
#define DARK_LATEX_CRYSTALS_4 0xA6
#define DARK_LATEX_CRYSTALS_5 0x58
#define GROUND_FLOOR_HALL 0x5A
#define ENDING 0x51
#define ENDING_STAY 0x5A
#define CREDITS 0xC9


/* Forward Declarations */

int ModuleMemStart(char* ModuleName);
int ProcessMemStart();
bool InTitleScreen();
bool CallFuncFromMainThread(void(*funcPtr)());
int GetCurrentMap();


/* Variables */

int MemStart = ProcessMemStart();


/* Functions */

int GetCurrentMap() {
    if (InTitleScreen()) { return 0; }
    uintptr_t loc = ModuleMemStart("RGSS202E.dll") + 0x17B52C;
    return *reinterpret_cast<int*>(loc);
}

int ModuleMemStart(char* ModuleName) { return (int)GetModuleHandle(ModuleName); }

int ProcessMemStart() { return ModuleMemStart("Game.exe"); }

bool InTitleScreen() {
    uintptr_t loc = ModuleMemStart("RGSS202E.dll") + 0x17B520;
    return !(*reinterpret_cast<bool*>(loc));
}

bool CallFuncFromMainThread(void(*funcPtr)()) {
    LPVOID mem = VirtualAllocEx(GetCurrentProcess(), 0, 1, MEM_COMMIT, PAGE_READWRITE);
    HANDLE thread = CreateRemoteThread(GetCurrentProcess(), NULL, 0, (LPTHREAD_START_ROUTINE)funcPtr, mem, 0, NULL);
    if (thread == NULL) { return false; }
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    VirtualFreeEx(GetCurrentProcess(), mem, 0, MEM_RELEASE);
    return true;
}
