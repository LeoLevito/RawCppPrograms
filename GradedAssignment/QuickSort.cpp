#include "QuickSort.h"
int QuickSort::partition(int arr[], int start, int end) {
	int pivot{ arr[end] };
	int i{ start - 1 };
	int j{};
	for (j = start; j <= (end - 1); j++) {
		if (arr[j] < pivot) {
			i++;
			int temp{};
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}
	int temp2{ arr[i + 1] };
	arr[i + 1] = arr[end];
	arr[end] = temp2;
	return (i + 1);
}

void QuickSort::quickSort(int arr[], int start, int end) {
	if (start < end) {
		int partitionIndex{ partition(arr,start,end) };
		quickSort(arr, start, partitionIndex - 1);
		quickSort(arr, partitionIndex + 1, end);
	}
}
