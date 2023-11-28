#pragma once
#include<iostream>

int getinputValue() {
	using std::cout;
	using std::cin;

	int input{};
	cout << "\nEnter integer, man: ";
	cin >> input;
	return input;
}
void Testing() {
	using std::cout;
	using std::cin;

	int insertNumber1{ getinputValue()};
	int insertNumber2{ getinputValue()};

	/*cout << "\nEnter a number: ";
	cin >> insertNumber1;*/
	//cout << "\nEnter another number: ";
	//cin >> insertNumber2;
	cout << '\n' << insertNumber1 << " + " << insertNumber2 << " = " << insertNumber1 + insertNumber2;
	cout << '\n' << insertNumber1 << " - " << insertNumber2 << " = " << insertNumber1 - insertNumber2;
}

int doubleNumber(int integer) {
	std::cout << "Enter number: ";
	std::cin >> integer;
	return (integer * 2);
}
