#include <iostream>
#include <vector>

using namespace std;

pair<vector<int>, pair<int, int>> bubbleSort(std::vector<int> array);

pair<vector<int>, pair<int, int>> bubbleSort(std::vector<int> array) {

    int counter_comparisons = 0;
    int counter_movements = 0;

    int n = array.size();
    int n_trocas = 0;
    counter_movements += 2;

    for (int i = 0; i < n - 1; ++i) {
        counter_comparisons++;

        n_trocas = 0;
        counter_movements++;

        for (int j = 1; j < n - i; ++j) {
            counter_comparisons++;

            if (array[j] < array[j - 1]) {
                counter_comparisons++;
                
                counter_movements += 4;
                int aux = array[j];
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