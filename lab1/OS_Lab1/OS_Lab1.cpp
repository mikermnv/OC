#include "Employee.h"
#include "ErrorCodes.h"
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <process.h>
#include <string>

int main() {
	std::string binFileName;
	int numOfEmployee = 0;


	std::cout << "Enter name of binary file:\n";
	std::cin >> binFileName;

	std::cout << "Enter number of employees:\n";
	std::cin >> numOfEmployee;

	STARTUPINFO si;
	PROCESS_INFORMATION piCreator;
	DWORD exit_code;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	ZeroMemory(&piCreator, sizeof(piCreator));

	std::string numOfEmplStr = std::to_string(numOfEmployee);
	std::wstring utilityName = L"creator.exe";
	std::wstring wstrParam1(binFileName.begin(), binFileName.end());
	std::wstring wstrParam2(numOfEmplStr.begin(), numOfEmplStr.end());
	
	std::wstring wstrArgs = utilityName + L"  " + wstrParam1 + L"  " + wstrParam2;
	wchar_t* creatorCmdLine = &wstrArgs[0];
	if (!CreateProcess(NULL, creatorCmdLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCreator))
	{

		std::cerr << GetLastError()<<"\n";
		std::cerr << "Failed to start Creator utility." << std::endl;
		return 1;
	}

	WaitForSingleObject(piCreator.hProcess, INFINITE);
	GetExitCodeProcess(piCreator.hProcess, &exit_code);
	if (exit_code != 0) {
		if (exit_code == ERR_CREATE_BIN_FILE) {
			std::cerr << "Failed to create binary file." << std::endl;
		}
		else if (exit_code == ERR_READ_ID) {
			std::cerr << "Invalid input for employee ID." << std::endl;
		}
		else if (exit_code == ERR_READ_NAME){
			std::cerr << "Invalid input for employee name." << std::endl;
		}
		else if (exit_code == ERR_READ_HOURS) {
			std::cerr << "Invalid input for employee hours." << std::endl;
		}
		else if (exit_code == ERR_EMPLOYEE_NUM) {
			std::cerr << "Invalid input number of employees." << std::endl;
		}
		else {
			std::cerr <<"Error in creator.exe" << std::endl;
		}
		return 1;
	}
	CloseHandle(piCreator.hProcess);
	CloseHandle(piCreator.hThread);

	std::ifstream inputFile(binFileName, std::ios::binary);
	if (!inputFile)
	{
		std::cerr << "Failed to open input binary file." << std::endl;
		return 1;
	}

	employee emp;
	while (inputFile.read(reinterpret_cast<char*>(&emp), sizeof(emp)))
	{
		std::cout << "Employee Number: " << emp.ID << std::endl;
		std::cout << "Employee Name: " << emp.name << std::endl;
		std::cout << "Hours Worked: " << emp.hours << std::endl;
		std::cout << std::endl;
	}

	inputFile.close();

	std::string outputFileName;
	double salaryPerHour = 0;

	std::cout << "Enter output file name: ";
	std::cin >> outputFileName;
	std::cout << "Enter hourly rate: ";
	std::cin >> salaryPerHour;

	PROCESS_INFORMATION piReporter;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	ZeroMemory(&piReporter, sizeof(piReporter));

	std::string salaryPerHourStr = std::to_string(salaryPerHour);
	std::wstring RepUtilityName = L"reporter.exe";
	std::wstring wstrRepParam1(binFileName.begin(), binFileName.end());
	std::wstring wstrRepParam2(outputFileName.begin(), outputFileName.end());
	std::wstring wstrRepParam3(salaryPerHourStr.begin(), salaryPerHourStr.end());

	std::wstring wstr2Args = RepUtilityName + L"  " + wstrRepParam1 + L"  " + wstrRepParam2 + L"  " + wstrRepParam3;
	wchar_t* reporterCmdLine = &wstr2Args[0];
	if (!CreateProcess(NULL, reporterCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &piReporter))
	{
		std::cerr << "Failed to start Reporter utility." << std::endl;
		return 1;
	}

	WaitForSingleObject(piReporter.hProcess, INFINITE);
	GetExitCodeProcess(piReporter.hProcess, &exit_code);
	if (exit_code != 0) {
		if (exit_code == ERR_OPEN_INPUT_FILE) {
			std::cerr << "Failed to open input binary file." << std::endl;
		}
		else if (exit_code == ERR_CREATE_OUTPUT_FILE) {
			std::cerr << "Failed to create output text file." << std::endl;
		}
		else if (exit_code == ERR_INVALID_SALARY_PER_HOUR) {
			std::cerr << "Invalid value for salary per hour." << std::endl;
		}
		else {
			std::cerr << "Error in reporter.exe" << std::endl;
		}
		return 1;
	}
	CloseHandle(piReporter.hProcess);
	CloseHandle(piReporter.hThread);

	std::ifstream reportFile(outputFileName);
	if (!reportFile)
	{
		std::cerr << "Failed to open report file." << std::endl;
		return 1;
	}

	std::string line;
	std::cout << "Report:" << std::endl;
	while (getline(reportFile, line))
		std::cout << line << std::endl;

	reportFile.close();

	return 0;
}