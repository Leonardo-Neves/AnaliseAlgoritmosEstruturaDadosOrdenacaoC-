#include <iostream>
#include <vector>

using namespace std;

pair<vector<long long>, pair<long long, long long>> selectionSort(vector<long long> array) {
    vector<long long> A = array;
    long long aux = 0, n = array.size();
    
    long long counter_comparisons = 0;
    long long counter_movements = 0;

    for (long long i = 0; i < n - 1; ++i) {
        counter_comparisons++;

        long long min = i;

        for (long long j = i + 1; j < n; ++j) {
            counter_comparisons++;

            if (A[j] < A[min]) {
                counter_comparisons++;

                min = j;
                counter_movements++;
            }
        }

        counter_comparisons++;
        if (i != min) {
            aux = A[min];
            A[min] = A[i];
            A[i] = aux;
            counter_movements += 3;
        }
    }

    return make_pair(A, make_pair(counter_comparisons, counter_movements));
}