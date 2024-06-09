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

        if (raiz->left == nullptr) {
            counter_comparisons++;
            return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
        }
        
        if (raiz->left->key > chave) {
            counter_comparisons++;

            counter_comparisons++;
            if (raiz->left->left != nullptr) {
                auto result1 = splay(raiz->left->left, chave, counter_comparisons, counter_movements);

                raiz->left->left = result1.first;
                counter_comparisons = result1.second.first;
                counter_movements = result1.second.second;
                counter_movements++;
            }
                
            auto result2 = rotacionar_dir(raiz, counter_comparisons, counter_movements);
            raiz = result2.first;
            counter_comparisons = result2.second.first;
            counter_movements = result2.second.second;
            counter_movements++;

        } else if (raiz->left->key <= chave) {
            counter_comparisons++;

            counter_comparisons++;
            if (raiz->left->right != nullptr) {
                auto result3 = splay(raiz->left->right, chave, counter_comparisons, counter_movements);

                raiz->left->right = result3.first;
                counter_comparisons = result3.second.first;
                counter_movements = result3.second.second;
                counter_movements++;
            }
            
            counter_comparisons++;
            if (raiz->left->right != nullptr) {
                auto result4 = rotacionar_esq(raiz->left, counter_comparisons, counter_movements);

                raiz->left = result4.first;
                counter_comparisons = result4.second.first;
                counter_movements = result4.second.second;
                counter_movements++;
            }
        }

        counter_comparisons++;
        if (raiz->left != nullptr) {
            auto result5 = rotacionar_dir(raiz, counter_comparisons, counter_movements);
            raiz = result5.first;
            counter_comparisons = result5.second.first;
            counter_movements = result5.second.second;
            counter_movements++;
        }

        return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
    } else {

        counter_comparisons++;
        if (raiz->right == nullptr) {
            return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
        }

        if (raiz->right->key > chave) {
            counter_comparisons++;

            counter_comparisons++;
            if (raiz->right->left != nullptr) {
                auto result6 = splay(raiz->right->left, chave, counter_comparisons, counter_movements);

                raiz->right->left = result6.first;
                counter_comparisons = result6.second.first;
                counter_movements = result6.second.second;
                counter_movements++;
            }
            
            counter_comparisons++;
            if (raiz->right->left != nullptr) {
                auto result7 = rotacionar_dir(raiz->right, counter_comparisons, counter_movements);

                raiz->right = result7.first;
                counter_comparisons = result7.second.first;
                counter_movements = result7.second.second;
                counter_movements++;
            }
                
        } else if (raiz->right->key <= chave) {
            counter_comparisons++;

            counter_comparisons++;
            if (raiz->right->right != nullptr) {
                auto result8 = splay(raiz->right->right, chave, counter_comparisons, counter_movements);

                raiz->right->right = result8.first;
                counter_comparisons = result8.second.first;
                counter_movements = result8.second.second;
                counter_movements++;
            }
                
            auto result9 = rotacionar_esq(raiz, counter_comparisons, counter_movements);

            raiz = result9.first;
            counter_comparisons = result9.second.first;
            counter_movements = result9.second.second;
            counter_movements++;
        }

        counter_comparisons++;
        if (raiz->right != nullptr) {
            auto result10 = rotacionar_esq(raiz, counter_comparisons, counter_movements);

            raiz = result10.first;
            counter_comparisons = result10.second.first;
            counter_movements = result10.second.second;
            counter_movements++;
        }
            
        return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
    }
}

pair<Node*, pair<long long, long long>> insere(Node* raiz, long long chave, long long counter_comparisons, long long counter_movements) {

    counter_comparisons++;
    if (raiz == nullptr) {
        auto result1 = criar_no(chave, counter_comparisons, counter_movements);
        counter_comparisons = result1.second.first;
        counter_movements = result1.second.second;
        counter_movements++;

        return make_pair(result1.first, make_pair(counter_comparisons, counter_movements));
    }
        
    auto result2 = splay(raiz, chave, counter_comparisons, counter_movements);
    raiz = result2.first;
    counter_comparisons = result2.second.first;
    counter_movements = result2.second.second;
    counter_movements++;

    counter_comparisons++;
    if (raiz->key == chave) {
        return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
    }
    
    auto result3 = criar_no(chave, counter_comparisons, counter_movements);
    Node* novo_no = result3.first;
    counter_comparisons = result3.second.first;
    counter_movements = result3.second.second;
    counter_movements++;

    counter_comparisons++;
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

    return make_pair(novo_no, make_pair(counter_comparisons, counter_movements));
}

pair<Node*, pair<long long, long long>> deleta(Node* raiz, long long chave, long long counter_comparisons, long long counter_movements) {

    counter_comparisons++;
    if (raiz == nullptr) {
        return make_pair(nullptr, make_pair(counter_comparisons, counter_movements));
    }

    auto result1 = splay(raiz, chave, counter_comparisons, counter_movements);

    raiz = result1.first;
    counter_comparisons = result1.second.first;
    counter_movements = result1.second.second;
    counter_movements++;

    counter_comparisons++;
    if (raiz == nullptr || chave != raiz->key) {
        return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
    }

    counter_comparisons++;
    if (raiz->left == nullptr) {

        counter_movements++;
        Node* temp = raiz->right;
        delete raiz;
        return make_pair(temp, make_pair(counter_comparisons, counter_movements));
    } else {

        counter_movements++;
        Node* temp = raiz;

        auto result2 = splay(raiz->left, chave, counter_comparisons, counter_movements);

        raiz = result2.first;
        counter_comparisons = result2.second.first;
        counter_movements = result2.second.second;
        counter_movements++;

        counter_movements++;
        raiz->right = temp->right;
        delete temp;
    }

    return make_pair(raiz, make_pair(counter_comparisons, counter_movements));
}

pair<long long, pair<long long, long long>> minimo(Node* raiz, long long counter_comparisons, long long counter_movements) {

    counter_comparisons++;
    if (raiz == nullptr) {
        return make_pair(-1, make_pair(counter_comparisons, counter_movements));
    }
        
    counter_movements++;
    Node* no = raiz;

    while (no->left != nullptr) {
        counter_comparisons++;

        counter_movements++;
        no = no->left;
    }
        
    return make_pair(no->key, make_pair(counter_comparisons, counter_movements));
}

pair<pair<long long, Node*>, pair<long long, long long>> extrai_minimo(Node* raiz, long long counter_comparisons, long long counter_movements) {

    counter_comparisons++;
    if (raiz == nullptr) {
        return {{ -1, nullptr }, {counter_comparisons, counter_movements}};
    }

    auto result1 = minimo(raiz, counter_comparisons, counter_movements);

    long long min_chave = result1.first;
    counter_comparisons = result1.second.first;
    counter_movements = result1.second.second;
    counter_movements++;

    auto result2 = deleta(raiz, min_chave, counter_comparisons, counter_movements);

    raiz = result2.first;
    counter_comparisons = result2.second.first;
    counter_movements = result2.second.second;
    counter_movements++;

    return {{ min_chave, raiz }, {counter_comparisons, counter_movements}};
}



pair<long long, long long> inorderTraversal(Node* raiz, vector<long long>& result, long long counter_comparisons, long long counter_movements) {
    counter_comparisons++;
    if (raiz == nullptr) {
        return {counter_comparisons, counter_movements};
    }
    inorderTraversal(raiz->left, result, counter_comparisons, counter_movements);
    result.push_back(raiz->key);
    counter_movements++;
    inorderTraversal(raiz->right, result, counter_comparisons, counter_movements);
    return make_pair(counter_comparisons, counter_movements);
}

pair<vector<long long>, pair<long long, long long>> splaySort(vector<long long> A) {
    long long counter_comparisons = 0;
    long long counter_movements = 0;

    Node* raiz = nullptr;
    for (long long x : A) {
        auto result = insere(raiz, x, counter_comparisons, counter_movements);
        raiz = result.first;
        counter_comparisons = result.second.first;
        counter_movements = result.second.second;
        counter_movements++;
    }

    vector<long long> sorted_A;
    auto result2 = inorderTraversal(raiz, sorted_A, counter_comparisons, counter_movements);
    counter_comparisons = result2.first;
    counter_movements = result2.second;
    counter_movements++;

    return make_pair(sorted_A, make_pair(counter_comparisons, counter_movements));
}

// pair<vector<long long>, pair<long long, long long>> splaySort(vector<long long> A) {

//     long long counter_comparisons = 0;
//     long long counter_movements = 0;

//     Node* raiz = nullptr;
//     for (long long x : A) {
//         auto result1 = insere(raiz, x, counter_comparisons, counter_movements);

//         raiz = result1.first;
//         counter_comparisons = result1.second.first;
//         counter_movements = result1.second.second;
//         counter_movements++;
//     }
        

//     vector<long long> B;
//     while (raiz != nullptr) {
//         counter_comparisons++;

//         auto result2 = extrai_minimo(raiz, counter_comparisons, counter_movements);

//         pair<long long, Node*> min_node = result2.first;
//         counter_comparisons = result2.second.first;
//         counter_movements = result2.second.second;
//         counter_movements++;

//         B.push_back(min_node.first);

//         counter_movements++;
//         raiz = min_node.second;
//     }
    
//     return make_pair(B, make_pair(counter_comparisons, counter_movements));
// }

// long long main() {
//     vector<long long> A = { 10, 3, 8, 15, 6, 1 };
//     auto result = splaySort(A);

//     vector<long long> sorted_A = result.first;

//     for (long long x : sorted_A) {
//         cout << x << "\n";
//     }
//     cout << endl;

//     cout << result.second.first << "\n";
//     cout << result.second.second << "\n";

//     return 0;
// }
