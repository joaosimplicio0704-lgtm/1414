# Fortnite External (DLL Injection)

DLL injetada no processo do Fortnite + injetor standalone.
Leitura direta de memória (sem RPM), aim via SendInput, offsets da sua dump.

## Estrutura do projeto

- **fortnite-external.dll** — DLL injetada no Fortnite. O cheat roda dentro
  do processo do jogo, com acesso direto à memória (sem OpenProcess/RPM).
- **injector.exe** — injetor standalone. Aloca memória no Fortnite, escreve
  o path da DLL e cria um remote thread que chama LoadLibraryA.

## Build via Visual Studio Code

### Pré-requisito (Windows)

1. Instale o **Visual Studio 2022** (ou **Build Tools 2022**) com a workload
   **Desktop development with C++**. O `cl.exe` e `MSBuild.exe` vem com isso.
2. Instale o extension **C/C++** da Microsoft no VS Code
   (ext: `ms-vscode.cpptools`).
3. Abra esta pasta no VS Code (`File > Open Folder...`).

### Build (mais rápido)

No VS Code, abra o terminal (`Ctrl+` `~`) e rode:

```bat
build.bat
```

O script procura o `MSBuild.exe`, builda Release x64 da solução
(`fortnite-external.sln`) e produz:

- `build\fortnite-external.dll`
- `build\injector.exe`

### Build via task do VS Code

1. `Ctrl+Shift+P` -> `Tasks: Run Build Task` (ou `Ctrl+Shift+B`).
2. A task `build` executa `build.bat`.

### Build via command line (sem VS Code)

```bat
"C:\Program Files\Microsoft Visual Studio\2022\<EDITION>\MSBuild\Current\Bin\MSBuild.exe" fortnite-external.sln /t:Build /p:Configuration=Release /p:Platform=x64 /m
```

O `<EDITION>` é `Community`, `Professional`, `Enterprise` ou `BuildTools`.

## Como usar

1. Abra o Fortnite (em janela, não fullscreen).
2. No terminal/VS Code, rode o injetor:

```bat
build\injector.exe build\fortnite-external.dll FortniteClient-Win64-Shipping.exe
```

3. O injetor procura o processo do Fortnite, aloca memória, escreve o path
   da DLL e cria um remote thread que carrega a DLL no processo do jogo.
4. No jogo:
   - `INSERT` — menu (tabs: Aim / ESP / Misc / Config)
   - `END` — sai (salva `config.json`)
   - Segurar `RMB` — aim

## Offsets

`src/offsets.h` já vem com os valores da sua dump (Dumper-7 JSON).
Todos os 28 offsets verificados, zero FILL.

| Symbol | Offset | Source |
|---|---|---|
| GWorld (UWORLD) | 0x1C8E1578 | Class (absolute address) |
| gEngine | 0x1C8E2F48 | Class (unused) |
| World::OwningGameInstance | 0x248 | World |
| World::PersistentLevel | 0x40 | World |
| World::Levels | 0x1E8 | World |
| World::GameState | 0x1D0 | World |
| GameInstance::LocalPlayers | 0x38 | GameInstance |
| LocalPlayer::ViewportClient | 0x78 | LocalPlayer |
| GameViewportClient::World | 0x78 | GameViewportClient |
| GameState::PlayerArray | 0x290 | GameState |
| PlayerState::PawnPrivate | 0x2F0 | PlayerState |
| Pawn::Controller | 0x2A8 | Pawn |
| PlayerController::AcknowledgedPawn | 0x320 | PlayerController |
| PlayerController::PlayerCameraManager | 0x330 | PlayerController |
| PlayerCameraManager::CameraCachePrivate | 0x15A0 | PlayerCameraManager |
| CameraCacheEntry::POV | 0x10 | CameraCache |
| MinimalViewInfo::Location | 0x0 | MinimalViewInfo |
| MinimalViewInfo::Rotation | 0x18 | MinimalViewInfo |
| MinimalViewInfo::FOV | 0x30 | MinimalViewInfo |
| Level::ActorCluster | 0xE0 | Level |
| FLevelActorContainer::Actors | 0x8 | FLevelActorContainer |
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

## Config

Edit `src/config.h` (`constexpr` block at top). Runtime menu:

- `INSERT` toggle menu (tabs: Aim / ESP / Misc / Config)
- `END` exit (saves `config.json`)
- Hold `RMB` aim (held, not toggle)

## Files

- `src/dllmain.cpp` — DllMain entry, starts CheatThread on attach
- `src/cheat_loop.cpp/.h` — cheat loop running inside the Fortnite process
- `src/internal.h` — InternalMemory: direct memory reads (no RPM)
- `src/entity.cpp/.h` — GWorld walk (absolute address), camera, bone read
- `src/aim.cpp/.h` — FOV gate + SendInput mouse delta
- `src/overlay.cpp/.h` — ESP boxes / skeleton / health / distance + menu
- `src/closehandle.cpp/.h` — unknown-exit handle cleanup (standalone)
- `src/injector.cpp` — VirtualAllocEx + WriteProcessMemory + CreateRemoteThread
- `src/config.h` — constexpr config block (hotkeys, fov, smoothing, ESP toggles)
- `src/offsets.h` — all offsets from Dumper-7 JSON
- `src/sdk.h` — FVector/FRotator/FMatrix/TArray + WorldToScreen
- `fortnite-external.vcxproj` — builds the DLL (DynamicLibrary)
- `injector.vcxproj` — builds the injector console app
- `fortnite-external.sln` — solution with both projects
- `build.bat` — build script (searches MSBuild, builds Release x64)
- `.vscode/` — VS Code config (tasks, launch, IntelliSense)

## Como a injeção funciona

1. `injector.exe` encontra o PID do `FortniteClient-Win64-Shipping.exe`.
2. Abre um handle com `PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ`.
3. `VirtualAllocEx` aloca memória no processo do Fortnite.
4. `WriteProcessMemory` escreve o path absoluto da DLL nessa memória.
5. `CreateRemoteThread` cria um thread no Fortnite que chama `LoadLibraryA`
   com o path da DLL.
6. A DLL é carregada no processo do Fortnite, `DllMain` dispara e inicia
   a `CheatThread`.
7. A cheat thread lê a memória diretamente (sem RPM), desenha o overlay e
   faz o aim via SendInput.

## Notas

- `kVisibleOnly` é false: sem line-trace externa sem SDK extra.
- `main.cpp`, `memory.cpp`, `StealthyOpenProcess.cpp` ainda existem no repo
  mas não são compilados pela DLL — são o caminho antigo de attach externo.
- O overlay é uma janela separada (GDI), não requer hooks no jogo.
