#include <iostream>
#include <vector>
#include <ctime>
#include <utility>

class SplayTree {
public:
    struct No {
        int chave;
        No* esquerda;
        No* direita;

        No(int c) : chave(c), esquerda(nullptr), direita(nullptr) {}
    };

    SplayTree() : contador_comparacoes(0), contador_movimentacoes(0), raiz(nullptr) {}

    void inserir(int chave) {
        raiz = inserir(raiz, chave);
    }

    void deletar(int chave) {
        raiz = deletar(raiz, chave);
    }

    std::vector<int> ordenar(std::vector<int>& A) {
        contador_comparacoes = 0;
        contador_movimentacoes = 0;

        for (int x : A) {
            inserir(x);
        }

        std::vector<int> B;
        while (raiz != nullptr) {
            auto min_result = extrairMinimo(raiz);
            int min_valor = min_result.first;
            raiz = min_result.second;
            B.push_back(min_valor);
        }

        return B;
    }

    int getContadorComparacoes() const {
        return contador_comparacoes;
    }

    int getContadorMovimentacoes() const {
        return contador_movimentacoes;
    }

private:
    int contador_comparacoes;
    int contador_movimentacoes;
    No* raiz;

    No* criarNo(int chave) {
        contador_movimentacoes++;
        return new No(chave);
    }

    No* rotacionarDireita(No* x) {
        contador_movimentacoes += 3;
        No* y = x->esquerda;
        x->esquerda = y->direita;
        y->direita = x;
        return y;
    }

    No* rotacionarEsquerda(No* x) {
        contador_movimentacoes += 3;
        No* y = x->direita;
        x->direita = y->esquerda;
        y->esquerda = x;
        return y;
    }

    No* splay(No* raiz, int chave) {
        contador_comparacoes++;
        if (raiz == nullptr || raiz->chave == chave) {
            return raiz;
        }

        contador_comparacoes++;
        if (raiz->chave > chave) {
            contador_comparacoes++;
            if (raiz->esquerda == nullptr) {
                return raiz;
            }

            contador_comparacoes++;
            if (raiz->esquerda->chave > chave) {
                contador_movimentacoes += 2;
                raiz->esquerda->esquerda = splay(raiz->esquerda->esquerda, chave);
                raiz = rotacionarDireita(raiz);
            } else if (raiz->esquerda->chave < chave) {
                
                contador_movimentacoes++;
                raiz->esquerda->direita = splay(raiz->esquerda->direita, chave);
                contador_comparacoes++;
                if (raiz->esquerda->direita != nullptr) {
                    raiz->esquerda = rotacionarEsquerda(raiz->esquerda);
                }
            }
            contador_comparacoes++;
            if (raiz->esquerda == nullptr) {
                return raiz;
            } else {
                return rotacionarDireita(raiz);
            }
        } else {
            contador_comparacoes++;
            if (raiz->direita == nullptr) {
                return raiz;
            }

            contador_comparacoes++;
            if (raiz->direita->chave > chave) {
                contador_movimentacoes++;
                raiz->direita->esquerda = splay(raiz->direita->esquerda, chave);
                contador_comparacoes++;
                if (raiz->direita->esquerda != nullptr) {
                    contador_movimentacoes++;
                    raiz->direita = rotacionarDireita(raiz->direita);
                }
            } else if (raiz->direita->chave < chave) {
                contador_movimentacoes += 2;
                raiz->direita->direita = splay(raiz->direita->direita, chave);
                raiz = rotacionarEsquerda(raiz);
            }

            contador_comparacoes++;
            if (raiz->direita == nullptr) {
                return raiz;
            } else {
                return rotacionarEsquerda(raiz);
            }
        }
    }

    No* inserir(No* raiz, int chave) {
        contador_comparacoes++;
        if (raiz == nullptr) {
            return criarNo(chave);
        }

        contador_movimentacoes++;
        raiz = splay(raiz, chave);

        contador_movimentacoes++;
        No* novoNo = criarNo(chave);

        contador_comparacoes++;
        if (raiz->chave > chave) {
            contador_movimentacoes += 3;
            novoNo->direita = raiz;
            novoNo->esquerda = raiz->esquerda;
            raiz->esquerda = nullptr;
        } else {
            contador_movimentacoes += 3;
            novoNo->esquerda = raiz;
            novoNo->direita = raiz->direita;
            raiz->direita = nullptr;
        }

        return novoNo;
    }

    No* deletar(No* raiz, int chave) {
        contador_comparacoes++;
        if (raiz == nullptr) {
            return nullptr;
        }

        contador_movimentacoes++;
        raiz = splay(raiz, chave);

        contador_comparacoes++;
        if (raiz == nullptr || chave != raiz->chave) {
            return raiz;
        }

        contador_comparacoes++;
        if (raiz->esquerda == nullptr) {
            return raiz->direita;
        } else {
            contador_movimentacoes += 3;
            No* temp = raiz;
            raiz = splay(raiz->esquerda, chave);
            raiz->direita = temp->direita;
        }

        return raiz;
    }

    No* encontrarMinimo(No* raiz) {
        contador_comparacoes++;
        if (raiz == nullptr) {
            return nullptr;
        }

        contador_movimentacoes++;
        No* no = raiz;
        

        while (no->esquerda != nullptr) {
            contador_comparacoes++;
            contador_movimentacoes++;
            no = no->esquerda;
        }

        return no;
    }

    std::pair<int, No*> extrairMinimo(No* raiz) {
        contador_comparacoes++;
        if (raiz == nullptr) {
            return {0, nullptr};
        }

        contador_movimentacoes += 2;
        No* noMinimo = encontrarMinimo(raiz);
        raiz = deletar(raiz, noMinimo->chave);
        return {noMinimo->chave, raiz};
    }
};

int main() {
    std::vector<int> A = {5, 2, 8, 1, 3, 2, 10, 4};
    SplayTree splayTree;

    // Medir tempo de execução
    std::clock_t c_start = std::clock();
    std::vector<int> B = splayTree.ordenar(A);
    std::clock_t c_end = std::clock();

    long double tempo_decorrido_ms = 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC;

    std::cout << "Array ordenado: ";
    for (int val : B) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "Tempo de CPU usado: " << tempo_decorrido_ms << " ms\n";
    std::cout << "Comparações: " << splayTree.getContadorComparacoes() << std::endl;
    std::cout << "Movimentações: " << splayTree.getContadorMovimentacoes() << std::endl;

    return 0;
}
