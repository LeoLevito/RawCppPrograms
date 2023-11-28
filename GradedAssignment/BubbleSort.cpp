#include "BubbleSort.h"
void BubbleSort::bubbleSort(int arr[], int n) {
	int i{}, j{}, temp{};
	for (i = 0; i < n; i++) {
		for (j = (i + 1); j < n; j++) {
			if (arr[j] < arr[i]) {
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
}
