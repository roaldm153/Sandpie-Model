#pragma once
#include <utility>
#include <cstring>
#include <cstdint>


class Node {
public:
    uint64_t value_ = 0;
    Node* pointer_ = nullptr;
    int16_t x_;
    int16_t y_;
    bool in_right_stack = false;
    ~Node() {
        if (pointer_ != nullptr) {
            delete pointer_;
        }
    }
    Node() = default;
    Node(int16_t x, int16_t y, uint64_t value): x_(x), y_(y), value_(value) {};
};

class Coordinates {
public:    
    int16_t x_;
    int16_t y_;
    Coordinates(int16_t x, int16_t y): x_(x), y_(y) {};
    Coordinates(const Coordinates& other): x_(other.x_), y_(other.y_) { };
    Coordinates() = default;
};

class Stack {
private:
    int32_t array_size_ = 1;
    int32_t stack_size_ = 0;
public:
    Coordinates* array = new Coordinates[array_size_] { };
    Stack() = default;
    Stack(int32_t size): array_size_(size) {};
    Stack(int32_t size, int32_t stack_size): array_size_(size), stack_size_(stack_size) {};

    ~Stack() {
        delete[] array;
    }

    Stack(const Stack& other): array_size_(other.array_size_), stack_size_(other.stack_size_) {
        memcpy(array, other.array, array_size_);
    }

    Stack& operator=(Stack& other) {
        swap(other);
        return *this;
    }

    void swap(Stack& other) {
        std::swap(array_size_, other.array_size_);
        std::swap(stack_size_, other.stack_size_);
        std::swap(array, other.array);
    }
    
    void Reallocate();
    void Push(int16_t, int16_t);
    Coordinates Pop();
    bool IsEmpty();
    Coordinates Top();
};

class HashTable {
private:
    uint32_t PairingFunction(uint32_t x, uint32_t y);
    uint32_t HashFunction(uint32_t x);
    uint32_t GetHash(int32_t x, int32_t y);
    void Reallocate();
    /* 
    размер хеш таблицы это степень двойки
    shift_size - степень двойки размера таблицы
    мы можем вычислять остаток от деления сдвигая биты
    */
    uint32_t array_size = 1;
    uint32_t shift_size = 32;
    int64_t cells_number = 0;
public:
    HashTable() = default;
    ~HashTable() {
        delete[] array;
    }
    Node* array = new Node[array_size];
    int16_t max_coordinate_x = INT16_MIN;
    int16_t max_coordinate_y = INT16_MIN;
    int16_t min_coordinate_x = INT16_MAX;
    int16_t min_coordinate_y = INT16_MAX;
    uint32_t width = 0;
    uint32_t heigh = 0;
    uint32_t GetHashTableSize();
    void Put(int16_t x, int16_t y, uint64_t value);
    void GetAndIncrease(int16_t x, int16_t y, uint64_t value);
    void TopplingIteration(Stack&, Stack&);
    Node& IncreaseCoordinate(int16_t x, int16_t y, uint64_t value);
    void CollapseCell(Node& cell, Stack&);
    void Print();
    void GetMinAndMax(); 
    bool IsStable();
    Node* Get(int16_t x, int16_t y);
    void GetInputValues(const char* input_filename, Stack& left_stack);
};