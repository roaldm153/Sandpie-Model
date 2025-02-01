#include <iostream>
#include <cstddef>
#include <cstdint>

#include "storage.h"

uint32_t HashTable::PairingFunction(uint32_t x, uint32_t y) {
    return ((x + y) * (x + y + 1) / 2) + y;
}
uint32_t HashTable::HashFunction(uint32_t x) {
    return (x << shift_size) >> shift_size;
}

void HashTable::Reallocate() {
    Node* old_array = array;
    array_size *= 2;
    --shift_size;
    array = new Node[array_size]{ };
    for (std::size_t i = 0; i < array_size / 2; ++i) {
        if (old_array[i].value_ != 0) {
            Put(old_array[i].x_, old_array[i].y_, old_array[i].value_);
        }
        if (old_array[i].pointer_ != nullptr) {
            Node* iterator = &old_array[i];
            while (iterator->pointer_ != nullptr) {
                Put(iterator->pointer_->x_, iterator->pointer_->y_, iterator->pointer_->value_);
                iterator = iterator->pointer_;
            }
        }
    }
    delete[] old_array;
}

uint32_t HashTable::GetHash(int32_t x, int32_t y) {
    if (x >= 0) {
        x = 2 * x;
    } else {
        x = -2 * x - 1;
    }
    if (y >= 0) {
        y = 2 * y;
    } else {
        y = -2 * y - 1;
    }

    return HashFunction(PairingFunction(x, y));
}

void HashTable::Put(int16_t x, int16_t y, uint64_t value) {
    uint32_t key = GetHash(x, y);
    while (key >= array_size) {
        Reallocate();
    }
    if (array[key].value_ == 0) {
        array[key].value_ = value;
        array[key].x_ = x;
        array[key].y_ = y;
    } else {
        Node* ptr = &array[key];
        while (ptr->pointer_ != nullptr) {
            ptr = ptr->pointer_;
        }
        Node* next_node = new Node;
        next_node->value_ = value;
        next_node->x_ = x;
        next_node->y_ = y;
        ptr->pointer_ = next_node;
    }
    ++cells_number;
}

Node* HashTable::Get(int16_t x, int16_t y) {
    uint32_t key = GetHash(x, y);
    if (array[key].x_ == x && array[key].y_ == y) {
        return &array[key];
    }
    Node* ptr = array[key].pointer_;
    while (ptr != nullptr && !(ptr->x_ == x && ptr->y_ == y)) {
        ptr = ptr->pointer_;
    }
    return ptr;
}

Node& HashTable::IncreaseCoordinate(int16_t x, int16_t y, uint64_t value) {
    Node* cell_pointer = Get(x, y);
    if (cell_pointer == nullptr) {
        Put(x, y, value);
        return *Get(x, y);
    } else {
        cell_pointer->value_ += value;
        return *cell_pointer;
    }
}

void HashTable::CollapseCell(Node& cell, Stack& right_stack) {
    if (cell.value_ >= 4) {
        cell.in_right_stack = false;
        int16_t x_1 = cell.x_ - 1;
        int16_t y_1 = cell.y_;
        int16_t x_2 = cell.x_ + 1;
        int16_t y_2 = cell.y_;
        int16_t x_3 = cell.x_;
        int16_t y_3 = cell.y_ - 1;
        int16_t x_4 = cell.x_;
        int16_t y_4 = cell.y_ + 1;
        Node& cell_1 = IncreaseCoordinate(x_1, y_1, cell.value_ / 4);
        if (cell_1.value_ >= 4 && !cell_1.in_right_stack) {
            cell_1.in_right_stack = true;
            right_stack.Push(cell_1.x_, cell_1.y_);
        }
        Node& cell_2 = IncreaseCoordinate(x_2, y_2, cell.value_ / 4);
        if (cell_2.value_ >= 4 && !cell_2.in_right_stack) {
            cell_2.in_right_stack = true;
            right_stack.Push(cell_2.x_, cell_2.y_);
        }
        Node& cell_3 = IncreaseCoordinate(x_3, y_3, cell.value_ / 4);
        if (cell_3.value_ >= 4 && !cell_3.in_right_stack) {
            cell_3.in_right_stack = true;
            right_stack.Push(cell_3.x_, cell_3.y_);
        }
        Node& cell_4 = IncreaseCoordinate(x_4, y_4, cell.value_ / 4);
        if (cell_4.value_ >= 4 && !cell_4.in_right_stack) {
            cell_4.in_right_stack = true;
            right_stack.Push(cell_4.x_, cell_4.y_);
        }
        cell.value_ %= 4;
    }
}

void HashTable::TopplingIteration(Stack& left_stack, Stack& right_stack) {
    double load_factor = static_cast<double>(cells_number) / static_cast<double>(array_size);
    if (load_factor >= 0.8) {
        Reallocate();
    }
    while (!left_stack.IsEmpty()) {
        Coordinates cell_coordinates = left_stack.Pop();
        Node* cell = Get(cell_coordinates.x_, cell_coordinates.y_);
        CollapseCell(*cell, right_stack);
    }
    left_stack.swap(right_stack);
}

void HashTable::GetMinAndMax() {
    for (std::size_t i = 0; i < array_size; ++i) {
        if (array[i].value_ != 0) {
            max_coordinate_x = std::max(max_coordinate_x, array[i].x_);
            max_coordinate_y = std::max(max_coordinate_y, array[i].y_);
            min_coordinate_x = std::min(min_coordinate_x, array[i].x_);
            min_coordinate_y = std::min(min_coordinate_y, array[i].y_);
        }
        Node* iterator = array[i].pointer_;
        while (iterator != nullptr) {
            if (iterator->value_ != 0) {
                max_coordinate_x = std::max(max_coordinate_x, iterator->x_);
                max_coordinate_y = std::max(max_coordinate_y, iterator->y_);
                min_coordinate_x = std::min(min_coordinate_x, iterator->x_);
                min_coordinate_y = std::min(min_coordinate_y, iterator->y_);
            }
            iterator = iterator->pointer_;
        }
    }
}

uint32_t HashTable::GetHashTableSize() {
    return array_size;
}

void HashTable::GetInputValues(const char* input_filename, Stack& left_stack) {
    const uint32_t kBufferSize = 512;
    FILE* input = std::fopen(input_filename, "r");
    if (!input) {
        std::cerr << "Error reading input file!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    char buffer[kBufferSize] { };
    const char* read = std::fgets(buffer, kBufferSize, input);
    while (read != nullptr) {
        int32_t x;
        int32_t y;
        uint32_t value;
        std::sscanf(read, "%d%d%d", &x, &y, &value);
        Put(static_cast<int16_t>(x), static_cast<int16_t>(y), static_cast<uint64_t>(value));
        if (value >= 4) {
            left_stack.Push(static_cast<int16_t>(x), static_cast<int16_t>(y));
        }
        read = std::fgets(buffer, kBufferSize, input);
    }
    fclose(input);
}

void Stack::Push(int16_t x, int16_t y) {
    if (stack_size_ > array_size_ - 1) {
        Reallocate();
    }
    array[stack_size_].x_ = x;
    array[stack_size_].y_ = y;
    ++stack_size_;
}

void Stack::Reallocate() {
    Coordinates* old_array = array;
    array_size_*= 2;
    array = new Coordinates[array_size_] { };
    std::copy(old_array, (old_array + stack_size_), array);
    delete[] old_array;
}

Coordinates Stack::Pop() {
    return array[--stack_size_];
}

bool Stack::IsEmpty() {
    return (stack_size_ == 0);
}

Coordinates Stack::Top() {
    return array[stack_size_];
}