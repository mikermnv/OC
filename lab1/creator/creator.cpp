#include "Employee.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>

int main(int argc, char* argv[]) {

	int employeeNum = atoi(argv[2]);
	std::string fileName = argv[1];
	std::ofstream file("..\\OS_Lab1\\" + fileName, std::ios::binary);
	if (!file)
	{
		std::cerr << "Failed to create binary file." << std::endl;
		return -1;
	}
	try {
		std::vector<employee> list(employeeNum);
		employee emp;
		for (int i = 0; i < employeeNum; i++) {
			std::cout << "Enter id of " << i + 1 << " employee:";
			if (!(std::cin >> emp.ID)) {
				throw 1;
			}
			std::cout << "Enter name of " << i + 1 << " employee:";
			if (!(std::cin >> emp.name)) {
				throw 2;
			}
			std::cout << "Enter hours for " << i + 1 << " employee: ";
			if (!(std::cin >> emp.hours)) {
				throw 3;
			}

			file.write(std::_Bit_cast<const char*>(&emp), sizeof(emp));
		}
		file.close();
	}
	catch (int errorCode) {
		file.close();
		return errorCode;
	}
	catch (...) {
		file.close();
		return -2;
	}
}