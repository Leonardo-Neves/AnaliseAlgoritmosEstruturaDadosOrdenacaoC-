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
    (*counter_comparisons)++;
    if (raiz == nullptr || raiz->key == chave) {
        return raiz;
    }

    (*counter_comparisons)++;
    if (raiz->key > chave) {

        if (raiz->left == nullptr) {
            (*counter_comparisons)++;
            return raiz;
        }
        
        if (raiz->left->key > chave) {
            (*counter_comparisons)++;

            (*counter_comparisons)++;
            if (raiz->left->left != nullptr) {
                raiz->left->left = splay(raiz->left->left, chave, counter_comparisons, counter_movements);
                (*counter_movements)++;
            }
                
            raiz = rotacionar_dir(raiz, counter_comparisons, counter_movements);
            (*counter_movements)++;

        } else if (raiz->left->key <= chave) {
            (*counter_comparisons)++;

            (*counter_comparisons)++;
            if (raiz->left->right != nullptr) {
                raiz->left->right = splay(raiz->left->right, chave, counter_comparisons, counter_movements);
                (*counter_movements)++;
            }
            
            (*counter_comparisons)++;
            if (raiz->left->right != nullptr) {
                raiz->left = rotacionar_esq(raiz->left, counter_comparisons, counter_movements);
                (*counter_movements)++;
            }
        }

        (*counter_comparisons)++;
        if (raiz->left != nullptr) {
            raiz = rotacionar_dir(raiz, counter_comparisons, counter_movements);
            (*counter_movements)++;
        }

        return raiz;
    } else {

        (*counter_comparisons)++;
        if (raiz->right == nullptr) {
            return raiz;
        }

        if (raiz->right->key > chave) {
            (*counter_comparisons)++;

            (*counter_comparisons)++;
            if (raiz->right->left != nullptr) {
                raiz->right->left = splay(raiz->right->left, chave, counter_comparisons, counter_movements);
                (*counter_movements)++;
            }
            
            (*counter_comparisons)++;
            if (raiz->right->left != nullptr) {
                raiz->right = rotacionar_dir(raiz->right, counter_comparisons, counter_movements);
                (*counter_movements)++;
            }
                
        } else if (raiz->right->key <= chave) {
            (*counter_comparisons)++;

            (*counter_comparisons)++;
            if (raiz->right->right != nullptr) {
                raiz->right->right = splay(raiz->right->right, chave, counter_comparisons, counter_movements);
                (*counter_movements)++;
            }
                
            raiz = rotacionar_esq(raiz, counter_comparisons, counter_movements);
            (*counter_movements)++;
        }

        (*counter_comparisons)++;
        if (raiz->right != nullptr) {
            raiz = rotacionar_esq(raiz, counter_comparisons, counter_movements);
            (*counter_movements)++;
        }
            
        return raiz;
    }
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
