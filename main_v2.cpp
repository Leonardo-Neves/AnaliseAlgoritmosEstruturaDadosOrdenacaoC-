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

void runInteration(std::map<std::string, std::pair<std::vector<int>, std::pair<int, int>>(*)(std::vector<int>)> methods, int iteration, std::vector<std::vector<std::vector<int>>> datasets, std::string posfixo) {
    
    // std::vector<std::vector<std::vector<int>>> datasets_adjusted;
    // for (int i = 0; i < lengthLists.size(); ++i) {
    //     std::vector<std::vector<int>> datasets_temporary;
    //     for (int j = 0; j < datasets.size(); ++j) { 
    //         datasets_temporary.push_back(datasets[j][i]);
    //         std::cout << datasets[j][i].size() << " ";
    //     }
    //     std::cout << "\n";
    //     datasets_adjusted.push_back(datasets_temporary);
    // }

    std::vector<std::string> datasets_name = {
        "Ordered",
        "OrderedInverse",
        "AlmostOrdered",
        "Random"
    };

    for (auto function = methods.begin(); function != methods.end(); ++function) {

        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_time);

        char time_str[100];
        std::strftime(time_str, sizeof(time_str), "%Y-%m-%d_%H-%M-%S", now_tm);
        std::string filename = "output_" + function->first + "_" + std::string(time_str) + "_" + posfixo +".csv";
        std::ofstream file(filename);

        if (file.is_open()) {
            file << "Iteration;Algorithm;DatasetName;DatasetSize;Time;Counter Comparisons;Counter Movements\n";

            for (int i = 0; i < datasets.size(); ++i) {

                std::vector<std::thread> threads;

                for (int j = 0; j < datasets[i].size(); ++j) {
                    threads.emplace_back(runExperiment, iteration, function->first, function->second, datasets[i][j], datasets_name[i], std::ref(file));
                }

                for (int i = 0; i < threads.size(); ++i) {
                    threads[i].join();
                }
            }
        }

        file.close();
    }
}

int main()
{
    
    int NUMBER_INTERATIONS = 1;

    std::map<std::string, std::pair<std::vector<int>, std::pair<int, int>>(*)(std::vector<int>)> methods;
    methods["selectionSort"] = selectionSort;
    methods["quickSort"] = quickSort;
    methods["mergeSort"] = mergeSort;
    methods["insertionSort"] = insertionSort;
    methods["heapSort"] = heapSort;
    methods["bubbleSort"] = bubbleSort;
    methods["splaySort"] = splaySort;

    
    
    for (int i = 0; i < NUMBER_INTERATIONS; ++i) {

        std::vector<int> lengthLists1 = {10, 100, 1000, 10000};

        DatasetGenerator datasetGenerator1(lengthLists1);

        std::vector<std::vector<std::vector<int>>> datasets1 {
            datasetGenerator1.generateOrdered(),
            datasetGenerator1.generateOrderedInverse(),
            datasetGenerator1.generateAlmostOrdered(),
            datasetGenerator1.generateRandom()
        };

        runInteration(methods, i, datasets1, "1");

        std::vector<int> lengthLists2 = {100000};

        DatasetGenerator datasetGenerator2(lengthLists2);

        std::vector<std::vector<std::vector<int>>> datasets2 {
            datasetGenerator2.generateOrdered(),
            datasetGenerator2.generateOrderedInverse(),
            datasetGenerator2.generateAlmostOrdered(),
            datasetGenerator2.generateRandom()
        };

        runInteration(methods, i, datasets2, "2");

        std::vector<int> lengthLists3 = {1000000};

        DatasetGenerator datasetGenerator3(lengthLists3);

        std::vector<std::vector<std::vector<int>>> datasets3 {
            datasetGenerator3.generateOrdered(),
            datasetGenerator3.generateOrderedInverse(),
            datasetGenerator3.generateAlmostOrdered(),
            datasetGenerator3.generateRandom()
        };

        runInteration(methods, i, datasets3, "3");
    }

    return 0;
}