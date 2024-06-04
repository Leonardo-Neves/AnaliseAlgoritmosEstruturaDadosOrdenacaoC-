#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <chrono>
#include <fstream>
#include <random>

#include "selectionSort.h"
#include "quickSort.h"
#include "mergeSort.h"
#include "insertionSort.h"
#include "heapSort.h"
#include "bubbleSort.h"
#include "splaySort.h"

#include "datasetGenerator.h"

using namespace std;

int main()
{
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

        for (auto function = methods.begin(); function != methods.end(); ++function) {

            for (int i = 0; i < datasets.size(); ++i) {
                for (int j = 0; j < datasets[i].size(); ++j) {

                    std::clock_t start = std::clock();
                    
                    auto result = methods[function->first](datasets[i][j]);

                    std::clock_t end = std::clock();

                    long double diff = 1000.0 * (end - start) / CLOCKS_PER_SEC;

                    csv += std::to_string(iteration + 1) + ";" + function->first + ";" + datasets_name[i] + ";" + std::to_string(datasets[i][j].size()) + ";" + std::to_string(diff) + ";" + std::to_string(result.second.first) + ";" + std::to_string(result.second.second) + "\n";

                }
            }
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 10000000);
    int random_value = distrib(gen);

    std::ofstream file("output" + std::to_string(random_value) + ".csv");

    if (file.is_open())
    {
        file << csv;
        file.close();
    }
    else 
    {
        std::cout << "Unable to open file";
    }

    return 0;
}