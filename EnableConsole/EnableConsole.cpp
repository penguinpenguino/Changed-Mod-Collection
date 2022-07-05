#include "ChangedLib.h"
#include <Windows.h>
#include <iostream>

extern "C" {

ModMain void mod_main() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);
}

}