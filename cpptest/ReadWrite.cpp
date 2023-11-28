#include "ReadWrite.h"
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
void createFile() {
	std::string fname{"example.txt"};
	
	
	int myInt = 15;
	int myInt2 = 35;
	//int myInt2 = 33;
	{
		std::ofstream myFile(fname);
		myFile << myInt << ' ' << myInt2 << '\n';
	}
	
	std::cout << myInt << '\n' << myInt2 << "\n at start" << '\n' << '\n';

	myInt = 2;
	myInt2 = 3;
	std::cout << myInt << '\n' << myInt2 << "\n after set" << '\n' << '\n';
	{
		std::ifstream yourFile(fname);
		yourFile >> myInt >> myInt2;
	}
	
	std::cout << myInt << '\n' << myInt2 << "\n after read?" << '\n' << '\n';
}

//int main() {
//	createFile();
//	return 0;
//
//}