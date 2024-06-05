#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <chrono>
#include <fstream>
#include <random>
#include <thread>
#include <mutex>

#include "selectionSort.h"
#include "quickSort.h"
#include "mergeSort.h"
#include "insertionSort.h"
#include "heapSort.h"
#include "bubbleSort.h"
#include "splaySort.h"

#include "datasetGenerator.h"

using namespace std;

std::mutex csv_mutex;

void runExperiment(int iteration, const std::string& algorithmName, const std::function<std::pair<std::vector<int>, std::pair<int, int>>(std::vector<int>)>& sortFunction, const std::vector<int>& dataset, const std::string& datasetName, std::string& csv) {
    std::clock_t start = std::clock();
    auto result = sortFunction(dataset);
    std::clock_t end = std::clock();

    long double diff = 1000.0 * (end - start) / CLOCKS_PER_SEC;

    std::string row = std::to_string(iteration + 1) + ";" + algorithmName + ";" + datasetName + ";" + std::to_string(dataset.size()) + ";" + std::to_string(diff) + ";" + std::to_string(result.second.first) + ";" + std::to_string(result.second.second) + "\n";

    std::lock_guard<std::mutex> guard(csv_mutex);
    csv += row;
    // std::cout << "Unable to open file";
}

int main() {
    std::vector<int> lengthLists = {10, 100, 1000, 10000};
    int NUMBER_INTERATIONS = 1;

    DatasetGenerator datasetGenerator(lengthLists);

    std::map<std::string, std::pair<std::vector<int>, std::pair<int, int>>(*)(std::vector<int>)> methods;
    methods["selectionSort"] = selectionSort;
    methods["quickSort"] = quickSort;
    methods["mergeSort"] = mergeSort;
    methods["insertionSort"] = insertionSort;
    methods["heapSort"] = heapSort;
    methods["bubbleSort"] = bubbleSort;
    methods["splaySort"] = splaySort;

    std::string csv = "Interation;Algorithm;DatasetName;DatasetSize;Time;Counter Comparisons;Counter Movements\n";

    for (int iteration = 0; iteration < NUMBER_INTERATIONS; ++iteration) {
        std::vector<std::vector<std::vector<int>>> datasets {
            datasetGenerator.generateOrdered(),
            datasetGenerator.generateOrderedInverse(),
            datasetGenerator.generateAlmostOrdered(),
            datasetGenerator.generateRandom()
        };

        std::vector<std::string> datasets_name = {
            "Ordered",
            "OrderedInverse",
            "AlmostOrdered",
            "Random"
        };

        std::vector<std::thread> threads;

        for (auto function = methods.begin(); function != methods.end(); ++function) {
            for (int i = 0; i < datasets.size(); ++i) {
                for (int j = 0; j < datasets[i].size(); ++j) {
                    
                    threads.push_back(std::thread(runExperiment, iteration, function->first, function->second, datasets[i][j], datasets_name[i], std::ref(csv)));
                }
            }
        }

        // Join all threads
        for (std::thread &t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 10000000);
    int random_value = distrib(gen);

    std::ofstream file("output" + std::to_string(random_value) + ".csv");

    if (file.is_open()) {
        file << csv;
        file.close();
    } else {
        std::cout << "Unable to open file";
    }

    return 0;
}
