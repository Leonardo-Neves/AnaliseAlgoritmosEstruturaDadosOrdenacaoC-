#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

pair<vector<int>, pair<int, int>> quickSort(vector<int> array);
pair<vector<int>, pair<int, int>> quicksort_ordena(vector<int> array, int p, int r, int counter_comparisons, int counter_movements);
tuple<vector<int>, int, int, pair<int, int>> quicksort_particao_ternaria(vector<int> array, int p, int r, int counter_comparisons, int counter_movements);

pair<vector<int>, pair<int, int>> quickSort(vector<int> array) {

    int counter_comparisons = 0;
    int counter_movements = 0;

    counter_movements++;
    int n = array.size();

    auto result = quicksort_ordena(array, 0, n - 1, counter_comparisons, counter_movements);
    counter_comparisons = result.second.first;
    counter_movements = result.second.second;
    counter_movements++;

    return make_pair(result.first, make_pair(counter_comparisons, counter_movements));
}

tuple<vector<int>, int, int, pair<int, int>> quicksort_particao_ternaria(vector<int> array, int p, int r, int counter_comparisons, int counter_movements) {

    counter_movements += 3;
    int x = array[r];
    int i = p - 1;
    int k = r;

    for (int j = p; j < k; ++j) {
        counter_comparisons++;

        if (array[j] < x) {
            counter_comparisons++;

            counter_movements += 2;
            i++;
            swap(array[i], array[j]);
        } else if (array[j] == x) {
            counter_comparisons++;

            counter_movements += 3;
            k--;
            swap(array[k], array[j]);
            j--;
        }
    }

    counter_movements++;
    int q = i + 1;

    for (int j = k; j <= r; ++j) {
        counter_comparisons++;

        counter_movements += 2;
        i++;
        swap(array[i], array[j]);
    }

    counter_movements++;
    int t = i;

    return make_tuple(array, q, t, make_pair(counter_comparisons, counter_movements));
}

pair<vector<int>, pair<int, int>> quicksort_ordena(vector<int> array, int p, int r, int counter_comparisons, int counter_movements) {
    while (p < r) {
        counter_comparisons++;

        auto result = quicksort_particao_ternaria(array, p, r, counter_comparisons, counter_movements);

        array = get<0>(result);
        int q = get<1>(result);
        int t = get<2>(result);
        counter_comparisons = get<3>(result).first;
        counter_movements = get<3>(result).second;
        counter_movements += 3;

        counter_comparisons++;
        if (q - p < r - t) {
            auto result = quicksort_ordena(array, p, q - 1, counter_comparisons, counter_movements);

            array = result.first;
            counter_comparisons = result.second.first;
            counter_movements = result.second.second;
            counter_movements++;

            counter_movements++;
            p = t + 1;
        } else {
            auto result = quicksort_ordena(array, t + 1, r, counter_comparisons, counter_movements);
            
            array = result.first;
            counter_comparisons = result.second.first;
            counter_movements = result.second.second;
            counter_movements++;
            
            counter_movements++;
            r = q - 1;
        }
    }

    return make_pair(array, make_pair(counter_comparisons, counter_movements));
}
