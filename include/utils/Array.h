#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>
class Array {
private:
    std::vector<T> data;

public:
    Array() = default;

    explicit Array(size_t size, const T& defaultValue = T()) : data(size, defaultValue) {}

    size_t size() const {
        return data.size();
    }

    bool empty() const {
        return data.empty();
    }

    void push(const T& value) {
        data.push_back(value);
    }

    void add(const T& value) {
        push(value);
    }

    void pop() {
        if (data.empty()) throw std::out_of_range("Pop from empty array");
        data.pop_back();
    }

    void remove(const T& value) {
        auto it = std::find(data.begin(), data.end(), value);
        if (it != data.end()) {
            data.erase(it);
        }
    }

    void removeAt(size_t index) {
        if (index >= data.size()) throw std::out_of_range("removeAt index out of range");
        data.erase(data.begin() + index);
    }

    T& operator[](size_t index) {
        return data.at(index);
    }

    const T& operator[](size_t index) const {
        return data.at(index);
    }

    void clear() {
        data.clear();
    }

    void resize(size_t newSize, const T& value = T()) {
        data.resize(newSize, value);
    }

    bool isEmpty() const {
        return data.empty();
    }

    T& front() {
        if (data.empty()) throw std::out_of_range("Front from empty array");
        return data.front();
    }

    T& back() {
        if (data.empty()) throw std::out_of_range("Back from empty array");
        return data.back();
    }

    // Iterators support
    typename std::vector<T>::iterator begin() { return data.begin(); }
    typename std::vector<T>::iterator end() { return data.end(); }
    typename std::vector<T>::const_iterator begin() const { return data.begin(); }
    typename std::vector<T>::const_iterator end() const { return data.end(); }
};

#endif // ARRAY_H
