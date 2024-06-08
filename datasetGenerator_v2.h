#ifndef DATASETGENERATOR_V2_H
#define DATASETGENERATOR_V2_H

#include <vector>

class DatasetGenerator {
public:
    DatasetGenerator();

    std::vector<long long> generateOrdered(long long length);
    std::vector<long long> generateOrderedInverse(long long length);
    std::vector<long long> generateAlmostOrdered(long long length);
    std::vector<long long> generateRandom(long long length);
    std::vector<long long> generateRandomWithRepeats(long long length);

private:
};

#endif // DATASETGENERATOR_V2_H
