#include <Windows.h>
#include <cstdio>
#include "config.h"
#include "memory.h"
#include "entity.h"
#include "aim.h"
#include "overlay.h"
#include "closehandle.h"
#include "StealthyOpenProcess.h"

static void SaveConfig(float fovPx, float smoothing, bool aimOn, bool espOn) {
    FILE* f = nullptr;
    if (fopen_s(&f, cfg::kConfigFile, "w") != 0 || !f) return;
    fprintf(f, "{\n  \"fovPx\": %.1f,\n  \"smoothing\": %.1f,\n  \"aim\": %s,\n  \"esp\": %s\n}\n",
        fovPx, smoothing, aimOn ? "true" : "false", espOn ? "true" : "false");
    fclose(f);
}

static void LoadConfig(float& fovPx, float& smoothing, bool& aimOn, bool& espOn) {
    FILE* f = nullptr;
    if (fopen_s(&f, cfg::kConfigFile, "r") != 0 || !f) return;
    char buf[512]{};
    size_t n = fread(buf, 1, sizeof(buf) - 1, f);
    fclose(f);
    buf[n] = 0;
    float v = 0;
    if (sscanf_s(buf, "\"fovPx\": %f", &v) == 1) fovPx = v;
    if (sscanf_s(buf, "\"smoothing\": %f", &v) == 1) smoothing = v;
    if (strstr(buf, "\"aim\": true")) aimOn = true;
    if (strstr(buf, "\"aim\": false")) aimOn = false;
    if (strstr(buf, "\"esp\": true")) espOn = true;
    if (strstr(buf, "\"esp\": false")) espOn = false;
}

int main() {
    StealthyOpenProcess::Initialize();
    StartHandleMonitor();

    float fovPx = cfg::kFovPx;
    float smoothing = cfg::kSmoothing;
    bool aimOn = cfg::kAimEnabled;
    bool espOn = cfg::kEspEnabled;
    LoadConfig(fovPx, smoothing, aimOn, espOn);

    printf("Fortnite External - waiting for %s...\n", cfg::kProcessName);
    printf("INSERT menu | END exit | Hold RMB aim\n");

    Memory mem;
    while (!mem.Attach(cfg::kProcessName)) {
        if (GetAsyncKeyState(cfg::kExitKey) & 1) { StopHandleMonitor(); StealthyOpenProcess::Shutdown(); return 0; }
        Sleep(500);
    }
    printf("Attached pid=%lu base=0x%llx\n", mem.Pid(), (unsigned long long)mem.ModuleBase());

    Overlay ov;
    if (!ov.Create()) { mem.Detach(); StopHandleMonitor(); return 1; }

    World world(mem);
    bool menuOpen = false;
    bool prevMenu = false, prevExit = false;

    for (;;) {
        // Hotkeys
        const bool menuDown = (GetAsyncKeyState(cfg::kMenuKey) & 0x8000) != 0;
        if (menuDown && !prevMenu) { menuOpen = !menuOpen; ov.SetClickThrough(!menuOpen); }
        prevMenu = menuDown;
        const bool exitDown = (GetAsyncKeyState(cfg::kExitKey) & 0x8000) != 0;
        if (exitDown && !prevExit) break;
        prevExit = exitDown;

        if (!mem.IsAttached()) {
            if (!mem.Attach(cfg::kProcessName)) { Sleep(cfg::kTickMs); continue; }
        }

        Camera cam{};
        if (!world.GetCamera(cam)) { Sleep(cfg::kTickMs); continue; }

        const int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        auto ents = world.GetEntities(cam);

        // ESP
        if (espOn) ov.Draw(ents, cam, cfg::kEspBox, cfg::kEspSkeleton, cfg::kEspHealth, cfg::kEspDistance, cfg::kEspSnapline, cfg::kMaxDistance);
        if (aimOn) ov.DrawFov(fovPx);
        ov.DrawMenu(menuOpen, ov.Tab(), fovPx, smoothing, aimOn, espOn);

        // Aim (held)
        if (aimOn && (GetAsyncKeyState(cfg::kAimKey) & 0x8000)) {
            uint8_t localTeam = 0;
            world.GetLocalTeam(localTeam);
            Entity target{};
            if (PickTarget(ents, cam, w, h, fovPx, localTeam, cfg::kTeamCheck, target))
                AimTick(target, cam, w, h, smoothing);
        }

        Sleep(cfg::kTickMs);
    }

    SaveConfig(fovPx, smoothing, aimOn, espOn);
    ov.Destroy();
    mem.Detach();
    StopHandleMonitor();
    StealthyOpenProcess::Shutdown();
    return 0;
}
