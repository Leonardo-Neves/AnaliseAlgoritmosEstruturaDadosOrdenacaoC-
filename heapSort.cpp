#include <vector>
#include <iostream>

using namespace std;

pair<vector<long long>, pair<long long, long long>> reDoMaximum(vector<long long> A, long long esq, long long dir, long long counter_comparisons, long long counter_movements);
pair<vector<long long>, pair<long long, long long>> buildMaximum(vector<long long> A, long long n, long long counter_comparisons, long long counter_movements);
pair<vector<long long>, pair<long long, long long>> heapSort(vector<long long> A);

pair<vector<long long>, pair<long long, long long>> reDoMaximum(vector<long long> A, long long esq, long long dir, long long counter_comparisons, long long counter_movements) {
    long long i = esq;
    long long j = i * 2 + 1;
    long long aux = A[i];
    counter_movements+=3;

    while (j <= dir) {
        counter_comparisons++;

        if ((j < dir) && (A[j] < A[j + 1])) {
            counter_comparisons++;
            j = j + 1;
            counter_movements++;
        }

        if (aux >= A[j]) {
            counter_comparisons++;
            break;
        }

        A[i] = A[j];
        i = j;
        j = i * 2 + 1;
        counter_movements += 3; 
    }

    A[i] = aux;
    counter_movements++;

    return make_pair(A, make_pair(counter_comparisons, counter_movements));
}

pair<vector<long long>, pair<long long, long long>> buildMaximum(vector<long long> A, long long n, long long counter_comparisons, long long counter_movements) {
    for (long long esq = (n / 2) - 1; esq >= 0; esq--) {
        counter_comparisons++;

        auto result = reDoMaximum(A, esq, n - 1, counter_comparisons, counter_movements);
        A = result.first;
        counter_comparisons = result.second.first;
        counter_movements = result.second.second;
        counter_movements++;
    }

    return make_pair(A, make_pair(counter_comparisons, counter_movements));
}

pair<vector<long long>, pair<long long, long long>> heapSort(vector<long long> A) {
    long long counter_comparisons = 0;
    long long counter_movements = 0;

    auto result = buildMaximum(A, A.size(), counter_comparisons, counter_movements);

    A = result.first;
    counter_comparisons = result.second.first;
    counter_movements = result.second.second;
    counter_movements++;

    for (long long m = A.size() - 1; m > 0; m--) {
        counter_comparisons++;
        
        swap(A[0], A[m]);
        auto result = reDoMaximum(A, 0, m - 1, counter_comparisons, counter_movements);
        A = result.first;
        counter_comparisons = result.second.first;
        counter_movements = result.second.second;
        counter_movements+=2;
    }

    return make_pair(A, make_pair(counter_comparisons, counter_movements));
}