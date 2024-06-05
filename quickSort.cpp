#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

pair<vector<long long>, pair<long long, long long>> quickSort(vector<long long> array);
pair<vector<long long>, pair<long long, long long>> quicksort_ordena(vector<long long> array, long long p, long long r, long long counter_comparisons, long long counter_movements);
tuple<vector<long long>, long long, long long, pair<long long, long long>> quicksort_particao_ternaria(vector<long long> array, long long p, long long r, long long counter_comparisons, long long counter_movements);

pair<vector<long long>, pair<long long, long long>> quickSort(vector<long long> array) {

    long long counter_comparisons = 0;
    long long counter_movements = 0;

    counter_movements++;
    long long n = array.size();

    auto result = quicksort_ordena(array, 0, n - 1, counter_comparisons, counter_movements);
    counter_comparisons = result.second.first;
    counter_movements = result.second.second;
    counter_movements++;

    return make_pair(result.first, make_pair(counter_comparisons, counter_movements));
}

tuple<vector<long long>, long long, long long, pair<long long, long long>> quicksort_particao_ternaria(vector<long long> array, long long p, long long r, long long counter_comparisons, long long counter_movements) {

    counter_movements += 3;
    long long x = array[r];
    long long i = p - 1;
    long long k = r;

    for (long long j = p; j < k; ++j) {
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
    long long q = i + 1;

    for (long long j = k; j <= r; ++j) {
        counter_comparisons++;

        counter_movements += 2;
        i++;
        swap(array[i], array[j]);
    }

    counter_movements++;
    long long t = i;

    return make_tuple(array, q, t, make_pair(counter_comparisons, counter_movements));
}

pair<vector<long long>, pair<long long, long long>> quicksort_ordena(vector<long long> array, long long p, long long r, long long counter_comparisons, long long counter_movements) {
    while (p < r) {
        counter_comparisons++;

        auto result = quicksort_particao_ternaria(array, p, r, counter_comparisons, counter_movements);

        array = get<0>(result);
        long long q = get<1>(result);
        long long t = get<2>(result);
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
