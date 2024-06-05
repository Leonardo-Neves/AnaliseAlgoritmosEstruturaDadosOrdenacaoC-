#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

pair<vector<long long>, pair<long long, long long>> interleave(vector<long long> A, long long initial_index, long long middle_index, long long end_index, long long counter_comparisons, long long counter_movements);
pair<vector<long long>, pair<long long, long long>> mergeSort(vector<long long> array);

pair<vector<long long>, pair<long long, long long>> interleave(vector<long long> A, long long initial_index, long long middle_index, long long end_index, long long counter_comparisons, long long counter_movements) {
    vector<long long> B(end_index - initial_index + 1);

    // Copy elements from A to B
    for (long long i = initial_index; i <= middle_index; ++i) {
        counter_comparisons++;

        counter_movements++;
        B[i - initial_index] = A[i];
    }

    // Copy elements from A to B in reverse order
    for (long long j = middle_index + 1; j <= end_index; ++j) {
        counter_comparisons++;

        counter_movements++;
        B[end_index + middle_index + 1 - j - initial_index] = A[j];
    }

    counter_movements += 2;
    long long i = initial_index;
    long long j = end_index;

    // Merge sorted halves from B back into A
    for (long long k = initial_index; k <= end_index; ++k) {
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

pair<vector<long long>, pair<long long, long long>> mergeSort(vector<long long> array) {

    long long counter_comparisons = 0;
    long long counter_movements = 0;

    counter_movements += 3;
    vector<long long> A = array;
    long long n = array.size();
    long long i = 1;

    while (i < n) {
        counter_comparisons++;

        for (long long initial_index = 0; initial_index < n; initial_index += 2 * i) {
            counter_comparisons++;

            counter_movements += 2;
            long long middle_index = initial_index + i - 1;
            long long end_index = min(initial_index + 2 * i - 1, n - 1);

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