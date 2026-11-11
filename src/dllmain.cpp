#include <Windows.h>
#include "cheat_loop.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        HANDLE t = CreateThread(nullptr, 0, CheatThread, nullptr, 0, nullptr);
        if (t) CloseHandle(t);
        break;
    case DLL_PROCESS_DETACH:
        StopCheat();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
