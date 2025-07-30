#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <memory>
#include <functional>
#include <vector>

/**
 * @brief Generic Linked List Node
 */
template<typename T>
class ListNode {
public:
    T data;
    std::shared_ptr<ListNode<T>> next;

    ListNode(const T& value) : data(value), next(nullptr) {}
    ~ListNode() = default;
};

/**
 * @brief Generic Singly Linked List
 * Simple linked list with standard operations
 */
template<typename T>
class LinkedList {
private:
    std::shared_ptr<ListNode<T>> head;
    int count;

    std::shared_ptr<ListNode<T>> getNodeAt(int index) {
        if (index < 0 || index >= count) {
            return nullptr;
        }
        
        auto current = head;
        for (int i = 0; i < index && current != nullptr; i++) {
            current = current->next;
        }
        
        return current;
    }

    std::shared_ptr<ListNode<T>> findNode(const T& value) {
        auto current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

public:
    LinkedList() : head(nullptr), count(0) {}

    ~LinkedList() {
        clear();
    }

    // Basic operations
    bool add(const T& value) {
        return addLast(value);
    }

    bool addFirst(const T& value) {
        auto newNode = std::make_shared<ListNode<T>>(value);
        newNode->next = head;
        head = newNode;
        count++;
        return true;
    }

    bool addLast(const T& value) {
        auto newNode = std::make_shared<ListNode<T>>(value);
        
        if (head == nullptr) {
            head = newNode;
        } else {
            auto current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        count++;
        return true;
    }

    bool insert(int index, const T& value) {
        if (index < 0 || index > count) {
            return false;
        }
        
        if (index == 0) {
            return addFirst(value);
        }
        
        auto newNode = std::make_shared<ListNode<T>>(value);
        auto prev = getNodeAt(index - 1);
        
        if (prev == nullptr) {
            return false;
        }
        
        newNode->next = prev->next;
        prev->next = newNode;
        count++;
        return true;
    }
    
    bool remove(const T& value) {
        if (head == nullptr) {
            return false;
        }
        
        if (head->data == value) {
            head = head->next;
            count--;
            return true;
        }
        
        auto current = head;
        while (current->next != nullptr && !(current->next->data == value)) {
            current = current->next;
        }
        
        if (current->next != nullptr) {
            current->next = current->next->next;
            count--;
            return true;
        }
        
        return false;
    }

    bool removeAt(int index) {
        if (index < 0 || index >= count) {
            return false;
        }
        
        if (index == 0) {
            return removeFirst();
        }
        
        auto prev = getNodeAt(index - 1);
        if (prev == nullptr || prev->next == nullptr) {
            return false;
        }
        
        prev->next = prev->next->next;
        count--;
        return true;
    }

    bool removeFirst() {
        if (head == nullptr) {
            return false;
        }
        
        head = head->next;
        count--;
        return true;
    }

    bool removeLast() {
        if (head == nullptr) {
            return false;
        }
        
        if (head->next == nullptr) {
            head = nullptr;
            count--;
            return true;
        }
        
        auto current = head;
        while (current->next->next != nullptr) {
            current = current->next;
        }
        
        current->next = nullptr;
        count--;
        return true;
    }
    
    // Access operations
    T* search(const T& value) {
        auto node = findNode(value);
        return node ? &(node->data) : nullptr;
    }

    const T* search(const T& value) const {
        auto current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    T* get(int index) {
        auto node = getNodeAt(index);
        return node ? &(node->data) : nullptr;
    }

    const T* get(int index) const {
        if (index < 0 || index >= count) {
            return nullptr;
        }
        
        auto current = head;
        for (int i = 0; i < index && current != nullptr; i++) {
            current = current->next;
        }
        
        return current ? &(current->data) : nullptr;
    }

    T* first() {
        return head ? &(head->data) : nullptr;
    }

    const T* first() const {
        return head ? &(head->data) : nullptr;
    }

    T* last() {
        if (head == nullptr) {
            return nullptr;
        }
        
        auto current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        return &(current->data);
    }

    const T* last() const {
        if (head == nullptr) {
            return nullptr;
        }
        
        auto current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        return &(current->data);
    }
    
    // Properties
    int size() const { return count; }
    bool isEmpty() const { return head == nullptr; }
    
    void clear() {
        while (head != nullptr) {
            head = head->next;
        }
        count = 0;
    }
    
    // Utility operations
    int indexOf(const T& value) const {
        auto current = head;
        int index = 0;
        
        while (current != nullptr) {
            if (current->data == value) {
                return index;
            }
            current = current->next;
            index++;
        }
        
        return -1;
    }

    bool contains(const T& value) const {
        return indexOf(value) >= 0;
    }
    
    // Advanced operations
    void sort(std::function<bool(const T&, const T&)> compare) {
        if (count <= 1) {
            return;
        }
        
        // Simple bubble sort for linked list
        bool swapped;
        do {
            swapped = false;
            auto current = head;
            
            while (current != nullptr && current->next != nullptr) {
                if (compare(current->next->data, current->data)) {
                    // Swap data
                    T temp = current->data;
                    current->data = current->next->data;
                    current->next->data = temp;
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
    }

    LinkedList<T> filter(std::function<bool(const T&)> predicate) const {
        LinkedList<T> result;
        auto current = head;
        
        while (current != nullptr) {
            if (predicate(current->data)) {
                result.addLast(current->data);
            }
            current = current->next;
        }
        
        return result;
    }

    void forEach(std::function<void(const T&)> callback) const {
        auto current = head;
        while (current != nullptr) {
            callback(current->data);
            current = current->next;
        }
    }

    void reverse() {
        if (count <= 1) {
            return;
        }
        
        std::shared_ptr<ListNode<T>> prev = nullptr;
        auto current = head;
        
        while (current != nullptr) {
            auto next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        
        head = prev;
    }
    
    // Iterator support
    class Iterator {
    private:
        std::shared_ptr<ListNode<T>> current;
    public:
        Iterator(std::shared_ptr<ListNode<T>> node) : current(node) {}
        
        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            if (current) current = current->next;
            return temp;
        }
        
        T& operator*() {
            return current->data;
        }
        
        T* operator->() {
            return &(current->data);
        }
        
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };
    
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

#endif // LINKED_LIST_H