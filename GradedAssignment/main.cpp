#include <iostream>
#include "InsertionSort.h"
#include "BubbleSort.h"
#include "QuickSort.h"
#include <chrono>

/*---------------------------------------------------------------------------------------------------------------------
	This program takes five integer arrays with different sizes and randomized
	elements, and sorts them using different sorting algorithms. The sorting is
	repeated multiple times for each sorting algorithm and each array.
	Average sorting time is measured for each sorting algorithm and for each array. 
---------------------------------------------------------------------------------------------------------------------*/

const int arr1Size{ 10 };
const int arr2Size{ 50 };
const int arr3Size{ 100 };
const int arr4Size{ 500 };
const int arr5Size{ 1000 };

int arr1[arr1Size];
int arr2[arr2Size];
int arr3[arr3Size];
int arr4[arr4Size];
int arr5[arr5Size];

InsertionSort ins{};
BubbleSort bub{};
QuickSort qui{};

int repeatMeasurements{ 100 };
std::chrono::duration<double, std::milli> insertionSortTotalTime{};
std::chrono::duration<double, std::milli> bubbleSortTotalTime{};
std::chrono::duration<double, std::milli> quickSortTotalTime{};
std::chrono::duration<double, std::milli> zero{ std::chrono::duration<double>::zero() };

int instanceArray[arr5Size];

auto currentPointInTime() {
	return  std::chrono::steady_clock::now();
}

void copyFromArgumentArray(int arr[], int arrSize) { //copies the elements of the argument array (arr[]) to instanceArray.
	memcpy(instanceArray, arr, arrSize * 4); //*4 because of 4 or 8 byte to pointer relativity, otherwise the array doesn't copy every element. Need to find a way to do this properly.
}

void performSpecifiedSortMethod(int sortMethod, int arrSize) { // perform specified sort method.
	switch (sortMethod) {
	case 1:
		ins.insertionSort(instanceArray, arrSize);
		break;
	case 2:
		bub.bubbleSort(instanceArray, arrSize);
		break;
	case 3:
		qui.quickSort(instanceArray, 0, arrSize - 1);
		break;
	}
}

void addToTotalTime(int sortMethod, std::chrono::duration<double, std::milli> elapsedSeconds) { //increases total time each time a sort method is performed, later to be used to calculate an average time.
	switch (sortMethod) {
	case 1:
		insertionSortTotalTime = insertionSortTotalTime + elapsedSeconds;
		break;
	case 2:
		bubbleSortTotalTime = bubbleSortTotalTime + elapsedSeconds;
		break;
	case 3:
		quickSortTotalTime = quickSortTotalTime + elapsedSeconds;
		break;
	}
}

std::chrono::duration<double, std::milli> sortReturnTime(int sortMethod, int arr[], int arrSize) { //calls functions for setting up arrays for sorting and performing sort methods. Also takes time measurement and stores it in a variable.
	
	copyFromArgumentArray(arr, arrSize);

	const auto start{ currentPointInTime() };

	performSpecifiedSortMethod(sortMethod, arrSize); // perform specified sort method.

	const auto end{ currentPointInTime() };

	const std::chrono::duration<double, std::milli> elapsedSeconds{ end - start };

	addToTotalTime(sortMethod, elapsedSeconds);				
	return elapsedSeconds; //(milliseconds)
}

void populateArray(int arr[], int arrSize) { //populate our array with random numbers
	for (int i = 0; i < arrSize; i++) {
		arr[i] = rand() % arrSize;
		//std::cout << arr[i] << ' ';
	}
}

void performMeasurements(int arr[], int arrSize) { //printing tables of time measurements. Calls functions that call functions that perform sorting and time keeping inline of std::cout.
	std::cout << "-----------------------------------------Array Size: " << arrSize;
	//reset numerator for average calculations so it doesn't pile up and cause inaccurate results.
	insertionSortTotalTime = zero;
	bubbleSortTotalTime = zero;
	quickSortTotalTime = zero;

	int insertionSort{ 1 };
	int bubbleSort{ 2 };
	int quickSort{ 3 };

	int iterator{};

	//cout sort times.
	while (iterator < repeatMeasurements) {	
		std::cout << "\n \n \n----Sorting cycle " << iterator + 1 << " (array size: " << arrSize << ")" << '\n';
		std::cout << "\nTABLE:\n";
		std::cout << std::left << std::setw(20) << "Sort Method" << "Sorting Time" << "\n \n";
		std::cout << std::left << std::setw(20) << "Insertion Sort" << sortReturnTime(insertionSort, arr, arrSize) << '\n';
		std::cout << std::left << std::setw(20) << "Bubble Sort" << sortReturnTime(bubbleSort, arr, arrSize) << '\n';
		std::cout << std::left << std::setw(20) << "Quick Sort" << sortReturnTime(quickSort, arr, arrSize) << '\n';
		iterator++;
	}

	//cout average sort times.
	std::cout << "\n \n \n----Calculate averages" << " (array size: " << arrSize << ")" << '\n';
	std::cout << "\nTABLE (avg):\n";
	std::cout << std::left << std::setw(20) << "Sort Method" << "Sorting Time (avg)" << "\n \n";
	std::cout << std::left << std::setw(20) << "Insertion Sort" << insertionSortTotalTime / repeatMeasurements << " (avg)" << '\n';
	std::cout << std::left << std::setw(20) << "Bubble Sort" << bubbleSortTotalTime / repeatMeasurements << " (avg)" << '\n';
	std::cout << std::left << std::setw(20) << "Quick Sort" << quickSortTotalTime / repeatMeasurements << " (avg)" << '\n';
	std::cout << "\n \n \n \n";
}

int main()
{
	populateArray(arr1, arr1Size);
	populateArray(arr2, arr2Size);
	populateArray(arr3, arr3Size);
	populateArray(arr4, arr4Size);
	populateArray(arr5, arr5Size);

	performMeasurements(arr1, arr1Size);
	performMeasurements(arr2, arr2Size);
	performMeasurements(arr3, arr3Size);
	performMeasurements(arr4, arr4Size);
	performMeasurements(arr5, arr5Size);


	return 0;
}





//(G) "measure time spent on unsorted data" = "measure time spent sorting unsorted data"
//(VG) "measure them on both unsorted and sorted data" = measure time spent sorting unsorted data and time spent sorting already sorted data
//                       (i.e. having the algorithm go through the array and see how long it takes for it to figure out it is already sorted).