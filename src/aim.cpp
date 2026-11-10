#include "aim.h"
#include <cmath>

bool PickTarget(const std::vector<Entity>& ents, const Camera& cam,
    int screenW, int screenH, float fovPx, uint8_t localTeam, bool teamCheck,
    Entity& out) {
    const float cx = screenW * 0.5f, cy = screenH * 0.5f;
    float best = fovPx;
    bool found = false;
    for (const auto& e : ents) {
        if (!e.valid || e.health <= 0.0f) continue;
        if (teamCheck && e.team == localTeam) continue;
        FVector s{};
        if (!WorldToScreen(e.head, cam.location, cam.rotation, cam.fov, screenW, screenH, s)) continue;
        const float d = std::sqrt((s.x - cx) * (s.x - cx) + (s.y - cy) * (s.y - cy));
        if (d < best) { best = d; out = e; found = true; }
    }
    return found;
}

void SendMouseDelta(float dx, float dy) {
    INPUT in{};
    in.type = INPUT_MOUSE;
    in.mi.dwFlags = MOUSEEVENTF_MOVE;
    in.mi.dx = (LONG)std::lround(dx);
    in.mi.dy = (LONG)std::lround(dy);
    SendInput(1, &in, sizeof(in));
}

void AimTick(const Entity& target, const Camera& cam, int screenW, int screenH, float smoothing) {
    FVector s{};
    if (!WorldToScreen(target.head, cam.location, cam.rotation, cam.fov, screenW, screenH, s)) return;
    const float dx = (s.x - screenW * 0.5f) / smoothing;
    const float dy = (s.y - screenH * 0.5f) / smoothing;
    SendMouseDelta(dx, dy);
}
