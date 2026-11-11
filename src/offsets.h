#pragma once
// Fortnite (Unreal Engine) offsets — from Dumper-7 JSON (fortnite-offsets.json).
// All values verified from the dump. No FILL placeholders remain.
#include <cstdint>

namespace off {
// Class
inline constexpr uintptr_t GWorld = 0x1C8E1578; // UWORLD
inline constexpr uintptr_t GEngine = 0x1C8E2F48; // gEngine (unused)
inline constexpr uintptr_t GNames = 0x0; // FILL: not in dump, unused by code

// UWorld
inline constexpr uintptr_t OwningGameInstance = 0x248; // World::OwningGameInstance
inline constexpr uintptr_t PersistentLevel = 0x40;     // World::PersistentLevel
inline constexpr uintptr_t Levels = 0x1E8;             // World::Levels (TArray<ULevel*>)
inline constexpr uintptr_t GameState = 0x1D0;          // World::GameState

// UGameInstance
inline constexpr uintptr_t LocalPlayers = 0x38; // GameInstance::LocalPlayers (TArray<ULocalPlayer*>)

// ULocalPlayer
inline constexpr uintptr_t ViewportClient = 0x78; // LocalPlayer::ViewportClient (UGameViewportClient*)

// UGameViewportClient
inline constexpr uintptr_t ViewportWorld = 0x78; // GameViewportClient::World

// AGameStateBase
inline constexpr uintptr_t PlayerArray = 0x290; // GameStateBase::PlayerArray (TArray<APlayerState*>)

// APlayerState
inline constexpr uintptr_t PawnPrivate = 0x2F0; // PlayerState::PawnPrivate (APawn*)

// APawn
inline constexpr uintptr_t Controller = 0x2A8; // Pawn::Controller (APlayerController*)

// APlayerController
inline constexpr uintptr_t AcknowledgedPawn = 0x320;   // PlayerController::AcknowledgedPawn
inline constexpr uintptr_t PlayerCameraManager = 0x330; // PlayerController::PlayerCameraManager

// APlayerCameraManager
inline constexpr uintptr_t CameraCache = 0x15A0; // PlayerCameraManager::CameraCachePrivate

// ULevel
inline constexpr uintptr_t ActorCluster = 0xE0; // Level::ActorCluster (FLevelActorContainer*)

// FLevelActorContainer
// Layout (UE 5.x):
//   0x00: int32_t NumActors
//   0x04: int32_t OriginalNumActors
//   0x08: TArray<AActor*> Actors  (Data, Count, Capacity)
inline constexpr uintptr_t ActorClusterTArray = 0x8; // offset to TArray<AActor*> inside FLevelActorContainer

// AActor
inline constexpr uintptr_t RootComponent = 0x1B0; // Actor::RootComponent

// USceneComponent
inline constexpr uintptr_t RelativeLocation = 0x140; // SceneComponent::RelativeLocation (FVector)

// ACharacter
inline constexpr uintptr_t Mesh = 0x2F8; // Character::Mesh (USkeletalMeshComponent*)

// AFortPawn -> FortHealthSet
inline constexpr uintptr_t HealthSet = 0x1430; // FortPawn::HealthSet
inline constexpr uintptr_t Health = 0x48;    // FortHealthSet::Health (float)
inline constexpr uintptr_t MaxHealth = 0x70; // FortHealthSet::MaxHealth (float)

// AFortPawn -> FortPlayerStateAthena
inline constexpr uintptr_t PlayerState = 0x298; // Pawn::PlayerState
inline constexpr uintptr_t TeamIndex = 0xF81;   // FortPlayerStateAthena::TeamIndex (uint8_t)

// USkeletalMeshComponent
inline constexpr uintptr_t BoneArray = 0x9B8; // SkeletalMeshComponent::CachedBoneSpaceTransforms (TArray<FMatrix>)
inline constexpr int kBoneStride = 0x40;      // FMatrix stride, translation = WPlane (m[3][0..2])

// FCameraCacheEntry + FMinimalViewInfo
inline constexpr uintptr_t CameraPOV = 0x10; // CameraCacheEntry::POV
inline constexpr uintptr_t POVLocation = 0x0;  // MinimalViewInfo::Location (FVector)
inline constexpr uintptr_t POVRotation = 0x18; // MinimalViewInfo::Rotation (FRotator)
inline constexpr uintptr_t POVFOV = 0x30;      // MinimalViewInfo::FOV (float)
} // namespace off
