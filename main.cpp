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

#include "selectionSort.h"
#include "quickSort.h"
#include "mergeSort.h"
#include "insertionSort.h"
#include "heapSort.h"
#include "bubbleSort.h"
#include "splaySort.h"

#include "datasetGenerator.h"

using namespace std;

std::mutex mtx;

std::string csv = "Interation;Algorithm;DatasetName;DatasetSize;Time;Counter Comparisons;Counter Movements\n";

void runExperiment(
    int iteration, 
    const std::string& functionName,
    const std::function<std::pair<std::vector<int>, 
    std::pair<int, int>>(std::vector<int>)>& method,
    const std::vector<int>& dataset, 
    const std::string& datasetName, 
    std::ofstream& file
) {

    std::clock_t start = std::clock();
    auto result = method(dataset);
    std::clock_t end = std::clock();
    long double diff = 1000.0 * (end - start) / CLOCKS_PER_SEC;

    {
        std::lock_guard<std::mutex> lock(mtx);
        file << iteration + 1 << ";" << functionName << ";" << datasetName << ";" 
             << dataset.size() << ";" << diff << ";" << result.second.first << ";" 
             << result.second.second << "\n";
    }
}

int main()
{
    std::vector<int> lengthLists = {10, 100, 1000, 10000, 100000, 1000000};
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

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);

    char filename[100];
    std::strftime(filename, sizeof(filename), "output_%Y-%m-%d_%H-%M-%S.csv", now_tm);

    std::ofstream file(filename);

    if (file.is_open()) {

        file << "Iteration;Algorithm;DatasetName;DatasetSize;Time;Counter Comparisons;Counter Movements\n";

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

            for (auto function = methods.begin(); function != methods.end(); ++function) {

                std::vector<std::thread> threads;

                for (int i = 0; i < datasets.size(); ++i) {
                    for (int j = 0; j < datasets[i].size(); ++j) {
                        threads.emplace_back(runExperiment, iteration, function->first, function->second, datasets[i][j], datasets_name[i], std::ref(file));
                    }
                }

                for (int i = 0; i < threads.size(); ++i) {
                    threads[i].join();
                }
            }

            
        }

        file.close();
    }

    return 0;
}