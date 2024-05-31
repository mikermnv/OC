#pragma once

#include "ThreadData.h"
#include "SpecialCodes.h"
#include <iostream>
#include <Windows.h>

DWORD WINAPI MinMax(LPVOID lpParam);
DWORD WINAPI AverageThread(LPVOID lpParam);