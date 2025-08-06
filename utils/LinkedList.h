#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdexcept>

template<typename T>
struct ListNode {
    T data;
    ListNode *next;

    ListNode(T &value) : data(value), next(nullptr) {
    }
};

template<typename T>
class LinkedList {
private:
    ListNode<T> *head;
    int current_size;

public:
    LinkedList() : head(nullptr), current_size(0) {
    }

    ~LinkedList() { clear(); }

    void add(T &value) {
        ListNode<T> *newNode = new ListNode<T>(value);
        if (head == nullptr) {
            head = newNode;
        } else {
            ListNode<T> *current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        current_size++;
    }

    void remove(T &value) {
        if (head == nullptr)
            return;

        if (head->data == value) {
            ListNode<T> *temp = head;
            head = head->next;
            delete temp;
            current_size--;
            return;
        }

        ListNode<T> *current = head;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }

        if (current->next != nullptr) {
            ListNode<T> *temp = current->next;
            current->next = temp->next;
            delete temp;
            current_size--;
        }
    }

    T &get(int index) {
        if (index < 0 || index >= current_size) {
            throw std::out_of_range("Index out of bounds");
        }

        ListNode<T> *current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    void set(int index, T &value) {
        if (index < 0 || index >= current_size) {
            throw std::out_of_range("Index out of bounds");
        }

        ListNode<T> *current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        current->data = value;
    }

    T *first() {
        if (head == nullptr) {
            return nullptr;
        }

        return &head->data;
    }

    void removeFirst() {
        if (head != nullptr) {
            ListNode<T> *temp = head;
            head = head->next;
            delete temp;
            current_size--;
        }
    }

    int size() { return current_size; }

    bool isEmpty() { return head == nullptr; }

    void clear() {
        while (head != nullptr) {
            ListNode<T> *temp = head;
            head = head->next;
            delete temp;
        }
        current_size = 0;
    }

    void forEach(void (*visit)(T &)) {
        ListNode<T> *current = head;
        while (current != nullptr) {
            visit(current->data);
            current = current->next;
        }
    }
};

#endif // LINKED_LIST_H
