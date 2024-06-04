#ifndef DATASETGENERATOR_H
#define DATASETGENERATOR_H

#include <vector>

class DatasetGenerator {
public:
    DatasetGenerator(const std::vector<int>& lengthLists);

    std::vector<std::vector<int>> generateOrdered();
    std::vector<std::vector<int>> generateOrderedInverse();
    std::vector<std::vector<int>> generateAlmostOrdered();
    std::vector<std::vector<int>> generateRandom();

private:
    std::vector<int> lengthLists;
};

#endif // DATASETGENERATOR_H
