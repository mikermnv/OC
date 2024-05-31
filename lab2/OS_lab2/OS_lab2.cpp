#include <iostream>
#include "SpecialCodes.h"
#include "ThreadData.h"
#include "ThreadFunctions.h"
#include <Windows.h>
#include <process.h>
#include <vector>
#include <exception>

int main() {
	std::vector<int> array;
	ThreadData threadData;
	int arraySize = 0;

	std::cout << "Enter array size: ";
	std::cin >> arraySize;
	std::cout << "Enter " << arraySize << " int elements: \n";
	int elem = 0;
	for (int i = 0; i < arraySize; i++) {
		std::cin >> elem;
		array.push_back(elem);
	}

	threadData.array = array;

	DWORD IDThreads[2];
	HANDLE hThreads[2];
	hThreads[0] = CreateThread(NULL, 0, MinMax, &threadData, 0, &IDThreads[0]);
	if (hThreads[0] == NULL) {
		std::cerr << "Failed to create MinMax thread." << std::endl;
		CloseHandle(hThreads[0]);
		return 1;
	}
	hThreads[1] = CreateThread(NULL, 0, AverageThread, &threadData, 0, &IDThreads[1]);
	if (hThreads[1] == NULL) {
		std::cerr << "Failed to create AverageThread." << std::endl;
		CloseHandle(hThreads[1]);
		return 1;
	}

	WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

	// It is more appropriate to use WaitForMultipleObjects here, since we are waiting for the completion of 
	// threads to one place in the program, and not to different ones.

	/*WaitForSingleObject(hThreads[0], INFINITE);
	WaitForSingleObject(hThreads[1], INFINITE);*/

	CloseHandle(hThreads[0]);
	CloseHandle(hThreads[1]);

	for (auto& elem : array) {
		if (elem == threadData.min || elem == threadData.max) {
			elem = static_cast<int>(threadData.average);
		}
	}

	std::cout << "New array:\n";
	for (auto& elem : array) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;

	return 0;
}