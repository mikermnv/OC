#pragma once

#include <Windows.h>

struct Params {
    int* array;
    int thread_id;
    int array_size;
    HANDLE hEndWorkEvent; //individual
    HANDLE hCannotWorkEvent; //individual
    HANDLE hContinueEvent; //individual
    CRITICAL_SECTION* cs;
};