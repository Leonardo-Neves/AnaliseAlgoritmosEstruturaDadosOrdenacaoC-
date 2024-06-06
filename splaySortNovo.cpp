#include <iostream>
#include <vector>

using namespace std;

struct Node {
    long long key;
    Node* left;
    Node* right;
};

pair<Node*, pair<long long, long long>> criar_no(long long chave, long long counter_comparisons, long long counter_movements) {
    Node* no = new Node();
    counter_movements += 3;
    no->key = chave;
    no->left = nullptr;
    no->right = nullptr;
    return make_pair(no, make_pair(counter_comparisons, counter_movements));
}

pair<Node*, pair<long long, long long>> rotacionar_dir(Node* x, long long counter_comparisons, long long counter_movements) {
    counter_movements += 3;
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return make_pair(y, make_pair(counter_comparisons, counter_movements));
}

pair<Node*, pair<long long, long long>> rotacionar_esq(Node* x, long long counter_comparisons, long long counter_movements) {
    counter_movements += 3;
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return make_pair(y, make_pair(counter_comparisons, counter_movements));
}

pair<Node*, pair<long long, long long>> splay(Node* raiz, long long chave, long long counter_comparisons, long long counter_movements) {
    counter_comparisons++;
    if (raiz == nullptr || raiz->key == chave) {
        return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
    }

    counter_comparisons++;
    if (raiz->key > chave) {

        counter_comparisons++;
        if (raiz->left == nullptr) {
            return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
        }

        counter_comparisons++;
        if (raiz->left->key > chave) {

            counter_movements++;
            auto result = splay(raiz->left->left, chave, counter_comparisons, counter_movements);
            raiz->left->left = result.first;

            counter_comparisons = result.second.first;
            counter_movements = result.second.second;

            counter_movements++;
            auto result2 = rotacionar_dir(raiz, counter_comparisons, counter_movements);
            raiz = result2.first;
            counter_comparisons = result2.second.first;
            counter_movements = result2.second.second;

        } else if (raiz->left->key < chave) {
            counter_comparisons++;

            counter_movements++;
            auto result = splay(raiz->left->right, chave, counter_comparisons, counter_movements);
            raiz->left->right = result.first;
            counter_comparisons = result.second.first;
            counter_movements = result.second.second;

            counter_comparisons++;
            if (raiz->left->right != nullptr) {

                counter_movements++;
                auto result2 = rotacionar_esq(raiz->left, counter_comparisons, counter_movements);
                raiz->left = result2.first;
                counter_comparisons = result2.second.first;
                counter_movements = result2.second.second;
            }
            
        }

        counter_comparisons++;
        if (raiz->left != nullptr) {
            counter_movements++;
            auto result = rotacionar_dir(raiz, counter_comparisons, counter_movements);
            raiz = result.first;
            counter_comparisons = result.second.first;
            counter_movements = result.second.second;
        }
        return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
    } else {
        counter_comparisons++;
        if (raiz->right == nullptr) {
            return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
        }

        counter_comparisons++;
        if (raiz->right->key > chave) {

            counter_movements++;
            auto result = splay(raiz->right->left, chave, counter_comparisons, counter_movements);
            raiz->right->left = result.first;
            counter_comparisons = result.second.first;
            counter_movements = result.second.second;

            counter_comparisons++;
            if (raiz->right->left != nullptr) {
                counter_movements++;
                auto result2 = rotacionar_dir(raiz->right, counter_comparisons, counter_movements);
                raiz->right = result2.first;
                counter_comparisons = result2.second.first;
                counter_movements = result2.second.second;
            }
        } else if (raiz->right->key < chave) {
            counter_comparisons++;

            counter_movements++;
            auto result = splay(raiz->right->right, chave, counter_comparisons, counter_movements);
            raiz->right->right = result.first;
            counter_comparisons = result.second.first;
            counter_movements = result.second.second;

            counter_movements++;
            auto result2 = rotacionar_esq(raiz, counter_comparisons, counter_movements);
            raiz = result2.first;
            counter_comparisons = result2.second.first;
            counter_movements = result2.second.second;
        }

        counter_comparisons++;
        if (raiz->right != nullptr) {
            counter_movements++;
            auto result = rotacionar_esq(raiz, counter_comparisons, counter_movements);
            raiz = result.first;
            counter_comparisons = result.second.first;
            counter_movements = result.second.second;
        }
        return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
    }
}

pair<Node*, pair<long long, long long>> insere(Node* raiz, long long chave, long long counter_comparisons, long long counter_movements) {
    counter_comparisons++;
    if (raiz == nullptr) {
        auto result = criar_no(chave, counter_comparisons, counter_movements);
        return make_pair(result.first, make_pair(result.second.first, result.second.second));
    }

    counter_movements++;
    auto result = splay(raiz, chave, counter_comparisons, counter_movements);
    raiz = result.first;
    counter_comparisons = result.second.first;
    counter_movements = result.second.second;

    counter_comparisons++;
    if (raiz->key == chave) {
        return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
    }

    auto novo_no = criar_no(chave, counter_comparisons, counter_movements);
    Node* novo = novo_no.first;
    counter_comparisons = novo_no.second.first;
    counter_movements = novo_no.second.second;

    counter_comparisons++;
    if (raiz->key > chave) {
        counter_movements += 3;
        novo->right = raiz;
        novo->left = raiz->left;
        raiz->left = nullptr;
    } else {
        counter_movements += 3;
        novo->left = raiz;
        novo->right = raiz->right;
        raiz->right = nullptr;
    }

    return make_pair(novo, make_pair(counter_comparisons, counter_movements));
}

pair<Node*, pair<long long, long long>> percorre_in_order(Node* raiz, vector<long long>& resultado, long long counter_comparisons, long long counter_movements) {
    counter_comparisons++;
    if (raiz == nullptr) {
        return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
    }

    counter_movements++;
    auto left_result = percorre_in_order(raiz->left, resultado, counter_comparisons, counter_movements);
    counter_comparisons = left_result.second.first;
    counter_movements = left_result.second.second;

    counter_movements++;
    resultado.push_back(raiz->key);

    counter_movements++;
    auto right_result = percorre_in_order(raiz->right, resultado, counter_comparisons, counter_movements);
    counter_comparisons = right_result.second.first;
    counter_movements = right_result.second.second;

    return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
}

pair<vector<long long>, pair<long long, long long>> splaySort(vector<long long> A) {
    long long counter_comparisons = 0;
    long long counter_movements = 0;

    Node* raiz = nullptr;
    for (long long x : A) {
        counter_comparisons++;

        counter_movements++;
        auto result = insere(raiz, x, counter_comparisons, counter_movements);
        raiz = result.first;
        counter_comparisons = result.second.first;
        counter_movements = result.second.second;
    }

    vector<long long> resultado;
    counter_movements++;
    auto traversal_result = percorre_in_order(raiz, resultado, counter_comparisons, counter_movements);
    counter_comparisons = traversal_result.second.first;
    counter_movements = traversal_result.second.second;

    return make_pair(resultado, make_pair(counter_comparisons, counter_movements));
}

// int main() {
//     vector<long long> A = { 10, 3, 8, 15, 6, 1 };
//     auto result = splaySort(A);

//     vector<long long> sorted_A = result.first;

//     for (long long x : sorted_A) {
//         cout << x << " ";
//     }
//     cout << endl;

//     cout << "Comparações: " << result.second.first << "\n";
//     cout << "Movimentações: " << result.second.second << "\n";

//     return 0;
// }
