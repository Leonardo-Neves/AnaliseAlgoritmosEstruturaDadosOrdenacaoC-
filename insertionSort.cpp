#include <iostream>
#include <vector>

using namespace std;

pair<vector<int>, pair<int, int>>  insertionSort(vector<int> array);

pair<vector<int>, pair<int, int>> insertionSort(vector<int> array) {

    int counter_comparisons = 0;
    int counter_movements = 0;

    for (int i = 1; i < array.size(); ++i) {
        counter_comparisons++;

        counter_movements += 2;
        int aux = array[i];
        int j = i - 1;

        for (int k = j; k >= 0; --k) {
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