#pragma once
// Aim via SendInput mouse deltas. Never writes view angles to game memory.
#include "sdk.h"
#include <vector>
#include "entity.h"

bool PickTarget(const std::vector<Entity>& ents, const Camera& cam,
    int screenW, int screenH, float fovPx, uint8_t localTeam, bool teamCheck,
    Entity& out);

void SendMouseDelta(float dx, float dy);
void AimTick(const Entity& target, const Camera& cam, int screenW, int screenH, float smoothing);
