#include <iostream>
#include <vector>

#include "selectionSort.h"
#include "quickSort.h"
#include "mergeSort.h"
#include "insertionSort.h"
#include "heapSort.h"
#include "bubbleSort.h"

int main()
{
    std::vector<int> array = {64, 25, 12, 22, 11};
    auto result = bubbleSort(array);

    // std::vector<int> sorted_array = result;

    std::vector<int> sorted_array = result.first;
    int comparisons = result.second.first;
    int movements = result.second.second;

    std::cout << "Sorted array: ";
    for (int i = 0; i < sorted_array.size(); ++i) {
        std::cout << sorted_array[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Number of comparisons: " << comparisons << std::endl;
    std::cout << "Number of movements: " << movements << std::endl;

    return 0;
}