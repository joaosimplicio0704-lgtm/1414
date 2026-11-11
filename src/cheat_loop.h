#pragma once
// Cheat loop - thread da DLL injetada
DWORD WINAPI CheatThread(LPVOID param);
void StopCheat();
bool IsCheatRunning();
