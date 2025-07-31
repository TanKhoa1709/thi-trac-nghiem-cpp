#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <memory>
#include <functional>
#include <vector>
#include <stdexcept>
#include <algorithm>

/**
 * @brief Generic Binary Search Tree Node
 */
template<typename T>
class TreeNode {
public:
    T data;
    std::shared_ptr<TreeNode<T>> left;
    std::shared_ptr<TreeNode<T>> right;

    TreeNode(const T& value) : data(value), left(nullptr), right(nullptr) {}
    ~TreeNode() = default;
};

/**
 * @brief Generic Binary Search Tree
 * Simple binary search tree with standard operations
 */
template<typename T>
class BinarySearchTree {
private:
    std::shared_ptr<TreeNode<T>> root;
    int nodeCount;
    std::function<bool(const T&, const T&)> compareLess; // Returns true if first < second

    // Helper methods
    std::shared_ptr<TreeNode<T>> insertNode(std::shared_ptr<TreeNode<T>> node, const T& value) {
        if (node == nullptr) {
            nodeCount++;
            return std::make_shared<TreeNode<T>>(value);
        }
        
        if (compareLess(value, node->data)) {
            node->left = insertNode(node->left, value);
        } else if (compareLess(node->data, value)) {
            node->right = insertNode(node->right, value);
        }
        // If equal, don't insert (no duplicates)
        
        return node;
    }

    std::shared_ptr<TreeNode<T>> removeNode(std::shared_ptr<TreeNode<T>> node, const T& value) {
        if (node == nullptr) {
            return node;
        }
        
        if (compareLess(value, node->data)) {
            node->left = removeNode(node->left, value);
        } else if (compareLess(node->data, value)) {
            node->right = removeNode(node->right, value);
        } else {
            // Node to be deleted found
            nodeCount--;
            
            if (node->left == nullptr) {
                return node->right;
            } else if (node->right == nullptr) {
                return node->left;
            }
            
            // Node with two children
            std::shared_ptr<TreeNode<T>> minRight = findMinNode(node->right);
            node->data = minRight->data;
            node->right = removeNode(node->right, minRight->data);
            nodeCount++; // Compensate for the decrement above
        }
        
        return node;
    }

    std::shared_ptr<TreeNode<T>> findMinNode(std::shared_ptr<TreeNode<T>> node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    TreeNode<T>* searchNode(std::shared_ptr<TreeNode<T>> node, const T& value) {
        if (node == nullptr) {
            return nullptr;
        }
        
        if (compareLess(value, node->data)) {
            return searchNode(node->left, value);
        } else if (compareLess(node->data, value)) {
            return searchNode(node->right, value);
        } else {
            return node.get();
        }
    }

    void inOrderTraversal(std::shared_ptr<TreeNode<T>> node, std::vector<T>& result) const {
        if (node != nullptr) {
            inOrderTraversal(node->left, result);
            result.push_back(node->data);
            inOrderTraversal(node->right, result);
        }
    }

    void preOrderTraversal(std::shared_ptr<TreeNode<T>> node, std::vector<T>& result) const {
        if (node != nullptr) {
            result.push_back(node->data);
            preOrderTraversal(node->left, result);
            preOrderTraversal(node->right, result);
        }
    }

    void postOrderTraversal(std::shared_ptr<TreeNode<T>> node, std::vector<T>& result) const {
        if (node != nullptr) {
            postOrderTraversal(node->left, result);
            postOrderTraversal(node->right, result);
            result.push_back(node->data);
        }
    }

    void destroyTree(std::shared_ptr<TreeNode<T>> node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            node.reset();
        }
    }

    int calculateHeight(std::shared_ptr<TreeNode<T>> node) const {
        if (node == nullptr) {
            return -1;
        }
        
        int leftHeight = calculateHeight(node->left);
        int rightHeight = calculateHeight(node->right);
        
        return 1 + std::max(leftHeight, rightHeight);
    }

public:
    BinarySearchTree() : root(nullptr), nodeCount(0) {
        compareLess = [](const T& a, const T& b) { return a < b; };
    }

    BinarySearchTree(std::function<bool(const T&, const T&)> compareFunc) 
        : root(nullptr), nodeCount(0), compareLess(compareFunc) {
    }

    ~BinarySearchTree() {
        clear();
    }

    // Tree operations
    bool insert(const T& value) {
        int oldCount = nodeCount;
        root = insertNode(root, value);
        return nodeCount > oldCount;
    }

    bool remove(const T& value) {
        int oldCount = nodeCount;
        root = removeNode(root, value);
        return nodeCount < oldCount;
    }

    T* search(const T& value) {
        TreeNode<T>* node = searchNode(root, value);
        return node ? &(node->data) : nullptr;
    }
    
    // Properties
    int size() const { return nodeCount; }
    bool isEmpty() const { return root == nullptr; }
    
    void clear() {
        destroyTree(root);
        root = nullptr;
        nodeCount = 0;
    }
    
    // Traversal methods
    std::vector<T> inOrder() const {
        std::vector<T> result;
        inOrderTraversal(root, result);
        return result;
    }

    std::vector<T> preOrder() const {
        std::vector<T> result;
        preOrderTraversal(root, result);
        return result;
    }

    std::vector<T> postOrder() const {
        std::vector<T> result;
        postOrderTraversal(root, result);
        return result;
    }
    
    // Utility methods
    bool contains(const T& value) const {
        return search(value) != nullptr;
    }

    T findMin() const {
        if (isEmpty()) {
            throw std::runtime_error("Tree is empty");
        }
        std::shared_ptr<TreeNode<T>> node = findMinNode(root);
        return node->data;
    }

    T findMax() const {
        if (isEmpty()) {
            throw std::runtime_error("Tree is empty");
        }
        std::shared_ptr<TreeNode<T>> node = root;
        while (node->right != nullptr) {
            node = node->right;
        }
        return node->data;
    }

    int height() const {
        return calculateHeight(root);
    }
};

#endif // BINARY_TREE_H