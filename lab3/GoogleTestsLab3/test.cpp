#include "pch.h"
#include "../lab3/Params.h"
#include <Windows.h>
#include <thread>
#include <chrono>


class MarkerTests : public ::testing::Test {
protected:
    void SetUp() override {
        array_ = new int[array_size_] {};
        cs_ = new CRITICAL_SECTION;
        InitializeCriticalSection(cs_);

        hContinueEvent_ = CreateEventA(NULL, FALSE, FALSE, NULL);
        ASSERT_NE(hContinueEvent_, nullptr);

        hCannotWorkEvent_ = CreateEventA(NULL, FALSE, FALSE, NULL);
        ASSERT_NE(hCannotWorkEvent_, nullptr);

        hEndWorkEvent_ = CreateEventA(NULL, FALSE, FALSE, NULL);
        ASSERT_NE(hEndWorkEvent_, nullptr);
    }

    void TearDown() override {
        DeleteCriticalSection(cs_);
        delete[] array_;
        delete cs_;
        CloseHandle(hContinueEvent_);
        CloseHandle(hCannotWorkEvent_);
        CloseHandle(hEndWorkEvent_);
    }

    static DWORD WINAPI marker(LPVOID param) {
        return ::marker(param);
    }

    int array_size_ = 10;
    int* array_;
    CRITICAL_SECTION* cs_;
    HANDLE hContinueEvent_;
    HANDLE hCannotWorkEvent_;
    HANDLE hEndWorkEvent_;
};

TEST_F(MarkerTests, TestMarkerThreadSafety) {
    constexpr int kNumThreads = 5;
    std::vector<std::thread> threads;
    std::vector<Params*> params(kNumThreads);

    for (int i = 0; i < kNumThreads; ++i) {
        params[i] = new Params{
            .array = array_,
            .thread_id = i + 1,
            .array_size = array_size_,
            .hEndWorkEvent = hEndWorkEvent_,
            .hCannotWorkEvent = hCannotWorkEvent_,
            .hContinueEvent = hContinueEvent_,
            .cs = cs_
        };
        threads.emplace_back(marker, params[i]);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    int marked_count = 0;
    for (int i = 0; i < array_size_; ++i) {
        if (array_[i] != 0) {
            ++marked_count;
        }
    }

    EXPECT_EQ(marked_count, kNumThreads);
    for (auto param : params) {
        delete param;
    }
}

TEST_F(MarkerTests, TestMarkerEventHandling) {
    Params param{
        .array = array_,
        .thread_id = 1,
        .array_size = array_size_,
        .hEndWorkEvent = hEndWorkEvent_,
        .hCannotWorkEvent = hCannotWorkEvent_,
        .hContinueEvent = hContinueEvent_,
        .cs = cs_
    };

    std::thread thread(marker, &param);

    // Wait for the thread to signal that it cannot continue
    EXPECT_EQ(WaitForSingleObject(hCannotWorkEvent_, 5000), WAIT_OBJECT_0);

    // Signal the thread to continue
    SetEvent(hContinueEvent_);

    // Wait for the thread to finish
    EXPECT_EQ(WaitForSingleObject(thread.native_handle(), 5000), WAIT_OBJECT_0);

    // Signal the thread to stop
    SetEvent(hEndWorkEvent_);

    thread.join();
}

TEST_F(MarkerTests, TestMarkerInvalidInput) {
    Params param{
        .array = nullptr,
        .thread_id = 0,
        .array_size = 0,
        .hEndWorkEvent = hEndWorkEvent_,
        .hCannotWorkEvent = hCannotWorkEvent_,
        .hContinueEvent = hContinueEvent_,
        .cs = cs_
    };

    EXPECT_DEATH(marker(&param), "");
}