#include <iostream>
#include <vector>

using namespace std;

pair<vector<long long>, pair<long long, long long>> bubbleSort(std::vector<long long> array);

pair<vector<long long>, pair<long long, long long>> bubbleSort(std::vector<long long> array) {

    long long counter_comparisons = 0;
    long long counter_movements = 0;

    long long n = array.size();
    long long n_trocas = 0;
    counter_movements += 2;

    for (long long i = 0; i < n - 1; ++i) {
        counter_comparisons++;

        n_trocas = 0;
        counter_movements++;

        for (long long j = 1; j < n - i; ++j) {
            counter_comparisons++;

            if (array[j] < array[j - 1]) {
                counter_comparisons++;
                
                counter_movements += 4;
                long long aux = array[j];
                array[j] = array[j - 1];
                array[j - 1] = aux;
                n_trocas++;
            }
        }
        
        if (n_trocas == 0) {
            counter_comparisons++;
            break;
        }
    }

    return make_pair(array, make_pair(counter_comparisons, counter_movements));
}