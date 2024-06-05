#include <iostream>
#include <vector>

using namespace std;

pair<vector<long long>, pair<long long, long long>>  insertionSort(vector<long long> array);

pair<vector<long long>, pair<long long, long long>> insertionSort(vector<long long> array) {

    long long counter_comparisons = 0;
    long long counter_movements = 0;

    for (long long i = 1; i < array.size(); ++i) {
        counter_comparisons++;

        counter_movements += 2;
        long long aux = array[i];
        long long j = i - 1;

        for (long long k = j; k >= 0; --k) {
            counter_comparisons++;

            counter_comparisons++;
            if (aux < array[k]) {
                counter_movements++;
                array[k + 1] = array[k];
            } else {
                counter_movements++;
                k += 1;
                break;
            }
            counter_movements++;
            array[k] = aux;
        }
    }

    return make_pair(array, make_pair(counter_comparisons, counter_movements));
}