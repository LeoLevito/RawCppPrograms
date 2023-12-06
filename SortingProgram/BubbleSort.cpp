#include "BubbleSort.h"
void BubbleSort::bubbleSort(int arr[], int n) { //(actually Optimized Bubble sort since I figured this would be better than the basic Bubble sort I had before. Thanks for the feedback Emil!)
	int i{}, j{}, temp{};
    bool swapped{};
	for (i = 0; i < n - 1; i++) {
        swapped = false;
		for (j = 0; j < n - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
                swapped = true;
			}
		}
        if (!swapped) {
            break;
        }
	}
}