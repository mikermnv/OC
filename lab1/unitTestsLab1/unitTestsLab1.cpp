#include "pch.h"
#include "CppUnitTest.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <process.h>
#include "../common/ErrorCodes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unitTestsLab1
{
	TEST_CLASS(unitTestsLab1)
	{
	public:
		
        TEST_METHOD(TestCreateProcessSuccess)
        {
            // Arrange
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(STARTUPINFO));
            si.cb = sizeof(STARTUPINFO);
            ZeroMemory(&pi, sizeof(pi));
            std::wstring utilityName = L"creator.exe";
            std::wstring wstrParam1 = L"test";
            std::wstring wstrParam2 = L"2";

            std::wstring wstrArgs = utilityName + L"  " + wstrParam1 + L"  " + wstrParam2;
            wchar_t* creatorCmdLine = &wstrArgs[0];
            // Act
            BOOL result = CreateProcess(NULL, creatorCmdLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

            // Assert
            Assert::IsTrue(result, L"CreateProcess failed");
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        TEST_METHOD(TestCreateProcessFailure)
        {
            // Arrange
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(STARTUPINFO));
            si.cb = sizeof(STARTUPINFO);
            ZeroMemory(&pi, sizeof(pi));
            std::wstring utilityName = L"non_existent_exe.exe";
            std::wstring wstrParam1 = L"test";
            std::wstring wstrParam2 = L"2";

            std::wstring wstrArgs = utilityName + L"  " + wstrParam1 + L"  " + wstrParam2;
            wchar_t* creatorCmdLine = &wstrArgs[0];
            // Act
            BOOL result = CreateProcess(NULL, creatorCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

            // Assert
            Assert::IsFalse(result, L"CreateProcess should have failed");
        }

        TEST_METHOD(TestOpenInputBinaryFileFailure)
        {
            // Arrange
            std::ifstream inputFile("non_existent_file.bin", std::ios::binary);

            // Act
            bool isOpen = inputFile.is_open();

            // Assert
            Assert::IsFalse(isOpen, L"Input binary file should not have opened");
        }

        TEST_METHOD(TestOpenReportFile)
        {
            // Arrange
            std::ifstream reportFile("test_report");

            // Act
            bool isOpen = reportFile.is_open();

            // Assert
            Assert::IsFalse(isOpen, L"Report file should not have opened");
        }

        TEST_METHOD(TestOpenReportFileFailure)
        {
            // Arrange
            std::ifstream reportFile("non_existent_report.txt");

            // Act
            bool isOpen = reportFile.is_open();

            // Assert
            Assert::IsFalse(isOpen, L"Report file should not have opened");
        }

        TEST_METHOD(TestInvalidSalaryPerHour)
        {
            // Arrange
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(STARTUPINFO));
            si.cb = sizeof(STARTUPINFO);
            ZeroMemory(&pi, sizeof(pi));
            std::wstring RepUtilityName = L"reporter.exe";
            std::wstring wstrRepParam1 = L"testw";
            std::wstring wstrRepParam2 = L"report";
            std::wstring wstrRepParam3 = L"t";

            std::wstring wstr2Args = RepUtilityName + L"  " + wstrRepParam1 + L"  " + wstrRepParam2 + L"  " + wstrRepParam3;
            wchar_t* reporterCmdLine = &wstr2Args[0];

            // Act
            BOOL result = CreateProcess(NULL, reporterCmdLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
            DWORD exitCode;
            GetExitCodeProcess(pi.hProcess, &exitCode);

            // Assert
            Assert::IsTrue(result, L"CreateProcess failed");
            Assert::AreEqual((DWORD)ERR_OPEN_INPUT_FILE, exitCode);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        TEST_METHOD(TestInvalidEmployeeID)
        {
            // Arrange
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(STARTUPINFO));
            si.cb = sizeof(STARTUPINFO);
            ZeroMemory(&pi, sizeof(pi));

            // Act
            BOOL result = CreateProcess(NULL, L"creator.exe  test.bin  -5", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
            DWORD exitCode;
            GetExitCodeProcess(pi.hProcess, &exitCode);

            // Assert
            Assert::IsFalse(result, L"CreateProcess should have failed");
            Assert::AreEqual((DWORD)ERR_READ_ID, exitCode);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        TEST_METHOD(TestInvalidEmployeeHours)
        {
            // Arrange
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(STARTUPINFO));
            si.cb = sizeof(STARTUPINFO);
            ZeroMemory(&pi, sizeof(pi));

            // Act
            BOOL result = CreateProcess(NULL, L"creator.exe  test.bin  10", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
            DWORD exitCode;
            GetExitCodeProcess(pi.hProcess, &exitCode);

            // Assert
            Assert::IsFalse(result, L"CreateProcess should have failed");
            Assert::AreEqual((DWORD)ERR_READ_HOURS, exitCode);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
	};
}
