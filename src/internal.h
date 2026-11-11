#pragma once
// Internal memory - leitura direta (DLL injetada no processo alvo)
#include <Windows.h>
#include <cstdint>
#include <cstring>

class InternalMemory {
public:
    template <typename T>
    T Read(uintptr_t addr) const {
        T v{};
        if (!addr) return v;
        __try {
            std::memcpy(&v, (const void*)addr, sizeof(T));
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            return T{};
        }
        return v;
    }

    bool ReadBytes(uintptr_t addr, void* out, SIZE_T size) const {
        if (!addr || !out) return false;
        __try {
            std::memcpy(out, (const void*)addr, size);
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
        return true;
    }
};
