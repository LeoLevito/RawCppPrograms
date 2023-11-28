#include <array>
#include <iostream>
#include <span>
#include <string> 
#include <random>
#include <stack>
#include <queue>
#include "learncppexamples.h"
#include <fstream>
#include <chrono>
#include "BinaryTree.h"

struct Tree {
	int leaves{}; float height{}; std::string treeName{};
};

void PrintArray(int A[], int n) {
	for (int i = 0; i < n; i++) {
		std::cout << A[i] << " LEAVES, AT INDEX: " << i;
		std::cout << std::endl;
	}
}

void InsertionSort(int A[], int n) {
	int index{}, value{};
	for (int i = 1; i < n; i++) {
		value = A[i];
		index = i - 1;

		while (index >= 0 && A[index] > value) {
			A[index + 1] = A[index];
			index = index - 1;

			//[[maybe_unused]] Tree* dos = new Tree[5];//Memory leak (intended)

			std::cout << "MOVE tree with leaves amount: " << value << " to array index: " << index + 1 << '\n';
		}
		A[index + 1] = value;
	}
}

void LinearSearch(int A[], int insertNumber, int n) {

	bool foundSpecifiedItem = false;
	for (int i = 0; i < n; i++) {
		if (A[i] == insertNumber) {

			std::cout << "found tree with your chosen leaves amount: " << insertNumber << " at array index: " << i << '\n';
			foundSpecifiedItem = true;
		}
	}
	if (!foundSpecifiedItem) {
		std::cout << "A tree with your chosen leaves amount does not exist in the current array! \n";
	}

}

int BinarySearch(int A[], int low, int high, int insertNumber) {
	bool foundSpecifiedItem = false;

	while (!foundSpecifiedItem) {
		int mid{ low + (high - low) / 2 };

		if (insertNumber == A[mid]) {
			foundSpecifiedItem = true;
			std::cout << "\nFOUND TREE WITH YOUR SPECIFIED LEAVES AMOUNT : " << insertNumber << "\nAT ARRAY INDEX : " << mid << '\n';
			return mid;
		}
		if (insertNumber > A[mid]) {
			low = mid + 1;
			std::cout << "increase low to : " << low << '\n';
		}
		if (insertNumber < A[mid]) {
			high = mid - 1;
			std::cout << "decrease high to : " << high << '\n';
		}
		if (low > high) {
			std::cout << "\nMESSAGE: Couldn't find any tree with your specified amount of leaves. Try again." << '\n';
			return -1;
		}

	}
	return -1;
}

auto CurrenPointInTime() {
	return  std::chrono::steady_clock::now();
}


void coutput(std::string coutRequest) {
	std::cout << coutRequest;
}

int insertTreeAmount() {
	int insertTreeAmount{};
	std::cout << "insert tree amount: ";
	std::cin >> insertTreeAmount;
	return insertTreeAmount;
}

int main()
{
	using std::cout; using std::cin; using std::queue; using std::stack; using std::string; using std::to_string;

	const int selectedTreeAmount{ insertTreeAmount() };

	Tree* trees = new Tree[selectedTreeAmount];

	srand((unsigned)time(NULL));
	std::random_device rd;
	std::mt19937 gen(rd());

	queue<Tree> treeQueue;
	stack<Tree> treeStack;

	int* Mama = new int[selectedTreeAmount];
	int nextIndex = 0;

	for (int i = 0; i < selectedTreeAmount; i++) {
		trees[i].leaves = rand() % selectedTreeAmount;
		string name = "treeName number ";
		name += to_string(rand() % selectedTreeAmount);
		trees[i].treeName = name;
		trees[i].height = std::generate_canonical<float, 10>(gen) * 20;
		treeQueue.push(trees[i]);
		treeStack.push(trees[i]);



		Mama[nextIndex] = { trees[i].leaves };
		nextIndex++;
		cout << trees[i].treeName << ", " << trees[i].height << "m, " << trees[i].leaves << " leaves \n" << '\n';
	}

	//do {
	//    std::cout << "current Queue size: " << treeQueue.size() << ", popped Tree: " << '\n' << treeQueue.front().treeName << ", " << treeQueue.front().height << "m, " << treeQueue.front().leaves << " leaves \n";
	//    treeQueue.pop();
	//    std::cout << "current Stack size: " << treeStack.size() << ", popped Tree: " << '\n' << treeStack.top().treeName << ", " << treeStack.top().height << "m, " << treeStack.top().leaves << " leaves \n \n";
	//    treeStack.pop();
	//} while (treeQueue.size() > 0);

	cout << "BEFORE CLEAN ------------------- \n";
	PrintArray(Mama, selectedTreeAmount);
	const auto start1{ CurrenPointInTime() };

	cout << "\nPERFORM CLEAN ------------------- \n";
	InsertionSort(Mama, selectedTreeAmount);
	const auto end1{ CurrenPointInTime() };
	const std::chrono::duration<double> elapsed_seconds1{ end1 - start1 };

	cout << "\nAFTER CLEAN -------------------- \n" << "Time spent cleaning : " << elapsed_seconds1 << '\n';;
	PrintArray(Mama, selectedTreeAmount);

	cout << "\nSEARCH ITEM -------------------- \n";
	int enterValue;
	cout << "enter value between 0 and " << selectedTreeAmount << ": ";
	cin >> enterValue;
	const auto start2{ CurrenPointInTime() };

	//LinearSearch(Mama, enterValue, 100);
	BinarySearch(Mama, 0, selectedTreeAmount, enterValue);
	const auto end2{ CurrenPointInTime() };
	const std::chrono::duration<double> elapsed_seconds2{ end2 - start2 };
	cout << "time spent searching (ish): " << elapsed_seconds2;

	//Testing();
	return 0;
}


