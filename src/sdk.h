#pragma once
// Unreal math + W2S. Convention: Unreal rotation matrix + FOV projection
// off PlayerCameraManager POV. Origin bottom-left -> Y inverted for screen.
#include <Windows.h>
#include <cmath>
#include <cstdint>

struct FVector {
    float x = 0, y = 0, z = 0;
    FVector() = default;
    FVector(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    FVector operator-(const FVector& o) const { return {x - o.x, y - o.y, z - o.z}; }
    FVector operator+(const FVector& o) const { return {x + o.x, y + o.y, z + o.z}; }
    float length() const { return std::sqrt(x * x + y * y + z * z); }
};

struct FRotator {
    float pitch = 0, yaw = 0, roll = 0;
};

struct FMatrix {
    float m[4][4] = {};
};

inline FMatrix RotationMatrix(const FRotator& r) {
    const float rad = 3.14159265f / 180.0f;
    const float cp = std::cos(r.pitch * rad), sp = std::sin(r.pitch * rad);
    const float cy = std::cos(r.yaw * rad), sy = std::sin(r.yaw * rad);
    const float cr = std::cos(r.roll * rad), sr = std::sin(r.roll * rad);
    FMatrix m;
    m.m[0][0] = cp * cy; m.m[0][1] = cp * sy; m.m[0][2] = sp; m.m[0][3] = 0;
    m.m[1][0] = sr * sp * cy - cr * sy; m.m[1][1] = sr * sp * sy + cr * cy; m.m[1][2] = -sr * cp; m.m[1][3] = 0;
    m.m[2][0] = -(cr * sp * cy + sr * sy); m.m[2][1] = cy * sr - cr * sp * sy; m.m[2][2] = cr * cp; m.m[2][3] = 0;
    m.m[3][0] = 0; m.m[3][1] = 0; m.m[3][2] = 0; m.m[3][3] = 1;
    return m;
}

inline bool WorldToScreen(const FVector& world, const FVector& camLoc, const FRotator& camRot,
    float fov, int screenW, int screenH, FVector& out) {
    if (fov <= 0.0f || screenW <= 0 || screenH <= 0) return false;
    const FMatrix rot = RotationMatrix(camRot);
    const FVector delta = world - camLoc;
    const float vDot = delta.x * rot.m[0][0] + delta.y * rot.m[0][1] + delta.z * rot.m[0][2];
    const float vDotY = delta.x * rot.m[1][0] + delta.y * rot.m[1][1] + delta.z * rot.m[1][2];
    const float vDotZ = delta.x * rot.m[2][0] + delta.y * rot.m[2][1] + delta.z * rot.m[2][2];
    if (vDot < 1.0f) return false;
    const float f = (screenW * 0.5f) / std::tan(fov * 3.14159265f / 360.0f);
    out.x = (screenW * 0.5f) + vDotY * f / vDot;
    out.y = (screenH * 0.5f) - vDotZ * f / vDot; // Y flipped (bottom-left origin)
    out.z = vDot;
    return true;
}

struct TArray {
    uintptr_t data = 0;
    int32_t count = 0;
    int32_t capacity = 0;
};
