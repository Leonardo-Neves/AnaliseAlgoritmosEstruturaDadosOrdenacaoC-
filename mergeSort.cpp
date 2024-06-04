#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

pair<vector<int>, pair<int, int>> interleave(vector<int> A, int initial_index, int middle_index, int end_index, int counter_comparisons, int counter_movements);
pair<vector<int>, pair<int, int>> mergeSort(vector<int> array);

pair<vector<int>, pair<int, int>> interleave(vector<int> A, int initial_index, int middle_index, int end_index, int counter_comparisons, int counter_movements) {
    vector<int> B(end_index - initial_index + 1);

    // Copy elements from A to B
    for (int i = initial_index; i <= middle_index; ++i) {
        counter_comparisons++;

        counter_movements++;
        B[i - initial_index] = A[i];
    }

    // Copy elements from A to B in reverse order
    for (int j = middle_index + 1; j <= end_index; ++j) {
        counter_comparisons++;

        counter_movements++;
        B[end_index + middle_index + 1 - j - initial_index] = A[j];
    }

    counter_movements += 2;
    int i = initial_index;
    int j = end_index;

    // Merge sorted halves from B back into A
    for (int k = initial_index; k <= end_index; ++k) {
        counter_comparisons++;

        counter_comparisons++;
        if (B[i - initial_index] <= B[j - initial_index]) {
            counter_movements += 2;
            A[k] = B[i - initial_index];
            i++;
        } else {
            counter_movements += 2;
            A[k] = B[j - initial_index];
            j--;
        }
    }

    return make_pair(A, make_pair(counter_comparisons, counter_movements));
}

pair<vector<int>, pair<int, int>> mergeSort(vector<int> array) {

    int counter_comparisons = 0;
    int counter_movements = 0;

    counter_movements += 3;
    vector<int> A = array;
    int n = array.size();
    int i = 1;

    while (i < n) {
        counter_comparisons++;

        for (int initial_index = 0; initial_index < n; initial_index += 2 * i) {
            counter_comparisons++;

            counter_movements += 2;
            int middle_index = initial_index + i - 1;
            int end_index = min(initial_index + 2 * i - 1, n - 1);

            counter_comparisons++;
            if (middle_index < end_index) {
                auto result = interleave(A, initial_index, middle_index, end_index, counter_comparisons, counter_movements);

                A = result.first;
                counter_comparisons = result.second.first;
                counter_movements = result.second.second;
                counter_movements++;
            }
        }

        counter_movements++;
        i *= 2;
    }

    return make_pair(A, make_pair(counter_comparisons, counter_movements));
}