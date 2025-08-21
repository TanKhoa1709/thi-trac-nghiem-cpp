#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#include <iostream>

#include <stdexcept>

template<typename T>
class DynamicArray {
private:
    T *data;
    int capacity;
    int current_size;

    void resize() {
        capacity *= 2;
        T *temp = new T[capacity];
        for (int i = 0; i < current_size; i++) {
            temp[i] = data[i];
        }
        delete[] data;
        data = temp;
    }

public:
    DynamicArray(int initial_capacity = 10) :
        capacity(initial_capacity), current_size(0) {
        data = new T[capacity];
    }

    ~DynamicArray() { delete[] data; }

    void add(T &value) {
        if (current_size >= capacity) {
            resize();
        }
        data[current_size++] = value;
    }

    void addCopy(T value) {
        if (current_size >= capacity) {
            resize();
        }
        data[current_size++] = value;
    }

    void remove(T &value) {
        for (int i = 0; i < current_size; i++) {
            if (data[i] == value) {
                data[i] = T(); // Reset the value

                // Shift elements left
                for (int j = i; j < current_size - 1; j++) {
                    data[j] = data[j + 1];
                }
                current_size--;
                break;
            }
        }
    }

    void removeIndex(int index) {
        if (index < 0 || index >= current_size) {
            throw std::out_of_range("Index out of bounds");
        }

        data[index] = T(); // Reset the value

        // Shift elements left
        for (int i = index; i < current_size - 1; i++) {
            data[i] = data[i + 1];
        }
        current_size--;
    }

    T &get(int index) {
        if (index < 0 || index >= current_size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    void set(int index, T &value) {
        if (index < 0 || index >= current_size) {
            throw std::out_of_range("Index out of bounds");
        }
        data[index] = value;
    }

    void setCopy(int index, T value) {
        if (index < 0 || index >= current_size) {
            throw std::out_of_range("Index out of bounds");
        }
        data[index] = value;
    }

    int size() { return current_size; }

    bool isEmpty() { return current_size == 0; }

    void clear() {
        for (int i = 0; i < current_size; i++) {
            data[i] = T(); // Reset the value
        }
        current_size = 0;
    }

    void popBack() {
        if (current_size > 0) {
            current_size--;
        }
    }
};

#endif // DYNAMIC_ARRAY_H
