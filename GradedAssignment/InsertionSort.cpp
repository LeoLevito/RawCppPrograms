#include "InsertionSort.h"
void InsertionSort::insertionSort(int arr[], int n) {
	int i{}, j{}, temp{};
	for (i = 1; i < n; i++) {
		temp = arr[i];
		j = i - 1;
		while (j >= 0 && temp <= arr[j]) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = temp;
	}
}