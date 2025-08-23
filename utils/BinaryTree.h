#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdexcept>
#include <functional>

template<typename T>
struct TreeNode {
    int size; // Size of the subtree rooted at this node
    T data;
    TreeNode *left;
    TreeNode *right;

    TreeNode(T &value) :
        size(1), data(value), left(nullptr), right(nullptr) {
    }
};

template<typename T>
class BinarySearchTree {
private:
    TreeNode<T> *root;

    int getSize(TreeNode<T> *node) { return node ? node->size : 0; }

    void updateSize(TreeNode<T> *node) {
        if (node)
            node->size = 1 + getSize(node->left) + getSize(node->right);
    }

    TreeNode<T> *getKth(TreeNode<T> *node, int k) {
        if (node == nullptr || k < 0 || k >= getSize(node)) {
            return nullptr;
        }

        int leftSize = getSize(node->left);

        if (k < leftSize) {
            return getKth(node->left, k);
        } else if (k == leftSize) {
            return node;
        } else {
            return getKth(node->right, k - leftSize - 1);
        }
    }

    void insertHelper(TreeNode<T> *&node, T &value) {
        if (node == nullptr) {
            node = new TreeNode<T>(value);
        } else if (value < node->data) {
            insertHelper(node->left, value);
        } else if (value > node->data) {
            insertHelper(node->right, value);
        }

        updateSize(node);
    }

    TreeNode<T> *removeHelper(TreeNode<T> *node, T &value) {
        if (node == nullptr)
            return node;

        if (value < node->data) {
            node->left = removeHelper(node->left, value);
        } else if (value > node->data) {
            node->right = removeHelper(node->right, value);
        } else {
            if (node->left == nullptr) {
                TreeNode<T> *temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                TreeNode<T> *temp = node->left;
                delete node;
                return temp;
            }

            TreeNode<T> *temp = findMin(node->right);
            node->data = temp->data;
            node->right = removeHelper(node->right, temp->data);
        }

        updateSize(node);
        return node;
    }

    TreeNode<T> *findMin(TreeNode<T> *node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    TreeNode<T> *findNode(TreeNode<T> *node, T &value) {
        if (node == nullptr || node->data == value) {
            return node;
        }
        if (value < node->data) {
            return findNode(node->left, value);
        }
        return findNode(node->right, value);
    }

    void clearHelper(TreeNode<T> *node) {
        if (node != nullptr) {
            clearHelper(node->left);
            clearHelper(node->right);
            delete node;
        }
    }

    void inOrderHelper(TreeNode<T> *node, std::function<void(T &)> callback) {
        if (node != nullptr) {
            inOrderHelper(node->left, callback); // Visit left subtree
            callback(node->data); // Visit current node
            inOrderHelper(node->right, callback); // Visit right subtree
        }
    }

public:
    BinarySearchTree() :
        root(nullptr) {
    }

    ~BinarySearchTree() { clear(); }

    void add(T &value) {
        insertHelper(root, value);
    }

    void remove(T &value) { root = removeHelper(root, value); }

    T *find(T &value) {
        TreeNode<T> *node = findNode(root, value);
        return node ? &node->data : nullptr;
    }

    int size() { return getSize(root); }

    bool isEmpty() { return root == nullptr; }

    void clear() {
        clearHelper(root);
        root = nullptr;
    }

    void inOrderTraversal(std::function<void(T &)> callback) {
        inOrderHelper(root, callback);
    }
};

#endif // BINARY_TREE_H
