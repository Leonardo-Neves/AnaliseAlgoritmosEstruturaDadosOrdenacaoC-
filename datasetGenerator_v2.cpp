#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <unordered_set>

#include "datasetGenerator_v2.h"

DatasetGenerator::DatasetGenerator() {}

std::vector<long long> DatasetGenerator::generateOrdered(long long length) {
    
    std::vector<long long> orderedList(length);
    std::iota(orderedList.begin(), orderedList.end(), 1);

    return orderedList;
}

std::vector<long long> DatasetGenerator::generateOrderedInverse(long long length) {
    
    std::vector<long long> orderedInverseList(length);
    std::iota(orderedInverseList.begin(), orderedInverseList.end(), 1);
    std::reverse(orderedInverseList.begin(), orderedInverseList.end());
      
    return orderedInverseList;
}

std::vector<long long> DatasetGenerator::generateAlmostOrdered(long long length) {
    
    std::random_device rd;
    std::mt19937 g(rd());
    
    std::vector<long long> half(length / 2);
    std::iota(half.begin(), half.end(), 1);
    std::vector<long long> remaining(length - half.size());
    std::iota(remaining.begin(), remaining.end(), half.size() + 1);
    std::shuffle(remaining.begin(), remaining.end(), g);
    
    // Verifica números únicos
    std::unordered_set<long long> uniqueNumbers(half.begin(), half.end());
    for (long long num : remaining) {
        while (uniqueNumbers.count(num) > 0) {
            num = std::uniform_int_distribution<>(half.size() + 1, length)(g);
        }
        uniqueNumbers.insert(num);
        half.push_back(num);
    }
        
    return half;
}


std::vector<long long> DatasetGenerator::generateRandom(long long length) {
    
    std::random_device rd;
    std::mt19937 g(rd());

    std::vector<long long> randomList(length);
    
    std::unordered_set<long long> uniqueNumbers;
    for (long long i = 0; i < length; ++i) {
        long long num = std::uniform_int_distribution<>(1, length * 2)(g);
        while (uniqueNumbers.count(num) > 0) {
            num = std::uniform_int_distribution<>(1, length * 2)(g);
        }
        randomList[i] = num;
        uniqueNumbers.insert(num);
    }
    
    return randomList;
}