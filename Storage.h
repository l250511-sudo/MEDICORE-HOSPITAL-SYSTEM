#pragma once
#include <iostream>

template <typename T>
class Storage {
private:
    T data[100]; // Static array as strictly required by the prompt
    int count;

public:
    // Constructor
    Storage() {
        count = 0;
    }

    // Add an item to the array
    bool add(T item) {
        if (count < 100) {
            *(data + count) = item; // Offset notation instead of data[count]
            count++;
            return true;
        }
        return false; // Array is full
    }

    // Find an item by its ID
    T findByID(int id) {
        for (int i = 0; i < count; i++) {
            // Check if pointer is not null, then check ID using offset notation
            if (*(data + i) != nullptr && (*(data + i))->getID() == id) {
                return *(data + i);
            }
        }
        return nullptr; // Not found
    }

    // Remove an item by ID and shift the remaining elements to avoid gaps
    bool removeByID(int id) {
        for (int i = 0; i < count; i++) {
            if (*(data + i) != nullptr && (*(data + i))->getID() == id) {
                // Shift all subsequent elements left by one using offset notation
                for (int j = i; j < count - 1; j++) {
                    *(data + j) = *(data + j + 1);
                }
                count--;
                return true; // Successfully removed
            }
        }
        return false; // ID not found
    }

    // Return the raw array
    T* getAll() {
        return data; // 'data' is already a pointer to the first element
    }

    // Get the current number of elements
    int size() const {
        return count;
    }
};