#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> interleave(vector<int>& A, int initial_index, int middle_index, int end_index);
vector<int> mergeSort(vector<int>& array);

vector<int> interleave(vector<int>& A, int initial_index, int middle_index, int end_index) {
    vector<int> B(end_index - initial_index + 1);

    // Copy elements from A to B
    for (int i = initial_index; i <= middle_index; ++i) {
        B[i - initial_index] = A[i];
    }

    // Copy elements from A to B in reverse order
    for (int j = middle_index + 1; j <= end_index; ++j) {
        B[end_index + middle_index + 1 - j - initial_index] = A[j];
    }

    int i = initial_index;
    int j = end_index;

    // Merge sorted halves from B back into A
    for (int k = initial_index; k <= end_index; ++k) {
        if (B[i - initial_index] <= B[j - initial_index]) {
            A[k] = B[i - initial_index];
            i++;
        } else {
            A[k] = B[j - initial_index];
            j--;
        }
    }

    return A;
}

vector<int> mergeSort(vector<int>& array) {
    vector<int> A = array;
    int n = array.size();
    int i = 1;

    while (i < n) {
        for (int initial_index = 0; initial_index < n; initial_index += 2 * i) {
            int middle_index = initial_index + i - 1;
            int end_index = min(initial_index + 2 * i - 1, n - 1);

            if (middle_index < end_index) {
                A = interleave(A, initial_index, middle_index, end_index);
            }
        }
        i *= 2;
    }

    return A;
}