#pragma once
// External RPM read-only. No writes to game memory, ever.
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

class Memory {
public:
    bool Attach(const char* processName);
    void Detach();
    bool IsAttached() const { return hProcess_ != nullptr; }
    DWORD Pid() const { return pid_; }
    uintptr_t ModuleBase() const { return moduleBase_; }

    template <typename T>
    T Read(uintptr_t addr) const {
        T v{};
        if (!hProcess_ || !addr) return v;
        SIZE_T got = 0;
        ReadProcessMemory(hProcess_, (LPCVOID)addr, &v, sizeof(T), &got);
        if (got != sizeof(T)) return T{};
        return v;
    }

    bool ReadBytes(uintptr_t addr, void* out, SIZE_T size) const {
        if (!hProcess_ || !addr || !out) return false;
        SIZE_T got = 0;
        return ReadProcessMemory(hProcess_, (LPCVOID)addr, out, size, &got) && got == size;
    }

private:
    HANDLE hProcess_ = nullptr;
    DWORD pid_ = 0;
    uintptr_t moduleBase_ = 0;
};
