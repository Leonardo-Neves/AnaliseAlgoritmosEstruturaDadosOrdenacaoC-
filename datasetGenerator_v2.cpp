#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <unordered_set>

#include "datasetGenerator_v2.h"

DatasetGenerator::DatasetGenerator() {}

// Método responsável por gerar um vetor de dados ordenados
std::vector<long long> DatasetGenerator::generateOrdered(long long length) {
    
    std::vector<long long> orderedList(length);
    std::iota(orderedList.begin(), orderedList.end(), 1);

    return orderedList;
}

// Método responsável por gerar um vetor de dados ordenados inversamente
std::vector<long long> DatasetGenerator::generateOrderedInverse(long long length) {
    
    std::vector<long long> orderedInverseList(length);
    std::iota(orderedInverseList.begin(), orderedInverseList.end(), 1);
    std::reverse(orderedInverseList.begin(), orderedInverseList.end());
      
    return orderedInverseList;
}

// Método responsável por gerar um vetor de dados ordenados parcialmente
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

// Método responsável por gerar um vetor de dados ordenados randômicamente
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

// Método responsável por gerar um vetor de dados ordenados randômicamente com dados repetidos
std::vector<long long> DatasetGenerator::generateRandomWithRepeats(long long length) {
    std::vector<long long> result;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> dis(0, length);

    for (size_t i = 0; i < length; ++i) {
        result.push_back(dis(gen));
    }

    return result;
}
