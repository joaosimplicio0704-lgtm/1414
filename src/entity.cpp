#include "entity.h"
#include "offsets.h"
#include <cmath>

static uintptr_t GetGameInstance(const Memory& m) {
    const uintptr_t gworld = m.ModuleBase() + off::GWorld;
    const uintptr_t world = m.Read<uintptr_t>(gworld);
    if (!world) return 0;
    return m.Read<uintptr_t>(world + off::OwningGameInstance);
}

static uintptr_t GetPlayerController(const Memory& m) {
    // UE5 path: GameInstance -> LocalPlayers[0] -> ULocalPlayer
    // -> ViewportClient -> UGameViewportClient -> World
    // -> GameState -> PlayerArray[0] -> APlayerState
    // -> PawnPrivate -> APawn -> Controller -> APlayerController
    const uintptr_t gi = GetGameInstance(m);
    if (!gi) return 0;
    const TArray players = m.Read<TArray>(gi + off::LocalPlayers);
    if (!players.data || players.count <= 0) return 0;
    const uintptr_t local = m.Read<uintptr_t>(players.data);
    if (!local) return 0;
    const uintptr_t vp = m.Read<uintptr_t>(local + off::ViewportClient);
    if (!vp) return 0;
    const uintptr_t world = m.Read<uintptr_t>(vp + off::ViewportWorld);
    if (!world) return 0;
    const uintptr_t gs = m.Read<uintptr_t>(world + off::GameState);
    if (!gs) return 0;
    const TArray psArr = m.Read<TArray>(gs + off::PlayerArray);
    if (!psArr.data || psArr.count <= 0) return 0;
    const uintptr_t ps = m.Read<uintptr_t>(psArr.data);
    if (!ps) return 0;
    const uintptr_t pawn = m.Read<uintptr_t>(ps + off::PawnPrivate);
    if (!pawn) return 0;
    return m.Read<uintptr_t>(pawn + off::Controller);
}

bool World::GetCamera(Camera& out) const {
    const uintptr_t pc = GetPlayerController(mem_);
    if (!pc) return false;
    const uintptr_t pcm = mem_.Read<uintptr_t>(pc + off::PlayerCameraManager);
    if (!pcm) return false;
    const uintptr_t cache = pcm + off::CameraCache + off::CameraPOV;
    out.location = mem_.Read<FVector>(cache + off::POVLocation);
    out.rotation = mem_.Read<FRotator>(cache + off::POVRotation);
    out.fov = mem_.Read<float>(cache + off::POVFOV);
    return out.fov > 0.0f && out.fov < 180.0f;
}

bool World::GetLocalTeam(uint8_t& out) const {
    const uintptr_t pc = GetPlayerController(mem_);
    if (!pc) return false;
    const uintptr_t pawn = mem_.Read<uintptr_t>(pc + off::AcknowledgedPawn);
    if (!pawn) return false;
    const uintptr_t ps = mem_.Read<uintptr_t>(pawn + off::PlayerState);
    if (!ps) return false;
    out = mem_.Read<uint8_t>(ps + off::TeamIndex);
    return true;
}

FVector World::GetBone(uintptr_t actor, int boneId) const {
    FVector fallback{};
    const uintptr_t rootComp = mem_.Read<uintptr_t>(actor + off::RootComponent);
    if (rootComp) fallback = mem_.Read<FVector>(rootComp + off::RelativeLocation);
    fallback.z += 80.0f;
    const uintptr_t mesh = mem_.Read<uintptr_t>(actor + off::Mesh);
    if (!mesh) return fallback;
    const TArray bones = mem_.Read<TArray>(mesh + off::BoneArray);
    if (!bones.data || bones.count <= boneId) return fallback;
    FMatrix mat{};
    if (!mem_.ReadBytes(bones.data + boneId * off::kBoneStride, &mat, sizeof(FMatrix))) return fallback;
    FVector bone{mat.m[3][0], mat.m[3][1], mat.m[3][2]};
    if (bone.length() < 1.0f) return fallback;
    return bone;
}

std::vector<Entity> World::GetEntities(const Camera& cam) const {
    std::vector<Entity> out;
    const uintptr_t gworld = mem_.ModuleBase() + off::GWorld;
    const uintptr_t world = mem_.Read<uintptr_t>(gworld);
    if (!world) return out;
    // UE5: actors live in Level::ActorCluster (FLevelActorContainer)
    // not ULevel::Actors (doesn't exist in UE5).
    const TArray levels = mem_.Read<TArray>(world + off::Levels);
    if (!levels.data || levels.count <= 0 || levels.count > 100) return out;
    uint8_t localTeam = 0;
    GetLocalTeam(localTeam);
    for (int32_t li = 0; li < levels.count; ++li) {
        const uintptr_t level = mem_.Read<uintptr_t>(levels.data + li * sizeof(uintptr_t));
        if (!level) continue;
        const uintptr_t cluster = mem_.Read<uintptr_t>(level + off::ActorCluster);
        if (!cluster) continue;
        // FLevelActorContainer: offset 0x8 is TArray<AActor*>
        const TArray actors = mem_.Read<TArray>(cluster + off::ActorClusterTArray);
        if (!actors.data || actors.count <= 0 || actors.count > 10000) continue;
        for (int32_t i = 0; i < actors.count; ++i) {
            const uintptr_t actor = mem_.Read<uintptr_t>(actors.data + i * sizeof(uintptr_t));
            if (!actor) continue;
            const uintptr_t rootComp = mem_.Read<uintptr_t>(actor + off::RootComponent);
            if (!rootComp) continue;
            Entity e;
            e.actor = actor;
            e.root = mem_.Read<FVector>(rootComp + off::RelativeLocation);
            if (e.root.length() < 1.0f) continue;
            const uintptr_t hs = mem_.Read<uintptr_t>(actor + off::HealthSet);
            if (hs) {
                e.health = mem_.Read<float>(hs + off::Health);
                e.maxHealth = mem_.Read<float>(hs + off::MaxHealth);
            }
            const uintptr_t ps = mem_.Read<uintptr_t>(actor + off::PlayerState);
            if (ps) e.team = mem_.Read<uint8_t>(ps + off::TeamIndex);
            e.head = GetBone(actor, 68);
            e.distance = (e.root - cam.location).length() / 100.0f;
            e.valid = true;
            out.push_back(e);
        }
    }
    return out;
}
