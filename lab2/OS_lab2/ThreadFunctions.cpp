#include "ThreadFunctions.h"

DWORD WINAPI MinMax(LPVOID lpParam) {
	try {
		auto* threadData = static_cast<ThreadData*>(lpParam);

		threadData->min = threadData->array[0];
		threadData->max = threadData->array[0];

		for (int elem : threadData->array) {
			if (elem < threadData->min)
				threadData->min = elem;
			Sleep(SLEEP_TIME_MIN);
			if (elem > threadData->max)
				threadData->max = elem;
			Sleep(SLEEP_TIME_MIN);
		}

		std::cout << "Min element: " << threadData->min << std::endl;
		std::cout << "Max element: " << threadData->max << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception in MinMax thread: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception in MinMax thread" << std::endl;
	}
	return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam) {
	try {
		auto* threadData = static_cast<ThreadData*>(lpParam);

		int sum = 0;
		for (int elem : threadData->array) {
			sum += elem;
			Sleep(SLEEP_TIME_MAX);
		}

		threadData->average = static_cast<double>(sum) / static_cast<double>(threadData->array.size());

		std::cout << "Average: " << threadData->average << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception in AverageThread: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception in AverageThread" << std::endl;
	}
	return 0;
}