#if _WIN64
#error Must be compiled as a 32-bit DLL
#endif

#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>

#define ModMain extern "C" __attribute__((constructor))
#define ModExit extern "C" __attribute__((destructor))



// Forward Declarations

int ModuleMemStart(char* ModuleName);
int ProcessMemStart();
bool InTitleScreen();
bool CallFuncFromMainThread(void(*funcPtr)());
void SetAtTitleScreen(bool AtTitleScreen);



// Variables

int MemStart = ProcessMemStart();



// Misc

/*void ReturnToTitleScreen() { ((void(*)())0x10016ED8)(); }*/

int ModuleMemStart(char* ModuleName) {
    int ret = -1;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
    MODULEENTRY32 entry;
    entry.dwSize = sizeof(entry);
    while (Module32Next(snap, &entry)) {
        if (strcmp(entry.szModule, ModuleName) == 0) { // NAME OF PROCESS
            ret = (int)entry.modBaseAddr;
            break;
        }
    }
    CloseHandle(snap);
    return ret;
}

int ProcessMemStart() { return ModuleMemStart("Game.exe"); }

void SetAtTitleScreen(bool AtTitleScreen) {
    uintptr_t loc = ModuleMemStart("RGSS202E.dll") + 0x17B520;
    *reinterpret_cast<bool*>(loc) = !AtTitleScreen;
}

bool InTitleScreen() {
    uintptr_t loc = ModuleMemStart("RGSS202E.dll") + 0x17B520;
    return !(*reinterpret_cast<bool*>(loc));
}

bool CallFuncFromMainThread(void(*funcPtr)()) {
    LPVOID mem = VirtualAllocEx(GetCurrentProcess(), 0, 1, MEM_COMMIT, PAGE_READWRITE);
    HANDLE thread = CreateRemoteThread(GetCurrentProcess(), NULL, 0, (LPTHREAD_START_ROUTINE)funcPtr, mem, 0, NULL);
    if (thread == NULL) {
        std::cout << "0x" << std::hex << GetLastError() << std::endl;
        return false;
    }
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    VirtualFreeEx(GetCurrentProcess(), mem, 0, MEM_RELEASE);
    return true;
}