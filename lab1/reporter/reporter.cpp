#include "Employee.h"
#include "ErrorCodes.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
	std::ifstream binFile;
	std::ofstream txtFile;
	try {
		std::string fileName = argv[1];
		binFile.open("..\\OS_Lab1\\" + fileName, std::ios::binary);
		if (!binFile)
		{
			std::cerr << "Failed to open input binary file." << std::endl;
			throw ERR_OPEN_INPUT_FILE;
		}
		std::string txtFileName = argv[2];
		txtFile.open("..\\OS_Lab1\\" + txtFileName);
		if (!txtFile)
		{
			std::cerr << "Failed to create output text file." << std::endl;
			throw ERR_CREATE_OUTPUT_FILE;
		}
		double salaryPerHour = atof(argv[3]);
		if (salaryPerHour <= 0) {
			std::cerr << "Invalid value for salary per hour." << std::endl;
			throw ERR_INVALID_SALARY_PER_HOUR;
		}

			txtFile << "Report on file \"" << fileName << "\"\n";
		txtFile << "Employee number(ID), employee name, hours, salary.\n";

		employee emp;
		while (binFile.read(std::_Bit_cast<char*>(&emp), sizeof(emp)))
		{
			double salary = emp.hours * salaryPerHour;
			txtFile << emp.ID << ", " << emp.name << ", " << emp.hours << ", " << salary << "\n";
		}

		binFile.close();
		txtFile.close();
	}
	catch (int errorCode) {
		std::cerr << "Error" << std::endl;
		binFile.close();
		txtFile.close();
		return errorCode;
	}
	catch (...) {
		std::cerr << "Unknown error" << std::endl;
		binFile.close();
		txtFile.close();
		return ERR_UNKNOWN_ERROR;
	}
}