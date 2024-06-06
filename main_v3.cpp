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
// #include "splaySort.h"
#include "splaySortNovo.h"

#include "datasetGenerator_v2.h"

using namespace std;

std::mutex mtx;

void runExperiment(
    long long iteration, 
    const std::string& functionName,
    const std::function<std::pair<std::vector<long long>, 
    std::pair<long long, long long>>(std::vector<long long>)>& method,
    const std::vector<long long>& dataset, 
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

void runInteration(std::map<std::string, std::pair<std::vector<long long>, std::pair<long long, long long>>(*)(std::vector<long long>)> methods, long long iteration, std::vector<std::vector<std::vector<long long>>> datasets, std::vector<std::string> datasets_name, std::string posfixo) {

    for (auto function = methods.begin(); function != methods.end(); ++function) {

        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_time);

        char time_str[100];
        std::strftime(time_str, sizeof(time_str), "%Y-%m-%d_%H-%M-%S", now_tm);
        std::string filename = "/home/leo/AnaliseAlgoritmosEstruturaDadosOrdenacaoC-/output/output_" + function->first + "_" + std::string(time_str) + "_" + std::to_string(iteration) + "_" + posfixo + ".csv";
        std::ofstream file(filename);

        if (file.is_open()) {
            file << "Iteration;Algorithm;DatasetName;DatasetSize;Time;Counter Comparisons;Counter Movements\n";

            for (long long i = 0; i < datasets.size(); ++i) {

                std::vector<std::thread> threads;

                for (long long j = 0; j < datasets[i].size(); ++j) {
                    threads.emplace_back(runExperiment, iteration, function->first, function->second, datasets[i][j], datasets_name[i], std::ref(file));
                }

                for (long long i = 0; i < threads.size(); ++i) {
                    threads[i].join();
                }
            }
        }

        file.close();
    }
}

std::vector<std::vector<std::vector<long long>>> generateDataset(std::vector<long long> lengthLists, long long repeat) {

    DatasetGenerator datasetGenerator;

    std::vector<std::vector<std::vector<long long>>> datasets;

    std::vector<std::vector<long long>> datasets_temporary_ordered;

    for (long long i = 0; i < lengthLists.size(); ++i) {
        datasets_temporary_ordered.push_back(datasetGenerator.generateOrdered(lengthLists[i]));
    }

    datasets.push_back(datasets_temporary_ordered);

    std::vector<std::vector<long long>> datasets_temporary_ordered_inverse;

    for (long long i = 0; i < lengthLists.size(); ++i) {
        datasets_temporary_ordered_inverse.push_back(datasetGenerator.generateOrderedInverse(lengthLists[i]));
    }

    datasets.push_back(datasets_temporary_ordered_inverse);

    std::vector<std::vector<long long>> datasets_temporary_almost_ordered;

    for (long long i = 0; i < lengthLists.size(); ++i) {
        datasets_temporary_almost_ordered.push_back(datasetGenerator.generateAlmostOrdered(lengthLists[i]));
    }

    datasets.push_back(datasets_temporary_almost_ordered);

    std::vector<std::vector<long long>> datasets_temporary_random;

    for (long long i = 0; i < lengthLists.size(); ++i) {
        datasets_temporary_random.push_back(datasetGenerator.generateRandom(lengthLists[i]));
    }

    datasets.push_back(datasets_temporary_random);

    return datasets;
}

int main()
{
    long long NUMBER_INTERATIONS = 1;

    std::map<std::string, std::pair<std::vector<long long>, std::pair<long long, long long>>(*)(std::vector<long long>)> methods;
    methods["selectionSort"] = selectionSort;
    methods["quickSort"] = quickSort;
    methods["mergeSort"] = mergeSort;
    methods["insertionSort"] = insertionSort;
    methods["heapSort"] = heapSort;
    methods["bubbleSort"] = bubbleSort;
    methods["splaySort"] = splaySort;

    std::vector<std::string> datasets_name = {
        "Ordered",
        "OrderedInverse",
        "AlmostOrdered",
        "Random"
    };

    for (long long i = 0; i < NUMBER_INTERATIONS; ++i) {

        std::vector<long long> lengthLists1 = {10, 100, 1000, 10000};

        std::vector<std::vector<std::vector<long long>>> datasets1 = generateDataset(lengthLists1, datasets_name.size());

        runInteration(methods, i, datasets1, datasets_name, "1");

        std::vector<long long> lengthLists2 = {100000};

        std::vector<std::vector<std::vector<long long>>> datasets2 = generateDataset(lengthLists2, datasets_name.size());

        runInteration(methods, i, datasets2, datasets_name, "2");
    }

    return 0;
}