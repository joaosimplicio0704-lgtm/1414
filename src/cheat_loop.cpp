#include <Windows.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "config.h"
#include "internal.h"
#include "entity.h"
#include "aim.h"
#include "overlay.h"

static volatile bool g_running = true;

void StopCheat() { g_running = false; }
bool IsCheatRunning() { return g_running; }

static void Log(const char* fmt, ...) {
    char buf[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    OutputDebugStringA(buf);
}

DWORD WINAPI CheatThread(LPVOID param) {
    InternalMemory mem;

    Log("Fortnite Internal - injetado!\n");
    Log("INSERT menu | END exit | Hold RMB aim\n");

    Overlay ov;
    if (!ov.Create()) {
        Log("Overlay creation failed.\n");
        return 1;
    }

    World world(mem);
    bool menuOpen = false;
    bool prevMenu = false, prevExit = false;

    float fovPx = cfg::kFovPx;
    float smoothing = cfg::kSmoothing;
    bool aimOn = cfg::kAimEnabled;
    bool espOn = cfg::kEspEnabled;

    while (g_running) {
        const bool menuDown = (GetAsyncKeyState(cfg::kMenuKey) & 0x8000) != 0;
        if (menuDown && !prevMenu) { menuOpen = !menuOpen; ov.SetClickThrough(!menuOpen); }
        prevMenu = menuDown;
        const bool exitDown = (GetAsyncKeyState(cfg::kExitKey) & 0x8000) != 0;
        if (exitDown && !prevExit) break;
        prevExit = exitDown;

        Camera cam{};
        if (!world.GetCamera(cam)) { Sleep(cfg::kTickMs); continue; }

        const int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        auto ents = world.GetEntities(cam);

        if (espOn) ov.Draw(ents, cam, cfg::kEspBox, cfg::kEspSkeleton, cfg::kEspHealth, cfg::kEspDistance, cfg::kEspSnapline, cfg::kMaxDistance);
        if (aimOn) ov.DrawFov(fovPx);
        ov.DrawMenu(menuOpen, ov.Tab(), fovPx, smoothing, aimOn, espOn);

        if (aimOn && (GetAsyncKeyState(cfg::kAimKey) & 0x8000)) {
            uint8_t localTeam = 0;
            world.GetLocalTeam(localTeam);
            Entity target{};
            if (PickTarget(ents, cam, w, h, fovPx, localTeam, cfg::kTeamCheck, target))
                AimTick(target, cam, w, h, smoothing);
        }

        Sleep(cfg::kTickMs);
    }

    ov.Destroy();
    Log("Cheat thread saindo.\n");
    return 0;
}
