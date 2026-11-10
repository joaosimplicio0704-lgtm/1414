#pragma once
// Handle cleanup util (adapted from user closehandle.cpp).
// Original depended on Cheat/Globals.hpp + DynamicStub — stripped to
// plain CreateThread so this compiles standalone. Logic unchanged.
#include <Windows.h>

void StartHandleMonitor();
void StopHandleMonitor();
