#include <Windows.h>
#include <TlHelp32.h>
#include <cstdio>
#include <cstring>

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
            const char* file = exe;
            for (const char* p = exe; *p; ++p) {
                if (*p == '\\' || *p == '/') file = p + 1;
            }
            if (_stricmp(file, name) == 0) { pid = pe.th32ProcessID; break; }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
    return pid;
}

int main(int argc, char** argv) {
    const char* dllPath = argc > 1 ? argv[1] : "fortnite-external.dll";
    const char* procName = argc > 2 ? argv[2] : "FortniteClient-Win64-Shipping.exe";

    printf("Injetor - procurando %s...\n", procName);
    DWORD pid = FindPid(procName);
    if (!pid) {
        printf("Processo não encontrado.\n");
        return 1;
    }

    char absPath[MAX_PATH]{};
    if (!GetFullPathNameA(dllPath, MAX_PATH, absPath, nullptr)) {
        printf("GetFullPathNameA failed.\n");
        return 1;
    }

    printf("Injetando %s no pid %lu...\n", absPath, pid);

    HANDLE h = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!h) {
        printf("OpenProcess failed: %lu\n", GetLastError());
        return 1;
    }

    SIZE_T pathLen = strlen(absPath) + 1;
    LPVOID remoteMem = VirtualAllocEx(h, nullptr, pathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMem) {
        printf("VirtualAllocEx failed: %lu\n", GetLastError());
        CloseHandle(h);
        return 1;
    }

    SIZE_T written = 0;
    if (!WriteProcessMemory(h, remoteMem, absPath, pathLen, &written) || written != pathLen) {
        printf("WriteProcessMemory failed: %lu\n", GetLastError());
        VirtualFreeEx(h, remoteMem, 0, MEM_RELEASE);
        CloseHandle(h);
        return 1;
    }

    HMODULE kernel32 = GetModuleHandle(L"kernel32.dll");
    LPVOID loadLibrary = (LPVOID)GetProcAddress(kernel32, "LoadLibraryA");
    if (!loadLibrary) {
        printf("GetProcAddress failed: %lu\n", GetLastError());
        VirtualFreeEx(h, remoteMem, 0, MEM_RELEASE);
        CloseHandle(h);
        return 1;
    }

    HANDLE hThread = CreateRemoteThread(h, nullptr, 0, (LPTHREAD_START_ROUTINE)loadLibrary, remoteMem, 0, nullptr);
    if (!hThread) {
        printf("CreateRemoteThread failed: %lu\n", GetLastError());
        VirtualFreeEx(h, remoteMem, 0, MEM_RELEASE);
        CloseHandle(h);
        return 1;
    }

    printf("Thread criada. Esperando 3s...\n");
    WaitForSingleObject(hThread, 3000);

    DWORD exitCode = 0;
    GetExitCodeThread(hThread, &exitCode);
    if (exitCode != 0 && exitCode != STILL_ACTIVE) {
        printf("DLL injetada! HMODULE=0x%lx\n", exitCode);
    } else {
        printf("DLL injetada (thread ainda ativa ou exit code=0).\n");
    }

    CloseHandle(hThread);
    VirtualFreeEx(h, remoteMem, 0, MEM_RELEASE);
    CloseHandle(h);

    return 0;
}
