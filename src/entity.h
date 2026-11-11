#pragma once
// Unreal entity walk: GWorld -> OwningGameInstance -> LocalPlayers ->
// PlayerController -> AcknowledgedPawn / PlayerCameraManager, Level -> Actors.
#include "internal.h"
#include "sdk.h"
#include <vector>

struct Camera {
    FVector location{};
    FRotator rotation{};
    float fov = 90.0f;
};

struct Entity {
    uintptr_t actor = 0;
    FVector root{};
    FVector head{};
    float health = 0, maxHealth = 100;
    uint8_t team = 0;
    float distance = 0;
    bool valid = false;
};

class World {
public:
    explicit World(const InternalMemory& mem) : mem_(mem) {}
    bool GetCamera(Camera& out) const;
    bool GetLocalTeam(uint8_t& out) const;
    std::vector<Entity> GetEntities(const Camera& cam) const;
    FVector GetBone(uintptr_t actor, int boneId) const;

private:
    const InternalMemory& mem_;
};
