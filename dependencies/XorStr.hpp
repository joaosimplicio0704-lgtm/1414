#pragma once
// Minimal XorStr shim so StealthyOpenProcess.cpp compiles as given.
// Supports both patterns used there:
//   XorStr("...") as const char*  and  xorstr("...").use([](const char* s){...})
// Real projects should swap in JustasMasiulis/xorstr or skadro.
#include <cstddef>
#include <cstdint>

template <size_t N>
struct XorStr {
    char enc[N];
    mutable char plain[N];
    constexpr XorStr(const char (&s)[N]) : enc{}, plain{} {
        constexpr uint32_t key = 0x9E3779B9u;
        for (size_t i = 0; i < N; ++i) enc[i] = char(s[i] ^ char((key >> ((i % 4) * 8)) & 0xFF) ^ char(i * 31));
    }
    const char* c_str() const {
        constexpr uint32_t key = 0x9E3779B9u;
        for (size_t i = 0; i < N; ++i) plain[i] = char(enc[i] ^ char((key >> ((i % 4) * 8)) & 0xFF) ^ char(i * 31));
        return plain;
    }
    operator const char*() const { return c_str(); }
    template <typename F>
    auto use(F&& f) const { return f(c_str()); }
};

#define XorStr(s) XorStr<sizeof(s)>(s)
#define xorstr(s) XorStr(s)
