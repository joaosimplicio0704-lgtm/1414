#pragma once
// Transparent topmost overlay (GDI). Click-through when menu closed.
#include <Windows.h>
#include "sdk.h"
#include "entity.h"
#include <vector>

class Overlay {
public:
    bool Create();
    void Destroy();
    void SetClickThrough(bool on);
    void Draw(const std::vector<Entity>& ents, const Camera& cam,
        bool box, bool skeleton, bool health, bool dist, bool snap, float maxDist);
    void DrawFov(float fovPx);
    void DrawMenu(bool open, int tab, float fovPx, float smoothing, bool aimOn, bool espOn);
    bool IsMenuOpen() const { return menuOpen_; }
    void ToggleMenu() { menuOpen_ = !menuOpen_; }
    int Tab() const { return tab_; }
    void SetTab(int t) { tab_ = t; }

private:
    HWND hwnd_ = nullptr;
    HDC hdc_ = nullptr;
    bool menuOpen_ = false;
    int tab_ = 0;
    RECT dragRect_{};
    POINT dragOff_{};
};
