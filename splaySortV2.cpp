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
    //(*counter_movements) += 3;
    return make_unique<Node>(chave);
}

unique_ptr<Node> rotacionar_dir(unique_ptr<Node> x, long long* counter_comparisons, long long* counter_movements) {
    //(*counter_movements) += 3;
    unique_ptr<Node> y = move(x->left);
    x->left = move(y->right);
    y->right = move(x);
    return y;
}

unique_ptr<Node> rotacionar_esq(unique_ptr<Node> x, long long* counter_comparisons, long long* counter_movements) {
    //(*counter_movements) += 3;
    unique_ptr<Node> y = move(x->right);
    x->right = move(y->left);
    y->left = move(x);
    return y;
}

Node* splay(unique_ptr<Node>& raiz, long long chave, long long* counter_comparisons, long long* counter_movements) {
    if (raiz == nullptr || raiz->key == chave) {
        return raiz.get();
    }

    unique_ptr<Node> header = make_unique<Node>(0);
    Node* left_tree = header.get();
    Node* right_tree = header.get();
    unique_ptr<Node> temp = nullptr;

    Node* current = raiz.release();

    while (true) {
        (*counter_comparisons)++;
        if (chave < current->key) {
            if (current->left == nullptr) {
                break;
            }
            (*counter_comparisons)++;
            if (chave < current->left->key) {
                // Zig-Zig
                temp = move(current->left);
                current->left = move(temp->right);
                temp->right.reset(current);
                current = temp.release();
                (*counter_movements)++;
                (*counter_comparisons)++;
                if (current->left == nullptr) {
                    break;
                }
            }
            // Link right
            right_tree->left.reset(current);
            right_tree = current;
            current = current->left.release();
            (*counter_movements)++;
        } else if (chave > current->key) {
            if (current->right == nullptr) {
                break;
            }
            (*counter_comparisons)++;
            if (chave > current->right->key) {
                // Zag-Zag
                temp = move(current->right);
                current->right = move(temp->left);
                temp->left.reset(current);
                current = temp.release();
                (*counter_movements)++;
                (*counter_comparisons)++;
                if (current->right == nullptr) {
                    break;
                }
            }
            // Link left
            left_tree->right.reset(current);
            left_tree = current;
            current = current->right.release();
            (*counter_movements)++;
        } else {
            break;
        }
    }

    // Assemble
    left_tree->right.reset(current->left.release());
    right_tree->left.reset(current->right.release());
    current->left.reset(header->right.release());
    current->right.reset(header->left.release());
    
    raiz.reset(current);
    return raiz.get();
}

unique_ptr<Node> insere(unique_ptr<Node> raiz, long long chave, long long* counter_comparisons, long long* counter_movements) {

    //(*counter_comparisons)++;
    if (raiz == nullptr) {
        //(*counter_movements)++;
        return criar_no(chave, counter_comparisons, counter_movements);;
    }
        
    raiz.reset(splay(raiz, chave, counter_comparisons, counter_movements));
    //(*counter_movements)++;

    //(*counter_comparisons)++;
    if (raiz->key == chave) {
        return move(raiz);
    }
    
    unique_ptr<Node> novo_no = criar_no(chave, counter_comparisons, counter_movements);
    //(*counter_movements)++;

    //(*counter_comparisons)++;
    if (raiz->key > chave) {
        counter_movements += 3;
        novo_no->right = move(raiz);
        novo_no->left = move(raiz->left);
        raiz->left = nullptr;
    } else {
        counter_movements += 3;
        novo_no->left = move(raiz);
        novo_no->right = move(raiz->right);
        raiz->right = nullptr;
    }

    return novo_no;
}

unique_ptr<Node> deleta(unique_ptr<Node> raiz, long long chave, long long* counter_comparisons, long long* counter_movements) {

    //(*counter_comparisons)++;
    if (raiz == nullptr) {
        return nullptr;
    }

    raiz.reset(splay(raiz, chave, counter_comparisons, counter_movements));
    //(*counter_movements)++;

    //(*counter_comparisons)++;
    if (raiz == nullptr || chave != raiz->key) {
        return move(raiz);
    }

    //(*counter_comparisons)++;
    if (raiz->left == nullptr) {

        //(*counter_movements)++;
        return move(raiz->right);
    } else {

        //(*counter_movements)++;
        unique_ptr<Node> temp = move(raiz);

        raiz.reset(splay(temp->left, chave, counter_comparisons, counter_movements));
        //(*counter_movements)++;

        //(*counter_movements)++;
        raiz->right = move(temp->right);
    }

    return move(raiz);
}

long long minimo(Node* raiz, long long* counter_comparisons, long long* counter_movements) {

    //(*counter_comparisons)++;
    if (raiz == nullptr) {
        return -1;
    }
        
    //(*counter_movements)++;
    Node* no = raiz;

    while (no->left != nullptr) {
        //(*counter_comparisons)++;

        //(*counter_movements)++;
        no = no->left.get();
    }
        
    return no->key;
}

pair<long long, unique_ptr<Node>> extrai_minimo(unique_ptr<Node> raiz, long long* counter_comparisons, long long* counter_movements) {

    //(*counter_comparisons)++;
    if (raiz == nullptr) {
        return { -1, nullptr };
    }

    long long min_chave = minimo(raiz.get(), counter_comparisons, counter_movements);
    //(*counter_movements)++;

    raiz = deleta(move(raiz), min_chave, counter_comparisons, counter_movements);;
    //(*counter_movements)++;

    return { min_chave, move(raiz) };
}

vector<long long> splaySort(vector<long long> *A, long long *counter_comparisons, long long *counter_movements) {
    
    unique_ptr<Node> raiz = nullptr;

    for (long long x : *A) {
        raiz = insere(move(raiz), x, counter_comparisons, counter_movements);
        //(*counter_movements)++;
    }

    vector<long long> B;

    while (raiz != nullptr) {
        //(*counter_comparisons)++;

        pair<long long, unique_ptr<Node>> min_node = extrai_minimo(move(raiz), counter_comparisons, counter_movements);;
        //(*counter_movements)++;

        B.push_back(min_node.first);

        //(*counter_movements)++;
        raiz = move(min_node.second);
    }
    
    return B;
}

