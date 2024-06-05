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

std::mutex mtx;
std::atomic<int> threads_completed(0);

void printProgressBar(int completed, int total) {
    const int barWidth = 50;
    float progress = (float)completed / total;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

void runExperiment(int iteration, const std::string& functionName,
                   const std::function<std::pair<std::vector<int>, std::pair<int, int>>(std::vector<int>)>& method,
                   const std::vector<int>& dataset, const std::string& datasetName, std::ofstream& file,
                   int total_experiments) {

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

    threads_completed++;
    int completed = threads_completed.load();
    printProgressBar(completed, total_experiments);
}

int main() {
    std::vector<int> lengthLists = {10, 100, 1000, 10000, 100000, 1000000};
    int NUMBER_ITERATIONS = 1;

    DatasetGenerator datasetGenerator(lengthLists);

    std::map<std::string, std::function<std::pair<std::vector<int>, std::pair<int, int>>(std::vector<int>)>> methods;
    methods["selectionSort"] = selectionSort;
    methods["quickSort"] = quickSort;
    methods["mergeSort"] = mergeSort;
    methods["insertionSort"] = insertionSort;
    methods["heapSort"] = heapSort;
    methods["bubbleSort"] = bubbleSort;
    methods["splaySort"] = splaySort;

    std::vector<std::vector<std::vector<int>>> datasets = {
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

    // Get current time for the filename
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);

    char filename[100];
    std::strftime(filename, sizeof(filename), "output_%Y-%m-%d_%H-%M-%S.csv", now_tm);

    std::ofstream file(filename);

    if (file.is_open()) {
        file << "Iteration;Algorithm;DatasetName;DatasetSize;Time;Counter Comparisons;Counter Movements\n";

        std::vector<std::thread> threads;
        int total_experiments = NUMBER_ITERATIONS * methods.size() * datasets.size() * datasets[0].size();

        for (int iteration = 0; iteration < NUMBER_ITERATIONS; ++iteration) {
            for (auto& function : methods) {
                for (int i = 0; i < datasets.size(); ++i) {
                    for (int j = 0; j < datasets[i].size(); ++j) {
                        threads.emplace_back(runExperiment, iteration, function.first, function.second, 
                                             datasets[i][j], datasets_name[i], std::ref(file), total_experiments);
                    }
                }
            }
        }

        for (auto& thread : threads) {
            thread.join();
        }

        file.close();
    } else {
        std::cout << "Unable to open file";
    }

    // Print a new line after the progress bar is complete
    std::cout << std::endl;

    return 0;
}
