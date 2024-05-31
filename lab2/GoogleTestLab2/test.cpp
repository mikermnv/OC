#include "pch.h"
#include "../OS_lab2/SpecialCodes.h"
#include "../OS_lab2/ThreadData.h"
#include "../OS_lab2/ThreadFunctions.h"
#include "../OS_lab2/ThreadFunctions.cpp"
#include <Windows.h>
#include <process.h>
#include <vector>
#include <exception>

TEST(ThreadingTests, TestMinMax) {
    std::vector<int> testArray = { 5, 2, 8, 1, 9 };
    ThreadData threadData;
    threadData.array = testArray;

    DWORD threadID;
    HANDLE hThread = CreateThread(NULL, 0, MinMax, &threadData, 0, &threadID);
    ASSERT_TRUE(hThread != NULL);

    WaitForSingleObject(hThread, INFINITE);

    EXPECT_EQ(threadData.min, 1);
    EXPECT_EQ(threadData.max, 9);

    CloseHandle(hThread);
}

TEST(ThreadingTests, TestAverage) {
    std::vector<int> testArray = { 5, 2, 8, 1, 9 };
    ThreadData threadData;
    threadData.array = testArray;

    DWORD threadID;
    HANDLE hThread = CreateThread(NULL, 0, AverageThread, &threadData, 0, &threadID);
    ASSERT_TRUE(hThread != NULL);

    WaitForSingleObject(hThread, INFINITE);

    EXPECT_NEAR(threadData.average, 5.0, 0.001);

    CloseHandle(hThread);
}

TEST(ThreadingTests, TestArrayModification) {
    std::vector<int> testArray = { 5, 2, 8, 1, 9 };
    ThreadData threadData;
    threadData.array = testArray;
    threadData.min = 1;
    threadData.max = 9;
    threadData.average = 5.0;

    DWORD threadIDs[2];
    HANDLE hThreads[2];
    hThreads[0] = CreateThread(NULL, 0, MinMax, &threadData, 0, &threadIDs[0]);
    ASSERT_TRUE(hThreads[0] != NULL);
    hThreads[1] = CreateThread(NULL, 0, AverageThread, &threadData, 0, &threadIDs[1]);
    ASSERT_TRUE(hThreads[1] != NULL);

    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    for (auto& elem : testArray) {
        if (elem == threadData.min || elem == threadData.max) {
            elem = static_cast<int>(threadData.average);
        }
    }

    EXPECT_EQ(testArray[0], 5);
    EXPECT_EQ(testArray[1], 2);
    EXPECT_EQ(testArray[2], 8);
    EXPECT_EQ(testArray[3], 5);
    EXPECT_EQ(testArray[4], 5);

    CloseHandle(hThreads[0]);
    CloseHandle(hThreads[1]);
}

TEST(ThreadingTests, TestNullMinMaxHandle) {
    std::vector<int> testArray = { 5, 2, 8, 1, 9 };
    ThreadData threadData;
    threadData.array = testArray;

    DWORD threadIDs;
    HANDLE hThreads;
    hThreads = CreateThread(NULL, 0, MinMax, &threadData, 0, &threadIDs);
    ASSERT_TRUE(hThreads != NULL);

    WaitForSingleObject(hThreads, INFINITE);

    CloseHandle(hThreads);
}

TEST(ThreadingTests, TestNullAverageThreadHandle) {
    std::vector<int> testArray = { 5, 2, 8, 1, 9 };
    ThreadData threadData;
    threadData.array = testArray;

    DWORD threadIDs;
    HANDLE hThreads;
    hThreads = CreateThread(NULL, 0, AverageThread, &threadData, 0, &threadIDs);
    ASSERT_TRUE(hThreads != NULL);

    WaitForSingleObject(hThreads, INFINITE);

    CloseHandle(hThreads);
}

TEST(ThreadingTests, TestArraySizeInput) {
    std::vector<int> testArray = { 5, 2, 8, 1, 9 };
    ThreadData threadData;
    threadData.array = testArray;

    DWORD threadIDs[2];
    HANDLE hThreads[2];
    hThreads[0] = CreateThread(NULL, 0, MinMax, &threadData, 0, &threadIDs[0]);
    ASSERT_TRUE(hThreads[0] != NULL);
    hThreads[1] = CreateThread(NULL, 0, AverageThread, &threadData, 0, &threadIDs[1]);
    ASSERT_TRUE(hThreads[1] != NULL);

    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    EXPECT_EQ(testArray.size(), 5);

    CloseHandle(hThreads[0]);
    CloseHandle(hThreads[1]);
}