#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdexcept>

template<typename T>
class DynamicArray {
private:
    T* data;
    size_t size;
    size_t capacity;
    
    void resize() {
        capacity = capacity == 0 ? 1 : capacity * 2;
        T* newData = new T[capacity];
        
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        
        delete[] data;
        data = newData;
    }

public:
    // Constructor
    DynamicArray() : data(nullptr), size(0), capacity(0) {}
    
    // Destructor
    ~DynamicArray() {
        delete[] data;
    }
    
    // Copy constructor
    DynamicArray(const DynamicArray& other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    
    // Assignment operator
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // Add element to end
    void push_back(const T& value) {
        if (size >= capacity) {
            resize();
        }
        data[size++] = value;
    }
    
    // Remove last element
    void pop_back() {
        if (size > 0) {
            --size;
        }
    }
    
    T get(size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void set(size_t index, const T& value) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        data[index] = value;
    }
    
    // Capacity methods
    size_t getSize() const {
        return size;
    }
    
    size_t getCapacity() const {
        return capacity;
    }
    
    bool empty() const {
        return size == 0;
    }
    
    bool isEmpty() const {
        return size == 0;
    }
    
    // Clear all elements
    void clear() {
        size = 0;
    }
};

#endif // DYNAMIC_ARRAY_H
