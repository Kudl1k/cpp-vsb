#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <cstdint>
#include <functional>
#include <iostream>
#include <utility>
#include <cstring>
#include <optional>
#include <vector>

/// UTF-8 string (reuse your previous implementation and modify it)

using CodePoint = uint32_t;

class UTF8String;


class ByteIterator {
public:
    ByteIterator(const UTF8String& s, int i) : str(s), index(i) {}
    uint8_t operator*() const;
    ByteIterator& operator++();
    ByteIterator& operator+=(int n);
    ByteIterator& operator-=(int n);
    ByteIterator operator+(int n) const;
    ByteIterator operator-(int n) const;
    bool operator==(const ByteIterator& other) const;
    bool operator!=(const ByteIterator& other) const;
    ByteIterator begin() const;
    ByteIterator end() const;
private:
    const UTF8String& str;
    int index;
};

class CodePointIterator {
public:
    CodePointIterator(const UTF8String& s, int i) : str(s), index(i) {}
    CodePoint operator*() const;
    CodePointIterator& operator++();
    CodePointIterator& operator--();
    bool operator==(const CodePointIterator& other) const;
    bool operator!=(const CodePointIterator& other) const;
    CodePointIterator begin() const;
    CodePointIterator end() const;
private:
    const UTF8String& str;
    int index;
};


class UTF8String {
public:
    UTF8String() = default;
    UTF8String(const std::string& s);
    UTF8String(const std::vector<CodePoint>&ptr);
    UTF8String(const std::vector<uint8_t>& ptr);
    UTF8String(const UTF8String &ptr);
    UTF8String(UTF8String&& other);

    int get_number_of_bytes(CodePoint ptr);

    int get_byte_count() const;
    int get_point_count() const;
    int get_reserved_count() const;

    void append(const char c);
    void append(const CodePoint cp);

    std::optional<uint8_t> operator[](const size_t idx) const;
    std::optional<CodePoint> nth_code_point(const size_t idx) const;
    friend UTF8String operator+(const UTF8String &lhs,const UTF8String &rhs);
    UTF8String& operator=(const UTF8String &ptr);
    UTF8String& operator+=(const UTF8String& str);
    UTF8String& operator=(UTF8String&& other);
    friend bool operator==(const UTF8String &lhs,const UTF8String &rhs);
    friend bool operator!=(const UTF8String &lhs,const UTF8String &rhs);

    operator std::string() const {
        std::string result;
        for (size_t i = 0; i < points; i++)
        {
            result.push_back(static_cast<char>(buffer.at(i)));
        }
        return result;
    }
    
    ByteIterator bytes() const {
        return ByteIterator(*this, 0);
    }
    
    CodePointIterator codepoints() const {
        return CodePointIterator(*this, 0);
    }


    ~UTF8String() = default;
private:
    size_t points = 0;
    size_t bytes_count = 0;

    std::vector<uint8_t> buffer;
};






// /// Binary tree
// // Big data that we cannot afford to copy
// struct BigData {
//     explicit BigData(int value): value(value) {}

//     BigData(const BigData&) = delete;
//     BigData& operator=(const BigData&) = delete;

//     int value;
// };

// // Define Tree class
// class Tree {
// private:
//     std::shared_ptr<BigData> value;
//     Tree* parent;
//     std::unique_ptr<Tree> left;
//     std::unique_ptr<Tree> right;

// public:
//     // Constructor
//     Tree(const std::shared_ptr<BigData>& val) : value(val), parent(nullptr) {}

//     // Method to get the value of a node
//     const BigData& get_value() const {
//         return *value;
//     }

//     // Method to get the parent of a node
//     Tree* get_parent() const {
//         return parent;
//     }

//     // Method to check if a node has a parent
//     bool has_parent() const {
//         return parent != nullptr;
//     }

//     // Method to get the left child of a node
//     std::unique_ptr<Tree> get_left_child() {
//         return std::move(left);
//     }

//     // Method to get the right child of a node
//     std::unique_ptr<Tree> get_right_child() {
//         return std::move(right);
//     }

//     // Method to set the left child of a node
//     std::unique_ptr<Tree> set_left_child(std::unique_ptr<Tree> new_left) {
//         std::swap(left, new_left);
//         if (left)
//             left->parent = this;
//         return new_left;
//     }

//     // Method to set the right child of a node
//     std::unique_ptr<Tree> set_right_child(std::unique_ptr<Tree> new_right) {
//         std::swap(right, new_right);
//         if (right)
//             right->parent = this;
//         return new_right;
//     }

//     // Method to take the left child of a node
//     std::unique_ptr<Tree> take_left_child() {
//         auto child = std::move(left);
//         if (child)
//             child->parent = nullptr;
//         return child;
//     }

//     // Method to take the right child of a node
//     std::unique_ptr<Tree> take_right_child() {
//         auto child = std::move(right);
//         if (child)
//             child->parent = nullptr;
//         return child;
//     }

//     // Method to take a specific child of a node
//     std::unique_ptr<Tree> take_child(Tree& child) {
//         if (left.get() == &child) {
//             return take_left_child();
//         } else if (right.get() == &child) {
//             return take_right_child();
//         } else {
//             throw std::invalid_argument("Given node is not a child of the current node");
//         }
//     }

//     // Method to swap the left and right children of a node
//     void swap_children() {
//         std::swap(left, right);
//     }

//     // Method to check if two nodes belong to the same tree
//     bool is_same_tree_as(const Tree* other) const {
//         return this == other;
//     }

//     // Method to replace the shared value of the current node and its descendants
//     void replace_value(const std::shared_ptr<BigData>& new_value) {
//         value = new_value;
//     }
// };