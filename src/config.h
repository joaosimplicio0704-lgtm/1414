#pragma once
// Config block — one place to tune. Edit here, rebuild, done.
#include <Windows.h>

namespace cfg {
// Hotkeys (held = only while key down, toggle = press once)
inline constexpr int kAimKey = VK_RBUTTON;   // held
inline constexpr int kMenuKey = VK_INSERT;   // toggle menu
inline constexpr int kExitKey = VK_END;      // exit

// Aim
inline constexpr float kFovPx = 120.0f;      // FOV circle radius in px
inline constexpr float kSmoothing = 5.0f;    // divide delta by this per tick
inline constexpr int kHeadBone = 68;         // Fortnite head bone id (Dumper-7)
inline constexpr bool kTeamCheck = true;     // skip same team
inline constexpr bool kVisibleOnly = false;  // no external line-trace; keep false
inline constexpr bool kAimEnabled = true;
inline constexpr bool kEspEnabled = true;

// ESP
inline constexpr bool kEspBox = true;
inline constexpr bool kEspSkeleton = true;
inline constexpr bool kEspHealth = true;
inline constexpr bool kEspDistance = true;
inline constexpr bool kEspSnapline = false;
inline constexpr float kMaxDistance = 300.0f; // meters, 0 = unlimited

// Loop
inline constexpr int kTickMs = 4;            // poll interval
inline constexpr const char* kProcessName = "FortniteClient-Win64-Shipping.exe";
inline constexpr const char* kConfigFile = "config.json";
} // namespace cfg
