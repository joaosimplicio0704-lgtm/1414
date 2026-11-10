#pragma once
// Fortnite (Unreal Engine) offsets — Dumper-7 as dep.
// Regen: run Dumper-7 against FortniteClient-Win64-Shipping.exe,
// paste GWorld / GNames / OwningGameInstance / LocalPlayers /
// PlayerController / AcknowledgedPawn / PlayerCameraManager /
// RootComponent / Health / TeamIndex / Mesh / BoneArray here.
// Until then these are FILL placeholders — code compiles, reads return 0.
#include <cstdint>

namespace off {
// FILL: Dumper-7 GWorld (pattern: 48 8B 05 ?? ?? ?? ?? 48 8B 88 ?? ?? ?? ??)
// dump: Dumper-7 -> SDK.hpp -> GWorld
inline constexpr uintptr_t GWorld = 0x0; // FILL: GWorld
// FILL: Dumper-7 GNames
inline constexpr uintptr_t GNames = 0x0; // FILL: GNames

// UWorld
inline constexpr uintptr_t OwningGameInstance = 0x0; // FILL: UWorld::OwningGameInstance
inline constexpr uintptr_t PersistentLevel = 0x0;    // FILL: UWorld::PersistentLevel

// UGameInstance
inline constexpr uintptr_t LocalPlayers = 0x0; // FILL: UGameInstance::LocalPlayers (TArray)

// ULocalPlayer
inline constexpr uintptr_t PlayerController = 0x0; // FILL: ULocalPlayer::PlayerController

// APlayerController
inline constexpr uintptr_t AcknowledgedPawn = 0x0;   // FILL: APlayerController::AcknowledgedPawn
inline constexpr uintptr_t PlayerCameraManager = 0x0; // FILL: APlayerController::PlayerCameraManager

// APlayerCameraManager
inline constexpr uintptr_t CameraCache = 0x0; // FILL: APlayerCameraManager::CameraCache (FCameraCacheEntry)

// ULevel
inline constexpr uintptr_t Actors = 0x0; // FILL: ULevel::Actors (TArray<AActor*>)

// AActor
inline constexpr uintptr_t RootComponent = 0x0; // FILL: AActor::RootComponent

// USceneComponent
inline constexpr uintptr_t RelativeLocation = 0x0; // FILL: USceneComponent::RelativeLocation (FVector)

// AFortPawn / AFortPlayerPawn
inline constexpr uintptr_t Health = 0x0;    // FILL: AFortPawn::Health (float)
inline constexpr uintptr_t MaxHealth = 0x0; // FILL: AFortPawn::MaxHealth (float)
inline constexpr uintptr_t TeamIndex = 0x0; // FILL: AFortPawn::TeamIndex (uint8)
inline constexpr uintptr_t Mesh = 0x0;      // FILL: ACharacter::Mesh (USkeletalMeshComponent)

// USkeletalMeshComponent
inline constexpr uintptr_t BoneArray = 0x0; // FILL: USkeletalMeshComponent::BoneArray (FVector*)
// Bone stride: 32 bytes per bone in Fortnite (FVector + padding). FILL if changed.
inline constexpr int kBoneStride = 32; // FILL: bone stride

// FCameraCacheEntry
inline constexpr uintptr_t CameraPOV = 0x0; // FILL: FCameraCacheEntry::POV (FCameraCachePOV)
// FCameraCachePOV: Location (FVector) @0x0, Rotation (FRotator) @0x18, FOV (float) @0x24
inline constexpr uintptr_t POVLocation = 0x0;  // FILL: POV.Location
inline constexpr uintptr_t POVRotation = 0x18; // FILL: POV.Rotation
inline constexpr uintptr_t POVFOV = 0x24;      // FILL: POV.FOV
} // namespace off
