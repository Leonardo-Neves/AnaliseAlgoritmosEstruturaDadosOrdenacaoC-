#include <iostream>
#include <vector>
#include <memory>

using namespace std;

struct Node {
    long long key;
    unique_ptr<Node> left;
    unique_ptr<Node> right;

    Node(long long chave) : key(chave), left(nullptr), right(nullptr) {}
};

unique_ptr<Node> criar_no(long long chave, long long* counter_comparisons, long long* counter_movements) {
    (*counter_movements) += 3;
    return make_unique<Node>(chave);
}

unique_ptr<Node> rotacionar_dir(unique_ptr<Node> x, long long* counter_comparisons, long long* counter_movements) {
    (*counter_movements) += 3;
    unique_ptr<Node> y = move(x->left);
    x->left = move(y->right);
    y->right = move(x);
    return y;
}

unique_ptr<Node> rotacionar_esq(unique_ptr<Node> x, long long* counter_comparisons, long long* counter_movements) {
    (*counter_movements) += 3;
    unique_ptr<Node> y = move(x->right);
    x->right = move(y->left);
    y->left = move(x);
    return y;
}

Node* splay(unique_ptr<Node>& raiz, long long chave, long long* counter_comparisons, long long* counter_movements) {
    (*counter_comparisons)++;
    if (raiz == nullptr || raiz->key == chave) {
        return raiz.get();
    }

    (*counter_comparisons)++;
    if (raiz->key > chave) {
        if (raiz->left == nullptr) {
            (*counter_comparisons)++;
            return raiz.get();
        }
        
        if (raiz->left->key > chave) {
            (*counter_comparisons)++;
            if (raiz->left->left != nullptr) {
                raiz->left.reset(splay(raiz->left, chave, counter_comparisons, counter_movements));
                (*counter_movements)++;
            }
            raiz = rotacionar_dir(move(raiz), counter_comparisons, counter_movements);
        } else if (raiz->left->key < chave) {
            (*counter_comparisons)++;
            if (raiz->left->right != nullptr) {
                raiz->left->right.reset(splay(raiz->left->right, chave, counter_comparisons, counter_movements));
                (*counter_movements)++;
            }
            if (raiz->left->right != nullptr) {
                raiz->left = rotacionar_esq(move(raiz->left), counter_comparisons, counter_movements);
                (*counter_movements)++;
            }
        }

        if (raiz->left != nullptr) {
            raiz = rotacionar_dir(move(raiz), counter_comparisons, counter_movements);
            (*counter_movements)++;
        }

        return raiz.get();
    } else {
        if (raiz->right == nullptr) {
            return raiz.get();
        }

        if (raiz->right->key > chave) {
            (*counter_comparisons)++;
            if (raiz->right->left != nullptr) {
                raiz->right->left.reset(splay(raiz->right->left, chave, counter_comparisons, counter_movements));
                (*counter_movements)++;
            }
            if (raiz->right->left != nullptr) {
                raiz->right = rotacionar_dir(move(raiz->right), counter_comparisons, counter_movements);
                (*counter_movements)++;
            }
        } else if (raiz->right->key < chave) {
            (*counter_comparisons)++;
            if (raiz->right->right != nullptr) {
                raiz->right->right.reset(splay(raiz->right->right, chave, counter_comparisons, counter_movements));
                (*counter_movements)++;
            }
            raiz = rotacionar_esq(move(raiz), counter_comparisons, counter_movements);
            (*counter_movements)++;
        }

        if (raiz->right != nullptr) {
            raiz = rotacionar_esq(move(raiz), counter_comparisons, counter_movements);
            (*counter_movements)++;
        }

        return raiz.get();
    }
}

unique_ptr<Node> insere(unique_ptr<Node> raiz, long long chave, long long* counter_comparisons, long long* counter_movements) {
    (*counter_comparisons)++;
    if (raiz == nullptr) {
        (*counter_movements)++;
        return criar_no(chave, counter_comparisons, counter_movements);
    }

    raiz.reset(splay(raiz, chave, counter_comparisons, counter_movements));
    (*counter_movements)++;

    (*counter_comparisons)++;
    if (raiz->key == chave) {
        return move(raiz);
    }

    unique_ptr<Node> novo_no = criar_no(chave, counter_comparisons, counter_movements);
    (*counter_movements)++;

    if (raiz->key > chave) {
        (*counter_movements) += 3;
        novo_no->right = move(raiz);
        novo_no->left = move(novo_no->right->left);
        novo_no->right->left = nullptr;
    } else {
        (*counter_movements) += 3;
        novo_no->left = move(raiz);
        novo_no->right = move(novo_no->left->right);
        novo_no->left->right = nullptr;
    }

    return novo_no;
}

unique_ptr<Node> deleta(unique_ptr<Node> raiz, long long chave, long long* counter_comparisons, long long* counter_movements) {
    (*counter_comparisons)++;
    if (raiz == nullptr) {
        return nullptr;
    }

    raiz.reset(splay(raiz, chave, counter_comparisons, counter_movements));
    (*counter_movements)++;

    (*counter_comparisons)++;
    if (raiz == nullptr || chave != raiz->key) {
        return move(raiz);
    }

    if (raiz->left == nullptr) {
        (*counter_movements)++;
        return move(raiz->right);
    } else {
        (*counter_movements)++;
        unique_ptr<Node> temp = move(raiz);
        raiz.reset(splay(temp->left, chave, counter_comparisons, counter_movements));
        (*counter_movements)++;
        raiz->right = move(temp->right);
    }

    return move(raiz);
}

long long minimo(Node* raiz, long long* counter_comparisons, long long* counter_movements) {
    (*counter_comparisons)++;
    if (raiz == nullptr) {
        return -1;
    }

    (*counter_movements)++;
    Node* no = raiz;

    while (no->left != nullptr) {
        (*counter_comparisons)++;
        (*counter_movements)++;
        no = no->left.get();
    }

    return no->key;
}

pair<long long, unique_ptr<Node>> extrai_minimo(unique_ptr<Node> raiz, long long* counter_comparisons, long long* counter_movements) {
    (*counter_comparisons)++;
    if (raiz == nullptr) {
        return { -1, nullptr };
    }

    long long min_chave = minimo(raiz.get(), counter_comparisons, counter_movements);
    (*counter_movements)++;

    raiz = deleta(move(raiz), min_chave, counter_comparisons, counter_movements);
    (*counter_movements)++;

    return { min_chave, move(raiz) };
}

vector<long long> splaySort(vector<long long>* A, long long* counter_comparisons, long long* counter_movements) {
    unique_ptr<Node> raiz = nullptr;

    for (long long x : *A) {
        raiz = insere(move(raiz), x, counter_comparisons, counter_movements);
        (*counter_movements)++;
    }

    vector<long long> B;

    while (raiz != nullptr) {
        (*counter_comparisons)++;
        pair<long long, unique_ptr<Node>> min_node = extrai_minimo(move(raiz), counter_comparisons, counter_movements);
        (*counter_movements)++;
        B.push_back(min_node.first);
        (*counter_movements)++;
        raiz = move(min_node.second);
    }

    return B;
}

// int main() {
//     vector<long long> A;
//     for (long long i = 1; i <= 1000000; ++i) {
//         A.push_back(rand() % 1000000); // Inserindo 1 milhão de elementos aleatórios
//     }
//     long long counter_comparisons = 0;
//     long long counter_movements = 0;

//     vector<long long> sorted = splaySort(&A, &counter_comparisons, &counter_movements);

//     for (long long x : sorted) {
//         cout << x << " ";
//     }
//     cout << endl;

//     cout << "Comparisons: " << counter_comparisons << endl;
//     cout << "Movements: " << counter_movements << endl;

//     return 0;
// }
