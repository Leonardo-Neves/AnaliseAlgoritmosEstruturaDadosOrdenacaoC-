#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <chrono>
#include <fstream>
#include <random>
#include <ctime>
#include <mutex>
#include <thread>
#include <atomic>
#include <iomanip>

#include "splaySortV5.h"

#include "datasetGenerator_v2.h"

int main()
{
    DatasetGenerator datasetGenerator;

    auto result = datasetGenerator.generateOrderedInverse(1000000);

    long long counter_comparisons = 0;
    long long counter_movements = 0;

    auto result2 = splaySort(&result, &counter_comparisons, &counter_movements);

    for (int i : result2) {
        std::cout << i << " ";
    }

    std::cout << "\n";

    return 0;
}