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

#include "datasetGenerator_v2.h"

using namespace std;

std::mutex mtx;

// Este método é responsável em executar um algoritmo, medir o tempo de CPU time e gravar os resultados de movimentações, comparações e tempo de execução em um arquivo CSV
void runExperiment(long long iteration, const std::string& functionName, const std::function<std::pair<std::vector<long long>, std::pair<long long, long long>>(std::vector<long long>)>& method, const std::vector<long long>& dataset, const std::string& datasetName, std::ofstream& file) {

    // Executando o método e mensurando o tempo de execução
    std::clock_t start = std::clock();
    auto result = method(dataset);
    std::clock_t end = std::clock();

    long double diff = 1000.0 * (end - start) / CLOCKS_PER_SEC;

    // Registrando resultados no arquivo CSV
    {
        std::lock_guard<std::mutex> lock(mtx);
        file << iteration + 1 << ";" << functionName << ";" << datasetName << ";" 
             << dataset.size() << ";" << diff << ";" << result.second.first << ";" 
             << result.second.second << "\n";
    }
}

// Este método é responsável por executar um interação do código, o que envolve executar um conjunto de dados sobre uma série de métodos utilizando computação paralela
void runInteration(std::map<std::string, std::pair<std::vector<long long>, std::pair<long long, long long>>(*)(std::vector<long long>)> methods, long long iteration, std::vector<std::vector<std::vector<long long>>> datasets, std::vector<std::string> datasets_name, std::string output_path, std::string posfixo = "") {

    // Interando sobre cada método
    for (auto function = methods.begin(); function != methods.end(); ++function) {

        // Obtendo tempo atual
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_time);

        // Criando arquivo CSV de saída
        char time_str[100];
        std::strftime(time_str, sizeof(time_str), "%Y-%m-%d_%H-%M-%S", now_tm);
        std::string filename = output_path + "/output_" + function->first + "_" + std::string(time_str) + "_" + std::to_string(iteration) + "_" + posfixo + ".csv";
        std::ofstream file(filename);

        // Criando threads para executar os algoritmos em paralelo
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

// Este método é responsável por gerar um conjunto de dados baseado em um vetor de entrada, que em cada posição do vetor, um número indicando o comprimento do vetor que será gerado
std::vector<std::vector<std::vector<long long>>> generateDataset(std::vector<long long> lengthLists) {

    /*
        Exemplo de vetor de saída
        [
            [   Ordered
                [0 ... 10],
                [0 ... 100],
                [0 ... 1000],
                [0 ... 10000]
            ],
            [   OrderedInverse
                [0 ... 10],
                [0 ... 100],
                [0 ... 1000],
                [0 ... 10000]
            ],
            [   AlmostOrdered
                [0 ... 10],
                [0 ... 100],
                [0 ... 1000],
                [0 ... 10000]
            ],
            [   Random
                [0 ... 10],
                [0 ... 100],
                [0 ... 1000],
                [0 ... 10000]
            ],
        ]
    */

    // Criando uma instância do gerador de conjunto de dados
    DatasetGenerator datasetGenerator;

    std::vector<std::vector<std::vector<long long>>> datasets;

    // Gerando parte do conjunto de dados para dados ordenados
    std::vector<std::vector<long long>> datasets_temporary_ordered;
    for (long long i = 0; i < lengthLists.size(); ++i) {
        datasets_temporary_ordered.push_back(datasetGenerator.generateOrdered(lengthLists[i]));
    }
    datasets.push_back(datasets_temporary_ordered);

    // Gerando parte do conjunto de dados para dados ordenados inversamente
    std::vector<std::vector<long long>> datasets_temporary_ordered_inverse;
    for (long long i = 0; i < lengthLists.size(); ++i) {
        datasets_temporary_ordered_inverse.push_back(datasetGenerator.generateOrderedInverse(lengthLists[i]));
    }
    datasets.push_back(datasets_temporary_ordered_inverse);

    // Gerando parte do conjunto de dados para dados ordenados parcialmente
    std::vector<std::vector<long long>> datasets_temporary_almost_ordered;
    for (long long i = 0; i < lengthLists.size(); ++i) {
        datasets_temporary_almost_ordered.push_back(datasetGenerator.generateAlmostOrdered(lengthLists[i]));
    }
    datasets.push_back(datasets_temporary_almost_ordered);

    // Gerando parte do conjunto de dados para dados ordenados randômicamente
    std::vector<std::vector<long long>> datasets_temporary_random;
    for (long long i = 0; i < lengthLists.size(); ++i) {
        datasets_temporary_random.push_back(datasetGenerator.generateRandom(lengthLists[i]));
    }
    datasets.push_back(datasets_temporary_random);

    return datasets;
}

int main()
{
    // Configurações Iniciais
    long long NUMBER_INTERATIONS = 50; // Número de interações que o código vai executar
    std::string output_path = "/home/leo/AnaliseAlgoritmosEstruturaDadosOrdenacaoC-/output" // Caminho de saída do resultado

    // Configuração dos métodos que será utilizado em cada interação
    std::map<std::string, std::pair<std::vector<long long>, std::pair<long long, long long>>(*)(std::vector<long long>)> methods;
    methods["selectionSort"] = selectionSort;
    methods["quickSort"] = quickSort;
    methods["mergeSort"] = mergeSort;
    methods["insertionSort"] = insertionSort;
    methods["heapSort"] = heapSort;
    methods["bubbleSort"] = bubbleSort;
    methods["splaySort"] = splaySort;

    // Nome dos conjuntos de dados, informação que será utilizada posteriormente como identificador no arquivo CSV de resultado
    std::vector<std::string> datasets_name = {
        "Ordered",
        "OrderedInverse",
        "AlmostOrdered",
        "Random"
    };

    // Executando as interações
    for (long long i = 0; i < NUMBER_INTERATIONS; ++i) {

        // Gerando um dataset para está interação, executando primeiro para vetores do tamanho, 10, 100, 1.000 e 10.000
        std::vector<long long> lengthLists1 = {10, 100, 1000, 10000};
        std::vector<std::vector<std::vector<long long>>> datasets1 = generateDataset(lengthLists1);

        // Executando a primeira parte da interação
        runInteration(methods, i, datasets1, datasets_name, output_path, "1");

        // Gerando um dataset para está interação, executando primeiro para vetores do tamanho de 100.000
        std::vector<long long> lengthLists2 = {100000};
        std::vector<std::vector<std::vector<long long>>> datasets2 = generateDataset(lengthLists2);

        // Executando a segunda parte da interação
        runInteration(methods, i, datasets2, datasets_name, output_path, "2");
    }

    return 0;
}