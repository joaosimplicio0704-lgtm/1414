#pragma once
// Fortnite (Unreal Engine) offsets — from user dump (xerozify.xyz/offsets).
// Values pasted as-is. Missing ones stay FILL — code compiles, reads return 0.
#include <cstdint>

namespace off {
// Class
inline constexpr uintptr_t GWorld = 0x1C8E1578; // UWORLD
inline constexpr uintptr_t GNames = 0x0; // FILL: not in dump
inline constexpr uintptr_t GEngine = 0x1C8E2F48; // gEngine (unused)

// UWorld
inline constexpr uintptr_t OwningGameInstance = 0x248; // World::OwningGameInstance
inline constexpr uintptr_t PersistentLevel = 0x40;     // World::PersistentLevel

// UGameInstance
inline constexpr uintptr_t LocalPlayers = 0x38; // GameInstance::LocalPlayers (TArray)

// ULocalPlayer
inline constexpr uintptr_t PlayerController = 0x30; // FILL: not in dump, typical 0x30, verify via CE

// APlayerController
inline constexpr uintptr_t AcknowledgedPawn = 0x320;   // PlayerController::AcknowledgedPawn
inline constexpr uintptr_t PlayerCameraManager = 0x330; // PlayerController::PlayerCameraManager

// APlayerCameraManager
inline constexpr uintptr_t CameraCache = 0x15A0; // PlayerCameraManager::CameraCachePrivate

// ULevel
inline constexpr uintptr_t Actors = 0xA0; // FILL: not in dump, typical 0xA0, verify via CE

// AActor
inline constexpr uintptr_t RootComponent = 0x1B0; // Actor::RootComponent

// USceneComponent
inline constexpr uintptr_t RelativeLocation = 0x140; // SceneComponent::RelativeLocation (FVector)

// ACharacter
inline constexpr uintptr_t Mesh = 0x2F8; // Character::Mesh (USkeletalMeshComponent)

// AFortPawn -> FortHealthSet
inline constexpr uintptr_t HealthSet = 0x1430; // FortPawn::HealthSet
inline constexpr uintptr_t Health = 0x48;    // FortHealthSet::Health (float)
inline constexpr uintptr_t MaxHealth = 0x70; // FortHealthSet::MaxHealth (float)

// APawn -> FortPlayerStateAthena
inline constexpr uintptr_t PlayerState = 0x298; // Pawn::PlayerState
inline constexpr uintptr_t TeamIndex = 0xF81;   // FortPlayerStateAthena::TeamIndex (uint8)

// USkeletalMeshComponent
inline constexpr uintptr_t BoneArray = 0x9B8; // SkeletalMeshComponent::CachedBoneSpaceTransforms (TArray<FMatrix>)
// FMatrix stride 0x40, translation = WPlane (m[3][0..2])
inline constexpr int kBoneStride = 0x40;

// FCameraCacheEntry + FMinimalViewInfo
inline constexpr uintptr_t CameraPOV = 0x10; // CameraCacheEntry::POV
inline constexpr uintptr_t POVLocation = 0x0;  // MinimalViewInfo::Location (FVector)
inline constexpr uintptr_t POVRotation = 0x18; // MinimalViewInfo::Rotation (FRotator)
inline constexpr uintptr_t POVFOV = 0x30;      // MinimalViewInfo::FOV (float)
} // namespace off
