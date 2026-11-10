#include "overlay.h"
#include <cstdio>
#include <cstring>
#include "config.h"

static LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    return DefWindowProcW(h, m, w, l);
}

bool Overlay::Create() {
    WNDCLASSW wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = L"FortniteOverlay";
    RegisterClassW(&wc);
    hwnd_ = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        wc.lpszClassName, L"Fortnite", WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        nullptr, nullptr, wc.hInstance, nullptr);
    if (!hwnd_) return false;
    ShowWindow(hwnd_, SW_SHOW);
    SetLayeredWindowAttributes(hwnd_, RGB(0, 0, 0), 0, LWA_ALPHA);
    hdc_ = GetDC(hwnd_);
    return hdc_ != nullptr;
}

void Overlay::Destroy() {
    if (hdc_) { ReleaseDC(hwnd_, hdc_); hdc_ = nullptr; }
    if (hwnd_) { DestroyWindow(hwnd_); hwnd_ = nullptr; }
}

void Overlay::SetClickThrough(bool on) {
    if (!hwnd_) return;
    LONG ex = GetWindowLongW(hwnd_, GWL_EXSTYLE);
    if (on) ex |= WS_EX_TRANSPARENT; else ex &= ~WS_EX_TRANSPARENT;
    SetWindowLongW(hwnd_, GWL_EXSTYLE, ex);
}

void Overlay::Draw(const std::vector<Entity>& ents, const Camera& cam,
    bool box, bool skeleton, bool health, bool dist, bool snap, float maxDist) {
    if (!hdc_) return;
    const int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    // Clear
    HBRUSH clear = CreateSolidBrush(RGB(0, 0, 0));
    RECT rc{0, 0, w, h};
    FillRect(hdc_, &rc, clear);
    DeleteObject(clear);
    SetBkMode(hdc_, TRANSPARENT);
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc_, pen);
    for (const auto& e : ents) {
        if (!e.valid) continue;
        if (maxDist > 0.0f && e.distance > maxDist) continue;
        FVector sRoot{}, sHead{};
        if (!WorldToScreen(e.root, cam.location, cam.rotation, cam.fov, w, h, sRoot)) continue;
        if (!WorldToScreen(e.head, cam.location, cam.rotation, cam.fov, w, h, sHead)) continue;
        const float height = sRoot.y - sHead.y;
        if (height < 4.0f || height > 800.0f) continue;
        const float width = height * 0.5f;
        const bool enemy = true; // team filter done by caller if needed
        COLORREF col = enemy ? RGB(255, 60, 60) : RGB(80, 255, 80);
        HPEN cpen = CreatePen(PS_SOLID, 1, col);
        SelectObject(hdc_, cpen);
        if (box) Rectangle(hdc_, (int)(sHead.x - width / 2), (int)sHead.y, (int)(sHead.x + width / 2), (int)sRoot.y);
        if (skeleton) {
            MoveToEx(hdc_, (int)sHead.x, (int)sHead.y, nullptr);
            LineTo(hdc_, (int)sRoot.x, (int)sRoot.y);
        }
        if (snap) {
            MoveToEx(hdc_, w / 2, h, nullptr);
            LineTo(hdc_, (int)sRoot.x, (int)sRoot.y);
        }
        char buf[64]{};
        if (health) {
            snprintf(buf, sizeof(buf), "%.0f", e.health);
            SetTextColor(hdc_, col);
            TextOutA(hdc_, (int)(sHead.x + width / 2 + 4), (int)sHead.y, buf, (int)strlen(buf));
        }
        if (dist) {
            snprintf(buf, sizeof(buf), "%.0fm", e.distance);
            SetTextColor(hdc_, RGB(255, 255, 255));
            TextOutA(hdc_, (int)(sHead.x - width / 2), (int)sRoot.y + 2, buf, (int)strlen(buf));
        }
        DeleteObject(cpen);
    }
    DeleteObject(pen);
}

void Overlay::DrawFov(float fovPx) {
    if (!hdc_) return;
    const int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    HPEN p = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc_, p);
    Ellipse(hdc_, (int)(w / 2 - fovPx), (int)(h / 2 - fovPx), (int)(w / 2 + fovPx), (int)(h / 2 + fovPx));
    DeleteObject(p);
}

void Overlay::DrawMenu(bool open, int tab, float fovPx, float smoothing, bool aimOn, bool espOn) {
    if (!hdc_ || !open) return;
    const int x = 60, y = 60, w = 340, h = 220;
    HBRUSH bg = CreateSolidBrush(RGB(20, 20, 20));
    RECT rc{x, y, x + w, y + h};
    FillRect(hdc_, &rc, bg);
    DeleteObject(bg);
    HPEN p = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc_, p);
    Rectangle(hdc_, x, y, x + w, y + h);
    DeleteObject(p);
    SetTextColor(hdc_, RGB(255, 255, 255));
    TextOutA(hdc_, x + 10, y + 8, "Fortnite External", 17);
    const char* tabs[] = {"Aim", "ESP", "Misc", "Config"};
    for (int i = 0; i < 4; ++i) {
        SetTextColor(hdc_, i == tab ? RGB(0, 255, 255) : RGB(180, 180, 180));
        TextOutA(hdc_, x + 10 + i * 70, y + 30, tabs[i], (int)strlen(tabs[i]));
    }
    char buf[128]{};
    if (tab == 0) {
        snprintf(buf, sizeof(buf), "Aim: %s  FOV: %.0f  Smooth: %.1f", aimOn ? "ON" : "OFF", fovPx, smoothing);
        SetTextColor(hdc_, RGB(255, 255, 255));
        TextOutA(hdc_, x + 10, y + 60, buf, (int)strlen(buf));
        TextOutA(hdc_, x + 10, y + 80, "Hold RMB to aim. INSERT menu, END exit.", 38);
    } else if (tab == 1) {
        snprintf(buf, sizeof(buf), "ESP: %s", espOn ? "ON" : "OFF");
        SetTextColor(hdc_, RGB(255, 255, 255));
        TextOutA(hdc_, x + 10, y + 60, buf, (int)strlen(buf));
    } else if (tab == 3) {
        TextOutA(hdc_, x + 10, y + 60, "Config saved to config.json on exit.", 36);
    }
}
