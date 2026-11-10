#include "memory.h"
#include "StealthyOpenProcess.h"

static DWORD FindPid(const char* name) {
    DWORD pid = 0;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return 0;
    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);
    if (Process32FirstW(snap, &pe)) {
        do {
            char exe[MAX_PATH]{};
            WideCharToMultiByte(CP_UTF8, 0, pe.szExeFile, -1, exe, sizeof(exe), nullptr, nullptr);
            if (_stricmp(exe, name) == 0) { pid = pe.th32ProcessID; break; }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
    return pid;
}

static uintptr_t FindModule(DWORD pid, const char* name) {
    uintptr_t base = 0;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (snap == INVALID_HANDLE_VALUE) return 0;
    MODULEENTRY32W me{};
    me.dwSize = sizeof(me);
    if (Module32FirstW(snap, &me)) {
        do {
            char mod[MAX_PATH]{};
            WideCharToMultiByte(CP_UTF8, 0, me.szModule, -1, mod, sizeof(mod), nullptr, nullptr);
            if (_stricmp(mod, name) == 0) { base = (uintptr_t)me.modBaseAddr; break; }
        } while (Module32NextW(snap, &me));
    }
    CloseHandle(snap);
    return base;
}

bool Memory::Attach(const char* processName) {
    Detach();
    pid_ = FindPid(processName);
    if (!pid_) return false;
    // StealthyOpenProcess first (user attachment), fallback to plain OpenProcess.
    HANDLE h = StealthyOpenProcess::Execute(pid_, PROCESS_VM_READ | PROCESS_QUERY_INFORMATION);
    if (!h) h = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid_);
    if (!h) return false;
    hProcess_ = h;
    moduleBase_ = FindModule(pid_, processName);
    return moduleBase_ != 0;
}

void Memory::Detach() {
    if (hProcess_) { CloseHandle(hProcess_); hProcess_ = nullptr; }
    pid_ = 0;
    moduleBase_ = 0;
}
