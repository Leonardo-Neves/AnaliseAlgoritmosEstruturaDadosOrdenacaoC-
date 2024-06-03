#include <iostream>
#include <vector>

using namespace std;

pair<vector<int>, pair<int, int>> selectionSort(vector<int> array) {
    vector<int> A = array;
    int aux = 0, n = array.size();
    
    int counter_comparisons = 0;
    int counter_movements = 0;

    for (int i = 0; i < n - 1; ++i) {
        counter_comparisons++;

        int min = i;

        for (int j = i + 1; j < n; ++j) {
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