#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>

class DatasetGenerator {
public:
    DatasetGenerator(const std::vector<int>& lengthLists) : lengthLists(lengthLists) {}

    std::vector<std::vector<int>> generateOrdered() {
        std::vector<std::vector<int>> result;
        for (int length : lengthLists) {
            std::vector<int> orderedList(length);
            std::iota(orderedList.begin(), orderedList.end(), 1);
            result.push_back(orderedList);
        }
        return result;
    }

    std::vector<std::vector<int>> generateOrderedInverse() {
        std::vector<std::vector<int>> result;
        for (int length : lengthLists) {
            std::vector<int> orderedInverseList(length);
            std::iota(orderedInverseList.begin(), orderedInverseList.end(), 1);
            std::reverse(orderedInverseList.begin(), orderedInverseList.end());
            result.push_back(orderedInverseList);
        }
        return result;
    }

    std::vector<std::vector<int>> generateAlmostOrdered() {
        std::vector<std::vector<int>> result;
        std::random_device rd;
        std::mt19937 g(rd());
        for (int length : lengthLists) {
            std::vector<int> half(length / 2);
            std::iota(half.begin(), half.end(), 1);
            std::vector<int> remaining(length - half.size());
            std::iota(remaining.begin(), remaining.end(), half.size() + 1);
            std::shuffle(remaining.begin(), remaining.end(), g);
            half.insert(half.end(), remaining.begin(), remaining.end());
            result.push_back(half);
        }
        return result;
    }

    std::vector<std::vector<int>> generateRandom() {
        std::vector<std::vector<int>> result;
        std::random_device rd;
        std::mt19937 g(rd());
        for (int length : lengthLists) {
            std::vector<int> randomList(length);
            std::generate(randomList.begin(), randomList.end(), [length]() {
                static std::random_device rd;
                static std::mt19937 g(rd());
                static std::uniform_int_distribution<> dis(1, length * 2);
                return dis(g);
            });
            result.push_back(randomList);
        }
        return result;
    }

private:
    std::vector<int> lengthLists;
};

int main() {
    std::vector<int> lengthLists = {10, 100, 1000, 10000, 100000, 1000000};
    DatasetGenerator datasetGenerator(lengthLists);

    // Generate ordered datasets
    auto orderedDatasets = datasetGenerator.generateOrdered();
    // sample of dataset ordered with lenght 10
    for (int i : orderedDatasets[0]) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Generate ordered inverse datasets
    auto orderedInverseDatasets = datasetGenerator.generateOrderedInverse();
    //  sample of dataset ordered with lenght 10
    for (int i : orderedInverseDatasets[0]) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Generate almost ordered datasets
    auto almostOrderedDatasets = datasetGenerator.generateAlmostOrdered();
    //  sample of dataset ordered with lenght 10
    for (int i : almostOrderedDatasets[0]) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Generate random datasets
    auto randomDatasets = datasetGenerator.generateRandom();
    // sample of dataset ordered with lenght 10
    for (int i : randomDatasets[0]) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}