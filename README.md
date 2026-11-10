# Fortnite External

External process, RPM read-only, mouse via SendInput, offsets via Dumper-7.

## Stack

- C++17, MSVC v143, x64, Windows SDK 10.0
- No injection, no writes to game memory
- Overlay: transparent topmost Win32 (GDI), click-through when menu closed
- Aim: screen-distance target pick, delta / smoothing per tick via SendInput

## Build

Open `fortnite-external.vcxproj` in Visual Studio 2022, Release x64, Build.
Output: `builds\fortnite-external.exe`

## Offsets

Edit `src/offsets.h`. All values are `// FILL:` placeholders.

Regen recipe:

1. Run Dumper-7 against `FortniteClient-Win64-Shipping.exe`
2. Paste `GWorld`, `GNames`, `OwningGameInstance`, `LocalPlayers`,
   `PlayerController`, `AcknowledgedPawn`, `PlayerCameraManager`,
   `RootComponent`, `Health`, `TeamIndex`, `Mesh`, `BoneArray`
3. Rebuild

Until offsets are filled the tool attaches but reads return 0.

## Config

Edit `src/config.h` (`constexpr` block at top). Runtime menu:

- `INSERT` toggle menu (tabs: Aim / ESP / Misc / Config)
- `END` exit (saves `config.json`)
- Hold `RMB` aim (held, not toggle)

## Files

- `src/main.cpp` — attach + poll loop (`Sleep(kTickMs)`, default 4ms)
- `src/memory.h/.cpp` — OpenProcess + ReadProcessMemory, StealthyOpenProcess first
- `src/entity.h/.cpp` — GWorld walk, camera, bone read
- `src/aim.h/.cpp` — FOV gate + SendInput mouse delta
- `src/overlay.h/.cpp` — ESP boxes / skeleton / health / distance + menu
- `src/StealthyOpenProcess.h/.cpp` — user attachment (HWBP VEH, kernel32 callstack)
- `src/closehandle.h/.cpp` — user attachment (unknown-exit handle cleanup, standalone)
- `dependencies/XorStr.hpp` — minimal shim so StealthyOpenProcess compiles

## Notes

- `kVisibleOnly` is false: external has no line-trace without extra SDK calls.
- Original pasted project was an internal DLL (`divinity-internal`, DynamicLibrary,
  hooks, oxorany, vibecode-ui). This repo ships the external conversion per
  default technique. Internal files are not built here.
- Run in a VM if you test hooks against system processes.
