#include "Employee.h"
#include "ErrorCodes.h"
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
		return ERR_CREATE_BIN_FILE;
	}
	try {
		if (employeeNum <= 0) {
			throw ERR_EMPLOYEE_NUM;
		}
		std::vector<employee> list(employeeNum);
		employee emp;
		for (int i = 0; i < employeeNum; i++) {
			std::cout << "Enter id of " << i + 1 << " employee:";
			if (!(std::cin >> emp.ID)) {
				throw ERR_READ_ID;
			}
			std::cout << "Enter name of " << i + 1 << " employee:";
			if (!(std::cin >> emp.name)) {
				throw ERR_READ_NAME;
			}
			std::cout << "Enter hours for " << i + 1 << " employee: ";
			if (!(std::cin >> emp.hours)) {
				throw ERR_READ_HOURS;
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
		return ERR_UNKNOWN_ERROR;
	}
}