#include <iostream>
#include <vector>

using namespace std;

struct Node {
    long long key;
    Node* left;
    Node* right;
};

Node* criar_no(long long chave, long long* counter_comparisons, long long* counter_movements) {
    Node* no = new Node();

    (*counter_movements) += 3;
    no->key = chave;
    no->left = nullptr;
    no->right = nullptr;
    return no;
}

Node* rotacionar_dir(Node* x, long long* counter_comparisons, long long* counter_movements) {
    (*counter_movements) += 3;
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

Node* rotacionar_esq(Node* x, long long* counter_comparisons, long long* counter_movements) {
    (*counter_movements) += 3;
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

Node* splay(Node* raiz, long long chave, long long* counter_comparisons, long long* counter_movements) {
    if (raiz == nullptr || raiz->key == chave) {
        (*counter_comparisons)++;
        return raiz;
    }

    Node header;
    header.left = header.right = nullptr;
    Node* LeftTreeMax = &header;
    Node* RightTreeMin = &header;

    while (true) {
        (*counter_comparisons)++;
        if (chave < raiz->key) {
            if (raiz->left == nullptr) {
                break;
            }

            (*counter_comparisons)++;
            if (chave < raiz->left->key) {
                raiz = rotacionar_dir(raiz, counter_comparisons, counter_movements);
                (*counter_movements)++;

                if (raiz->left == nullptr) {
                    break;
                }
            }

            (*counter_movements)++;
            RightTreeMin->left = raiz;
            RightTreeMin = raiz;
            raiz = raiz->left;
            RightTreeMin->left = nullptr;
        } else if (chave > raiz->key) {
            if (raiz->right == nullptr) {
                break;
            }

            (*counter_comparisons)++;
            if (chave > raiz->right->key) {
                raiz = rotacionar_esq(raiz, counter_comparisons, counter_movements);
                (*counter_movements)++;

                if (raiz->right == nullptr) {
                    break;
                }
            }

            (*counter_movements)++;
            LeftTreeMax->right = raiz;
            LeftTreeMax = raiz;
            raiz = raiz->right;
            LeftTreeMax->right = nullptr;
        } else {
            break;
        }
    }

    (*counter_movements)++;
    LeftTreeMax->right = raiz->left;
    (*counter_movements)++;
    RightTreeMin->left = raiz->right;
    (*counter_movements)++;
    raiz->left = header.right;
    (*counter_movements)++;
    raiz->right = header.left;

    return raiz;
}

Node* insere(Node* raiz, long long chave, long long* counter_comparisons, long long* counter_movements) {

    (*counter_comparisons)++;
    if (raiz == nullptr) {
        return criar_no(chave, counter_comparisons, counter_movements);;
    }
        
    raiz = splay(raiz, chave, counter_comparisons, counter_movements);
    (*counter_movements)++;

    (*counter_comparisons)++;
    if (raiz->key == chave) {
        return raiz;
    }
    
    Node* novo_no = criar_no(chave, counter_comparisons, counter_movements);
    (*counter_movements)++;

    (*counter_comparisons)++;
    if (raiz->key > chave) {
        counter_movements += 3;
        novo_no->right = raiz;
        novo_no->left = raiz->left;
        raiz->left = nullptr;
    } else {
        counter_movements += 3;
        novo_no->left = raiz;
        novo_no->right = raiz->right;
        raiz->right = nullptr;
    }

    return novo_no;
}

Node* deleta(Node* raiz, long long chave, long long* counter_comparisons, long long* counter_movements) {

    (*counter_comparisons)++;
    if (raiz == nullptr) {
        return nullptr;
    }

    raiz = splay(raiz, chave, counter_comparisons, counter_movements);
    (*counter_movements)++;

    (*counter_comparisons)++;
    if (raiz == nullptr || chave != raiz->key) {
        return raiz;
    }

    (*counter_comparisons)++;
    if (raiz->left == nullptr) {

        (*counter_movements)++;
        Node* temp = raiz->right;
        delete raiz;
        return temp;
    } else {

        (*counter_movements)++;
        Node* temp = raiz;

        raiz = splay(raiz->left, chave, counter_comparisons, counter_movements);
        (*counter_movements)++;

        (*counter_movements)++;
        raiz->right = temp->right;
        delete temp;
    }

    return raiz;
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
        no = no->left;
    }
        
    return no->key;
}

pair<long long, Node*> extrai_minimo(Node* raiz, long long* counter_comparisons, long long* counter_movements) {

    (*counter_comparisons)++;
    if (raiz == nullptr) {
        return { -1, nullptr };
    }

    long long min_chave = minimo(raiz, counter_comparisons, counter_movements);
    (*counter_movements)++;

    raiz = deleta(raiz, min_chave, counter_comparisons, counter_movements);
    (*counter_movements)++;

    return { min_chave, raiz };
}

pair<long long*, long long*> inorderTraversal(Node* raiz, vector<long long>& result, long long* counter_comparisons, long long* counter_movements) {
    (*counter_comparisons)++;
    if (raiz == nullptr) {
        return {counter_comparisons, counter_movements};
    }
    inorderTraversal(raiz->left, result, counter_comparisons, counter_movements);
    result.push_back(raiz->key);
    (*counter_movements)++;
    inorderTraversal(raiz->right, result, counter_comparisons, counter_movements);
    return make_pair(counter_comparisons, counter_movements);
}

vector<long long> splaySort(vector<long long> *A, long long *counter_comparisons, long long *counter_movements) {

    Node* raiz = nullptr;
    for (long long x : *A) {
        raiz = insere(raiz, x, counter_comparisons, counter_movements);
        (*counter_movements)++;
    }

    vector<long long> sorted_A;
    auto result2 = inorderTraversal(raiz, sorted_A, counter_comparisons, counter_movements);
    (*counter_movements)++;

    return sorted_A;
}
