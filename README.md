# Fortnite External

External process, RPM read-only, mouse via SendInput, offsets from your dump.

## Stack

- C++17, MSVC v143, x64, Windows SDK 10.0
- No injection, no writes to game memory
- Overlay: transparent topmost Win32 (GDI), click-through when menu closed
- Aim: screen-distance target pick, delta / smoothing per tick via SendInput

## Build

Open `fortnite-external.vcxproj` in Visual Studio 2022, Release x64, Build.
Output: `build/fortnite-external.exe`

## Offsets

`src/offsets.h` já vem com os valores da sua dump (xerozify.xyz/offsets).
Values used:

| Symbol | Offset | Source |
|---|---|---|
| GWorld (UWORLD) | 0x1C8E1578 | Class |
| gEngine | 0x1C8E2F48 | Class |
| World::OwningGameInstance | 0x248 | World |
| World::PersistentLevel | 0x40 | World |
| GameInstance::LocalPlayers | 0x38 | GameInstance |
| PlayerController::AcknowledgedPawn | 0x320 | PlayerController |
| PlayerController::PlayerCameraManager | 0x330 | PlayerController |
| PlayerCameraManager::CameraCachePrivate | 0x15A0 | PlayerCameraManager |
| MinimalViewInfo::Location | 0x0 | MinimalViewInfo |
| MinimalViewInfo::Rotation | 0x18 | MinimalViewInfo |
| MinimalViewInfo::FOV | 0x30 | MinimalViewInfo |
| Actor::RootComponent | 0x1B0 | Actor |
| SceneComponent::RelativeLocation | 0x140 | SceneComponent |
| Character::Mesh | 0x2F8 | Character |
| FortPawn::HealthSet | 0x1430 | FortPawn |
| FortHealthSet::Health | 0x48 | FortHealthSet |
| FortHealthSet::MaxHealth | 0x70 | FortHealthSet |
| Pawn::PlayerState | 0x298 | Pawn |
| FortPlayerStateAthena::TeamIndex | 0xF81 | FortPlayerStateAthena |
| SkeletalMeshComponent::CachedBoneSpaceTransforms | 0x9B8 | SkeletalMeshComponent |
| FMatrix stride | 0x40 | Matrix |

Still FILL (verify via Cheat Engine / Dumper-7 on your build):

- `ULocalPlayer::PlayerController` — dump não tem, usado 0x30
- `ULevel::Actors` — dump não tem, usado 0x0A0

If the game reads nothing (attach ok, but 0 entities / 0 camera),
regen these two offsets with Dumper-7 or Cheat Engine pointer scan.

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
