# Fortnite External

External process, RPM read-only, mouse via SendInput, offsets from your dump.

## Build via Visual Studio Code

### Pré-requisito (Windows)

1. Instale o **Visual Studio 2022** (ou **Build Tools 2022**) com a workload
   **Desktop development with C++**. O `cl.exe` e `MSBuild.exe` vem com isso.
2. Instale o extension **C/C++** da Microsoft no VS Code
   (ext: `ms-vscode.cpptools`).
3. Abra esta pasta no VS Code (`File > Open Folder...`).

### Build + Run (mais rápido)

No VS Code, abra o terminal (`Ctrl+` `~`) e rode:

```bat
build.bat
```

O script procura o `MSBuild.exe`, builda Release x64 e roda o exe.
A saída fica em `build\fortnite-external.exe`.

### Build via task do VS Code

1. `Ctrl+Shift+P` -> `Tasks: Run Build Task` (ou `Ctrl+Shift+B`).
2. A task `build` executa `build.bat`.
3. Depois, para rodar: `Ctrl+F5` (ou `Run and Debug` panel -> `Run` green button).

### Build via command line (sem VS Code)

```bat
"C:\Program Files\Microsoft Visual Studio\2022\<EDITION>\MSBuild\Current\Bin\MSBuild.exe" fortnite-external.vcxproj /t:Build /p:Configuration=Release /p:Platform=x64 /m
```

O `<EDITION>` é `Community`, `Professional`, `Enterprise` ou `BuildTools`.

### Debug

`Ctrl+F5` roda com o launch config em `.vscode/launch.json`.
O exe abre em console integrado. Tem breakpoints por arrastação se quiser
debugar o loop principal em `src/main.cpp`.

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

Se o jogo ler 0 de tudo (attach ok, mas 0 entities / 0 camera),
regen essas duas offsets com Dumper-7 ou Cheat Engine pointer scan.

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
- `build.bat` — build script (searches MSBuild, builds Release x64, runs exe)
- `.vscode/` — VS Code config (tasks, launch, IntelliSense)

## Notes

- `kVisibleOnly` is false: external has no line-trace without extra SDK calls.
- Original pasted project was an internal DLL (`divinity-internal`, DynamicLibrary,
  hooks, oxorany, vibecode-ui). This repo ships the external conversion per
  default technique. Internal files are not built here.
- Run in a VM if you test hooks against system processes.
